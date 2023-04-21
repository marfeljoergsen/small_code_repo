#!/usr/bin/env python

# ***DEPRECATED BELOW:***
#!/usr/bin/env python3.9
# WARNING: THIS REQUIRES PYTHON3.9 IN CONDA VENV - DO NOT USE SYSTEM PYTHON!
# -----------------------

from __future__ import print_function

import random
import os
import os.path
import sys
import argparse

from OCC.Core.Quantity import Quantity_Color, Quantity_TOC_RGB
from OCC.Display.SimpleGui import init_display

from OCC.Extend.TopologyUtils import TopologyExplorer
from OCC.Extend.DataExchange import read_step_file, \
        read_step_file_with_names_colors, read_iges_file, read_stl_file

# These are for loading .brep files:
from OCC.Core.TopoDS import TopoDS_Shape, TopoDS_Vertex, TopoDS_Edge, TopoDS_Shell, \
        TopoDS_Face, TopoDS_Compound

from OCC.Core.BRep import BRep_Builder
from OCC.Core.BRepTools import breptools_Read
from OCC.Core.BRep import BRep_Tool # for getting X/Y/Z-values of e.g. vertices

# For bounding-box:
from OCC.Core.Bnd import Bnd_Box
from OCC.Core.BRepMesh import BRepMesh_IncrementalMesh
from OCC.Core.BRepBndLib import brepbndlib_Add

# For recognize face features as in "core_geometry_recognize_feature.py":
from OCC.Core.BRepAdaptor import BRepAdaptor_Surface
from OCC.Core.GeomAbs import GeomAbs_Plane, GeomAbs_Cylinder, GeomAbs_Cone,\
     GeomAbs_Sphere, GeomAbs_Torus, GeomAbs_BezierSurface, GeomAbs_BSplineSurface,\
     GeomAbs_SurfaceOfRevolution, GeomAbs_SurfaceOfExtrusion, GeomAbs_OffsetSurface,\
     GeomAbs_OtherSurface

# For showing coordinate system: - WARNING: INCOMPLETE!!!
# NB: SEE: https://dev.opencascade.org/content/how-display-coordinate-system
#     and: https://dev.opencascade.org/content/selecting-aistrihedron
#   maybe: https://dev.opencascade.org//content/how-make-objectaistrihedron-unselectable
from OCC.Core import AIS
from OCC.Core.AIS import AIS_Trihedron
from OCC.Core.Geom import Geom_Axis2Placement
import OCC.Core.gp as gp


