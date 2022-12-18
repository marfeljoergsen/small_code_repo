#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Sep  5 17:35:27 2019
@author: mfj

NB: In "spyder" use "%reset" to clear all variables -
Alternatively, choose the Run-menu -> Configuration per file... ->
   Clear all variables before execution [Select Checkbox] 
"""

import numpy as np
import matplotlib.pyplot as plt
import io

def returnPntsList(fname,dim): # dim=2 for 2D, dim=3 for 3D data
    with open(fname, 'r') as f:
        data = f.read().split() # this is the whole file, splitted...?
        pnts = [] # initialize list
        for elem in data:
            try:
                pnts.append(float(elem)) # convert to float
            except ValueError:
                pass
    raise Exception('This is incomplete - and untested...')
    # Isn't this incorrect [1::dim] slices so e.g. dim=2 is every 2nd record that is skipped?
    x=pnts[0::dim]; y=pnts[1::dim]; # extract vectors from columns
    return [x,y];


def returnPnts(fname,dim,FixedColumns): # dim=2 for 2D, dim=3 for 3D data
    #FixedColumns = False
    #FixedColumns = True
    print('FixedColumns=',FixedColumns)
    # FixedColumns = True: Data is columnwise, i.e.
    #     col.1=X, col.2=Y, col.3=Z...
    # FixedColumns = False: Data is ordered sequentially 
    #     (x1,y1,x2,y2,x3,y3... or x1,y1,z1, x2,y2,z2)...
    l=1
    for line in open(fname):
        row = line.split() # row is a list of strings
        if FixedColumns:
            row = row[:dim] # only use first "dim" columns
        listString = np.array(row)
        arrFloat = listString.astype(np.float)
        if l==1:
            pnts = arrFloat
        else:
            if (len(row)==0):
                print('Empty row at line: ' + str(l))
            else:
                if FixedColumns:
                    try:
                        pnts=np.vstack((pnts,arrFloat))
                    except ValueError:
                        print('Invalid line: '+ l +'contains: ' + row)
                        pass
                else:
                    # creates a new array instead of mutating:
                    pnts=np.append(pnts,arrFloat)

        l=l+1        
    print('Finishing reading:',l-1,'number of lines...')
    if FixedColumns:
        if dim==2: # extract vectors from columns
            x=pnts[:,0]; y=pnts[:,1];
            return [x,y];
        elif dim==3:
            x=pnts[:,0]; y=pnts[:,1], z=pnts[:,2];
            return [x,y,z];
    else:
        if dim==2: # extract vectors from columns, slicing: [start:stop:step]
            x=pnts[0::2]; y=pnts[1::2];
            return [x,y];
        elif dim==3:
            x=pnts[0::3]; y=pnts[1::3], z=pnts[2::3];
            return [x,y,z];


def plotFun2D(ax,x,y,title):
#    lineX = x[4:]
#    lineX.append(x[4])
#    lineY = y[4:]
#    lineY.append(y[4])
#    plt.plot(lineX, lineY,'-')
#    plt.plot(x[0:4],y[0:4],'x')
    plt.plot(x,y)
    for lin in np.arange(0,np.size(x)):
        #print(lin+1)
        plt.text(x[lin], y[lin], str(lin+1), fontsize=12)
    plt.grid(True)
    #plt.axis('scaled')    
    plt.gca().set_aspect('equal', adjustable='box')
    plt.title(title)
    
def plotFun3D(ax,x,y,z,title):
    raise Exception('This is incomplete - and untested...')
    plt.plot(x,y,z)
    plt.grid(True)
    plt.axis('scaled')
    plt.title(title)
    
# ==== Maybe delete these? ====
def getXYFileName(lineNumStr,dim): # dim=2 for 2D, dim=3 for 3D data
    filePrefix='/tmp/l'
    fname=filePrefix + lineNumStr + '.txt';
    [x,y] = returnPnts(fname,2,FixedColumns)
    return [x,y,fname];
    
def plotLineNum(lineNumStr):
    [x,y,fname] = getXYFileName(lineNumStr);
    plotFun2D(ax,x,y,fname)
# ====

def plotFile(ax,fname,dim,FixedColumns): # dim=2 for 2D, dim=3 for 3D data
    if dim==2:
        [x,y] = returnPnts(fname,2,FixedColumns)
        plotFun2D(ax,x,y,fname)
    elif dim==3:
        [x,y,z] = returnPnts(fname,3,FixedColumns)
        plotFun3D(ax,x,y,z,fname)
    else:
        raise Exception('Need to use dim=2 or dim=3 (for 2D/3D plots)')

def plotListOfFiles2D(ax,plotFiles,FixedColumns):    
    for f in plotFiles:
        print('Plotting: ' + f)
        plotFile(ax,f,2,FixedColumns)
        plt.legend(plotFiles)
        plt.show()
        #input('--> Press a key to continue...') # not needed in terminal window
    print('plotListOfFiles2D finished...')

def plotListOfFiles3D(ax,plotFiles):    
    for f in plotFiles:
        print('Plotting: ' + f)
        plotFile(ax,f,3)
        plt.legend(plotFiles)
        plt.show()
        #input('--> Press a key to continue...') # not needed in terminal window
    print('plotListOfFiles3D finished...')
# -----
def getXYZpntsFromInputFile(fname):
    with open(fname, 'r') as in_file:
        content = in_file.read()
    
    allVerts = [];
    s = io.StringIO(content)
    lnum=1
    dataLine=0
    dataLineLimit=4
    for line in s:
        if (dataLine>=1 and dataLine<=dataLineLimit):
            if (dataLine==1):
                vert4 = []; # clear
                #print(line)
            data = line.split()       
            pnts = [] # initialize list
            for elem in data:
                try:
                    pnts.append(float(elem)) # convert to float
                except ValueError:
                    pass
            vert4.append( pnts );
            dataLine = dataLine+1
            #print(pnts)
            #print(vert4)
            
        if (dataLine>dataLineLimit):
            allVerts.append(vert4)
            dataLine=0
            
        if (line.startswith(' (subroutine make_rect_loop) vert4')):
            print('Found at line: ' + str(lnum))
            dataLine=1
        lnum=lnum+1
    return allVerts

def plotXYfromXYZpoints(ax,vert4pnts, *args):
    numQuads=len(vert4pnts)
    if len(args)==1:
        pltPnt=args[0] # optional
    else:
        pltPnt=-1
    for p in list(range(numQuads)):
        print(p)
        if (pltPnt>=0) and (p != pltPnt):
            continue
        currPnts = vert4pnts[p]
        mat3d = np.array(currPnts)
        matXY = mat3d[:,0:2] # all rows, but only columns 0 and 1
        matXY = np.vstack([mat3d[:,0:2], mat3d[0,0:2]]) # append first row, columns 0+1
        plotFun2D(ax,matXY[:,0], matXY[:,1], 'p='+str(p+1))
        input('--> Press a key to continue (or CTRL+C to break)...')
    plt.legend(1+np.arange(numQuads))

fig = plt.figure() # <module 'matplotlib.pyplot' from '/usr/lib/python3.7/site-packages/matplotlib/pyplot.py'>
ax = fig.add_subplot(111) # <matplotlib.axes._subplots.AxesSubplot at 0x7f814ff295d0>

# TO SPLIT A FILE FOR EVERY E.G. 25 LINES USE: "split -l 25 quad25NoSpaces.txt"
# Verify with: "cat xaa | wc -l; cat xab | wc -l; cat xac | wc -l; cat xad | wc -l; cat xae | wc -l; cat xaf | wc -l"
# To provide the file names for concatenation: "readlink -f xa{a,b,c,d,e,f} | tr '\n' ' '"
#readFiles='/tmp/quad25/xaa /tmp/quad25/xab /tmp/quad25/xac /tmp/quad25/xad /tmp/quad25/xae /tmp/quad25/xaf'
#readFiles='/tmp/ellipse_currently_3quarterCircles1of3.txt /tmp/ellipse_currently_3quarterCircles2of3.txt /tmp/ellipse_currently_3quarterCircles3of3.txt'
#readFiles='/tmp/vert/vert1.txt'
readFiles='/tmp/a/1.txt'
fileList=readFiles.split()

plotListOfFiles2D(ax,fileList,FixedColumns=True)

print('All done')