class cadfileDebugger:
    def __init__(self, args):
        self.fname = args.filename
        self.printOnly = args.print_only
        self.verbose = args.verbose
        self.shp = None # Init, e.g. needed for "self.recognize_face"
        if not self.printOnly:
            self.display, self.start_display, self.add_menu, self.add_function_to_menu = init_display()
        else:
            print('--- PRINT-ONLY MODE ---')
            self.import_as_one_shape()

            # consider only this for self.verbose (not implemented yet)
            self.printShapeInfo() 
            print(' ')
            if self.verbose:
                self.recognize_batch()
                print(' ')

            # This is generally useful to chceking coor-sys transforms
            self.printBBInfo()
            #self.printBBInfo(use_mesh=False)
            sys.exit(0)

    def printShapeInfo(self):
        t = TopologyExplorer(self.shp) # See: /usr/lib/python3.10/site-packages/OCC/Extend/TopologyUtils.py
        print('=== TopologyExplorer reports the following: ===')
        if t.number_of_faces()>0:
            print('  number_of_faces = ', t.number_of_faces())
        if t.number_of_vertices()>0:
            print('  number_of_vertices = ', t.number_of_vertices())
        if t.number_of_edges()>0:
            print('  number_of_edges = ', t.number_of_edges())
        if t.number_of_wires()>0:
            print('  number_of_wires = ', t.number_of_wires())
        if t.number_of_shells()>0:
            print('  number_of_shells = ', t.number_of_shells())
        if t.number_of_solids()>0:
            print('  number_of_solids = ', t.number_of_solids())
        if t.number_of_comp_solids()>0:
            print('  number_of_comp_solids = ', t.number_of_comp_solids())
        if t.number_of_compounds()>0:
            print('  number_of_compounds = ', t.number_of_compounds())
    
    def printBBInfo(self, use_mesh=False):
        # use_mesh=True returns approximate values, use_mesh=False seems more accurate!
        bb = self.get_boundingbox(self.shp, use_mesh)
        halfdelta = tuple(l/2 for l in bb[6:9])
        print("Bounding box:")
        print("  min(x,y,z)=", bb[0:3])
        print("  max(x,y,z)=", bb[3:6])
        print("  delta(x,y,z)=", bb[6:9])
        print("  center(x,y,z)=", tuple([sum(x) for x in zip(bb[0:3],halfdelta)]) )
        print()
        #import ipdb; ipdb.set_trace()
    
    def get_boundingbox(self, shape, tol=1e-8, use_mesh=True):
        """return the bounding box of the TopoDS_Shape `shape`
        Parameters
        ----------
        shape : TopoDS_Shape or a subclass such as TopoDS_Face
            the shape to compute the bounding box from
        tol: float
            tolerance of the computed boundingbox
        use_mesh : bool
            a flag that tells whether or not the shape has first to be meshed before the bbox
            computation. This produces more accurate results
        """
        bbox = Bnd_Box()
        bbox.SetGap(tol)
        if use_mesh: # very approximate solution
            mesh = BRepMesh_IncrementalMesh()
            mesh.SetParallelDefault(True)
            mesh.SetShape(shape)
            mesh.Perform()
            if not mesh.IsDone():
                raise AssertionError("Mesh not done.")
        brepbndlib_Add(shape, bbox, use_mesh)
    
        xmin, ymin, zmin, xmax, ymax, zmax = bbox.Get()
        # Return a tuple: min(x,y,z) ; max(x,y,z) ; delta(x,y,z)
        return xmin, ymin, zmin, xmax, ymax, zmax, xmax - xmin, ymax - ymin, zmax - zmin
    
    
    def recognize_face(self, a_face):
        """Takes a TopoDS shape and tries to identify its nature
        whether it is a plane a cylinder a torus etc.
        if a plane, returns the normal
        if a cylinder, returns the radius
        """
        if not type(a_face) is TopoDS_Face:
            print("A face was not selected: Please hit the 'G' key to switch to face selection mode")
            return False
        surf = BRepAdaptor_Surface(a_face, True)
        # The Surface from BRepAdaptor allows to use a Face of the BRep topology look
        # like a 3D surface.  It has the methods of the class Surface from Adaptor3d.
        # It is created or initialized with a Face. It takes into account the local
        # coordinates system.  The u,v parameter range is the minmax value for the
        # restriction, unless the flag restriction is set to false.
        surf_type = surf.GetType()
        if surf_type == GeomAbs_Plane:
            print(" * Identified Plane Geometry")
            # look for the properties of the plane
            # first get the related gp_Pln
            gp_pln = surf.Plane()
            location = gp_pln.Location()  # a point of the plane
            normal = gp_pln.Axis().Direction()  # the plane normal
            # then export location and normal to the console output
            print(
                "--> Location (global coordinates)",
                location.X(),
                location.Y(),
                location.Z(),
            )
            print("--> Normal (global coordinates)", normal.X(), normal.Y(), normal.Z())
        elif surf_type == GeomAbs_Cylinder:
            print(" * Identified Cylinder Geometry")
            # look for the properties of the cylinder
            # first get the related gp_Cyl
            gp_cyl = surf.Cylinder()
            location = gp_cyl.Location()  # a point of the axis
            axis = gp_cyl.Axis().Direction()  # the cylinder axis
            # then export location and normal to the console output
            print(
                "--> Location (global coordinates)",
                location.X(),
                location.Y(),
                location.Z(),
            )
            print("--> Axis (global coordinates)", axis.X(), axis.Y(), axis.Z())
        elif surf_type == GeomAbs_Cone:
            print(" * Identified GeomAbs_Cone Geometry")
            print(" -- this is currently incomplete, please improve this code!")
        elif surf_type == GeomAbs_Sphere:
            print(" * Identified GeomAbs_Sphere Geometry")
            print(" -- this is currently incomplete, please improve this code!")
        elif surf_type == GeomAbs_Torus:
            print(" * Identified GeomAbs_Torus Geometry")
            print(" -- this is currently incomplete, please improve this code!")
        elif surf_type == GeomAbs_BezierSurface:
            print(" * Identified GeomAbs_BezierSurface Geometry")
            print(" -- this is currently incomplete, please improve this code!")
        elif surf_type == GeomAbs_BSplineSurface:
            print(" * Identified BSplineSurface Geometry")
            # gp_bsrf = surf.Surface()
            # degree = gp_bsrf.NbUKnots()
            # TODO use a model that provided BSplineSurfaces, as1_pe_203.stp only contains
            # planes and cylinders
        elif surf_type == GeomAbs_SurfaceOfRevolution:
            print(" * Identified GeomAbs_SurfaceOfRevolution Geometry")
            print(" -- this is currently incomplete, please improve this code!")
        elif surf_type == GeomAbs_SurfaceOfExtrusion:
            print(" * Identified GeomAbs_SurfaceOfExtrusion Geometry")
            print(" -- this is currently incomplete, please improve this code!")
        elif surf_type == GeomAbs_OffsetSurface:
            print(" * Identified GeomAbs_OffsetSurface Geometry")
            print(" -- this is currently incomplete, please improve this code!")
        elif surf_type == GeomAbs_OtherSurface:
            print(" * Identified GeomAbs_OtherSurface Geometry")
            print(" -- this is currently incomplete, please improve this code!")
        else:
            # TODO there are plenty other type that can be checked
            # see documentation for the BRepAdaptor class
            # https://www.opencascade.com/doc/occt-6.9.1/refman/html/class_b_rep_adaptor___surface.html
            print(surf_type, "recognition not implemented")
            raise NameError('All surface types should already be handled - but something is wrong - please check/fix!')
    
    
    def recognize_clicked(self, recognizeObject, *kwargs):
        """This is the function called every time
        a face is clicked in the 3d view
        """
        # These require more arguments (of type "from_xxx") so cannot be used in printShapeInfo():
        t = TopologyExplorer(self.shp) # See: /usr/lib/python3.10/site-packages/OCC/Extend/TopologyUtils.py
        # Below 5 are unused - because cannot select wire (only edge) and don't have "wires", not "solids":
        #print('number_of_edges_from_wire = ', t.number_of_edges_from_wire(shape))
        #print('number_of_ordered_edges_from_wire = ', t.number_of_ordered_edges_from_wire(shape))
        #print('number_of_ordered_vertices_from_wire = ', t.number_of_ordered_vertices_from_wire(shape))
        #print('number_of_faces_from_wires = ', t.number_of_faces_from_wires(shape))
        #print('number_of_faces_from_solids = ', t.number_of_faces_from_solids(shape))
    
        # NB: The try/catch-groups are mostly for "import_as_one_shape()", which crashes otherwise
        for shape in recognizeObject:  # this should be a TopoDS_Face
             if type(shape) is TopoDS_Face:
                 print("Face selected: ", shape)
                 print(' - number_of_vertices_from_face = ', t.number_of_vertices_from_face(shape))
                 print(' - number_of_edges_from_face = ', t.number_of_edges_from_face(shape))
                 print(' - number_of_wires_from_face = ', t.number_of_wires_from_face(shape))
                 try:
                    print(' - number_of_solids_from_face = ', t.number_of_solids_from_face(shape))
                 except Exception as exc:
                    print(' - EXCEPTION FOR: number_of_solids_from_face = ', exc)
                 self.recognize_face(shape)
             elif type(shape) is TopoDS_Compound:
                 print("Compound selected: ", shape)
                 print("   - Closed?", shape.Closed())
                 print("   - Convex?", shape.Convex())
                 print(" *** INCOMPLETE - POSSIBLE TO PRINT MORE RELEVANT INFO?")  
                 #import ipdb; ipdb.set_trace()
                 # THIS DOES NOT WORK:pnt = BRep_Tool.Pnt(shape) # shape -> gp_Pnt
                 #print("   - (X, Y, Z) = (", pnt.X(), pnt.Y(), pnt.Z(), ")")
             elif type(shape) is TopoDS_Vertex:
                 print("Vertex selected: ", shape)
                 print(' - number_of_faces_from_vertex = ', t.number_of_faces_from_vertex(shape))
                 print(' - number_of_edges_from_vertex = ', t.number_of_edges_from_vertex(shape))
                 pnt = BRep_Tool.Pnt(shape) # shape -> gp_Pnt
                 print("   - (X, Y, Z) = (", pnt.X(), pnt.Y(), pnt.Z(), ")")
             elif type(shape) is TopoDS_Edge:
                 print("Edge selected: ", shape)
                 try:
                     print(' - number_of_faces_from_edge = ', t.number_of_faces_from_edge(shape))
                 except Exception as exc:
                     print(' - EXCEPTION FOR: number_of_faces_from_edge = ', exc)
                 try:
                     print(' - number_of_faces_from_edge = ', t.number_of_faces_from_edge(shape))
                 except Exception as exc:
                     print(' - EXCEPTION FOR: number_of_faces_from_edge = ', exc)
                 print(' - number_of_wires_from_edge = ', t.number_of_wires_from_edge(shape))
                 print(' - number_of_vertices_from_edge = ', t.number_of_vertices_from_edge(shape))
                 curve = BRep_Tool.Curve(shape)
                 firstVertex = curve[0].Value(0.0)
                 lastVertex = curve[0].Value(1.0)
                 print(f" - Edge from first: {firstVertex.Coord()} to last: {lastVertex.Coord()} vertices")
    
                 # Maybe something like:
                 #print(" - Location (shape local coorsys location): ")
                 #lcs = shape.Location()
                 #orientation = shape.Orientation()
                 #uvpnts = BRep_Tool.UVPoints( shape )
                 #TopExp::FirstVertex and TopExp::LastVertex
                 #import ipdb; ipdb.set_trace()
             elif type(shape) is TopoDS_Shell:
                 nChildren = shape.NbChildren() # number of children
                 print(f"Shell selected: {shape} ; this has {nChildren} children (shape.NbChildren())")
                 #print(" - Location (shape local coorsys location): ")
                 #lcs = shape.Location()
                 #import ipdb; ipdb.set_trace()
             else:
                 print("Error: Selected shape does not work (recognize_clicked shapeloop): ", recognizeObject)
                 return
    
    def recognize_batch(self, event=None):
        """Menu item : process all the faces of a single shape"""
        print(" ====================================")
        print(" *** Recognize batch (only faces) ***")
        print(" ====================================")
        # loop over faces only
        for face in TopologyExplorer(self.shp).faces():
            # call the recognition function
            self.recognize_face(face)
    
    
    def import_as_one_shape(self, event=None):
        # grep -Pin 'def.*read' /usr/lib/python3.10/site-packages/OCC/Extend/DataExchange.py
        # def read_step_file(filename, as_compound=True, verbosity=True):
        # def read_step_file_with_names_colors(filename):
        # def read_iges_file( filename, return_as_shapes=False, verbosity=False, visible_only=False):
        # def read_stl_file(filename):
        # ---
        # NB: See read-functions source code at: /usr/lib/python3.10/site-packages/OCC/Extend/DataExchange.py 
        # ---
        print('Import as one shape:', self.fname)
        filename, fext = os.path.splitext(self.fname)
        if fext == '.step' or fext == '.stp':
            shp = read_step_file(os.path.join(self.fname))
        elif fext == '.iges' or fext == '.igs':
            shp = read_iges_file(os.path.join(self.fname))
        elif fext == '.stl':
            shp = read_stl_file(os.path.join(self.fname))
        elif fext == '.brep':
            # Inspiration from "core_parallel_slicer.py": get_brep() around line 49.
            shp = TopoDS_Shape()
            builder = BRep_Builder()
            breptools_Read(shp, self.fname, builder)
        else:
            print(f'Unknown file extension: "{ext}" - cannot continue!')
            sys.exit(1)
    
        if not self.printOnly:
            # NB: display is an instance of Viewer3d (from init_display cf. /usr/lib/python3.10/site-packages/OCC/Display/SimpleGui.py)
            self.display.EraseAll()
            self.display.DisplayShape(shp, update=True)
        self.shp = shp
    
    
    def import_as_multiple_shapes(self, event=None):
        print('Import as multiple shapes:', self.fname)
    
        #self.import_as_multiple_shapes() # TODO: THIS SEEM TO ONLY WORK WITH .STP (unlike _as_one_shape)
        compound = read_step_file(os.path.join(self.fname))
        self.display.EraseAll()
        t = TopologyExplorer(compound)
        # For debugging:
        self.shp = compound # TODO: TEMPORARY FIX - MIGHT CAUSE ISSUES
        self.printShapeInfo()
    
        # Compounds
        for compound in t.compounds():
            color = Quantity_Color(
                random.random(), random.random(), random.random(), Quantity_TOC_RGB
            )
            self.display.DisplayColoredShape(compound, color)
        # Comp_solids
        for comp_solid in t.comp_solids():
            color = Quantity_Color(
                random.random(), random.random(), random.random(), Quantity_TOC_RGB
            )
            self.display.DisplayColoredShape(comp_solid, color)
        # Solids
        for solid in t.solids():
            color = Quantity_Color(
                random.random(), random.random(), random.random(), Quantity_TOC_RGB
            )
            self.display.DisplayColoredShape(solid, color)
        # Shells:
        for shell in t.shells():
            color = Quantity_Color(
                random.random(), random.random(), random.random(), Quantity_TOC_RGB
            )
            self.display.DisplayColoredShape(shell, color)
        # Faces:
        for face in t.faces():
            color = Quantity_Color(
                random.random(), random.random(), random.random(), Quantity_TOC_RGB
            )
            self.display.DisplayColoredShape(face, color)
        # Edges:
        for edge in t.edges():
            color = Quantity_Color(
                random.random(), random.random(), random.random(), Quantity_TOC_RGB
            )
            self.display.DisplayColoredShape(edge, color)
        # Wires:
        for wire in t.wires():
            color = Quantity_Color(
                random.random(), random.random(), random.random(), Quantity_TOC_RGB
            )
            self.display.DisplayColoredShape(wire, color)
        # Vertices:
        for vertex in t.vertices():
            color = Quantity_Color(
                random.random(), random.random(), random.random(), Quantity_TOC_RGB
            )
            self.display.DisplayColoredShape(vertex, color)
        #import ipdb; ipdb.set_trace() 
        self.display.FitAll()
        self.shp = compound
    
    
    def exit(self, event=None):
        sys.exit()
    
    def fitall(self, event=None):
        self.display.FitAll()
    def viewtop(self, event=None):
        self.display.View_Top()
    def viewbottom(self, event=None):
        self.display.View_Bottom()
    def viewleft(self, event=None):
        self.display.View_Left()
    def viewright(self, event=None):
        self.display.View_Right()
    def viewfront(self, event=None):
        self.display.View_Front()
    def viewrear(self, event=None):
        self.display.View_Rear()
    def viewiso(self, event=None):
        self.display.View_Iso()
    
    def selectFace(self, event=None):
        self.display.SetSelectionModeFace() # Face selection 
    def selectVertex(self, event=None):
        self.display.SetSelectionModeVertex() # Vertex selection 
    def selectEdge(self, event=None):
        self.display.SetSelectionModeEdge() # Edge selection 
    def selectShape(self, event=None):
        self.display.SetSelectionModeShape() # Shape selection 
    def selectNeutral(self, event=None):
        self.display.SetSelectionModeNeutral() # Neutral (default) selection 

    def run(self):
        # Add menus:
        self.add_menu("CAD import")
        self.add_function_to_menu("CAD import", self.import_as_one_shape)
        self.add_function_to_menu("CAD import", self.import_as_multiple_shapes)
    
        # From: Enable click on face for details as "core_geometry_recognize_feature.py"
        self.add_menu("CAD utilities")
        self.add_function_to_menu("CAD utilities", self.recognize_batch)
        self.add_function_to_menu("CAD utilities", self.printBBInfo)
    
        # View-menu:
        self.add_menu("CAD views")
        self.add_function_to_menu("CAD views", self.fitall)
        self.add_function_to_menu("CAD views", self.viewtop)
        self.add_function_to_menu("CAD views", self.viewbottom)
        self.add_function_to_menu("CAD views", self.viewleft)
        self.add_function_to_menu("CAD views", self.viewright)
        self.add_function_to_menu("CAD views", self.viewfront)
        self.add_function_to_menu("CAD views", self.viewrear)
        self.add_function_to_menu("CAD views", self.viewiso)
        #--- maybe add these to view-menu too: ---
        #self.display.SetModeWireFrame(self):
        #self.display.SetModeShaded(self):
        #self.display.SetModeHLR(self):
        #self.display.SetOrthographicProjection(self):
        #self.display.SetPerspectiveProjection(self):
    
        # Selection Mode-menu:
        self.add_menu("Selection mode")
        self.add_function_to_menu("Selection mode", self.selectFace)
        self.add_function_to_menu("Selection mode", self.selectVertex)
        self.add_function_to_menu("Selection mode", self.selectEdge)
        self.add_function_to_menu("Selection mode", self.selectShape)
        self.add_function_to_menu("Selection mode", self.selectNeutral)
    
        # Mouse/GUI selection mode: See /usr/lib/python3.10/site-packages/OCC/Display/OCCViewer.py
        self.display.register_select_callback(self.recognize_clicked)
        # WARNING/TODO: Avoid these (from keyPressEvent in site-packages/OCC/Display/qtDisplay.py):
        #INFO:OCC.Display.qtDisplay:key: code 16777299 not mapped to any function
        #INFO:OCC.Display.qtDisplay:key: code 16777248 not mapped to any function
    
        # Default:
        #self.import_as_one_shape() # same color - nicer
        self.import_as_multiple_shapes() # several random colors - a bit better for printing TopologyExplorer-stuff - but uglier
    
        # For debugging:
        self.printShapeInfo() 
        #printBBInfo() # use_mesh=True returns approximate values
        self.printBBInfo(use_mesh=False) # use_mesh=False seems more accurate!
    
        # Switch to Face selection mode (from: site-packages/OCC/Display/OCCViewer.py)
        self.display.SetSelectionModeNeutral() # Neutral (default) selection 
    
        print ("Keyboard-shortcuts: See site-packages/OCC/Display/qtDisplay.py (InitDriver)")
        print (" --- do not press shift for these keymapping ---")
        print ("   S: display.SetModeShaded (SOLID)")
        print ("   W: display.SetModeWireFrame (WIREFRAME)")
        print ("   H: display.SetModeHLR (HLR=Hidden Line Removal mode)")
        print (" ")
        print ("   A: display.EnableAntiAliasing")
        print ("   B: display.DisableAntiAliasing")
        print ("   F: display.FitAll")
        print ("   G: display.SetSelectionMode (PRESS REPEATEDLY TO CYCLE MODES UNTIL E.G. VERTICES CAN BE SELECTED!")
        print (" ")
    
        # Show origin with text - TODO: Font size should be a ratio of bounding box!:
        pnt = gp.gp_Pnt(0.0,0.0,0.0)
        self.display.DisplayMessage(pnt, "ORIGIN")
        # It does not work, showing AIS_Trihedron:
        #placement = Geom_Axis2Placement(OCC.Core.gp.gp_XOY())
        #at = AIS_Trihedron(placement)
        #myAISContext = AIS.AIS_InteractiveContext(self.display)
    
        print(" === NB: This is incomplete, consider completing it via e.g. core_shape_properties.py + core_display_qt5_app.py ===")
        # Start self.display and event-loop:
        self.start_display()

def parseArgs():
    parser = argparse.ArgumentParser(description="Run cadfile debug tool",
                                     formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    required = parser.add_argument_group('required arguments')
    optional = parser._action_groups.pop()
    # Required args
    required.add_argument(dest='filename', help="CAD file-name to open")
    # Optional args
    optional.add_argument('-p', '--print', dest='print_only', default=False, action='store_true',
                        help="Only print data to screen, without starting the GUI")
    optional.add_argument('-v', '--verbose', dest='verbose', default=False, action='store_true',
                        help="verbose output")
    parser._action_groups.append(optional)
    # If no argument besides the filename has been given; print help and exit.
    if len(sys.argv) <= 1:
        parser.print_help()
        sys.exit(0)
    return parser.parse_args()
   
if __name__ == "__main__":
    args = parseArgs()
    cfd = cadfileDebugger(args)
    cfd.run()
