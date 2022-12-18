#!/usr/bin/python2

import matplotlib
print("Default mpl_configdir (place your own matplotlibrc here): " + matplotlib.get_configdir())
print("  - example (you might need to create path/file):         " + matplotlib.get_configdir() + "/matplotlibrc")
print("Currently used matplotlibrc file:                         " + matplotlib.matplotlib_fname())
print("---")

print("All possible backends (not implying they are available on this system):")
#print(matplotlib.rcsetup.interactive_bk)
#print(matplotlib.rcsetup.non_interactive_bk)
print(matplotlib.rcsetup.all_backends)
print("---")

# plt.get_backend()
res = matplotlib.get_backend() #'Qt5Agg'
print("Currently using backend: " + res)
# You can explicitly instruct matplotlib to use the TkAgg backend by making a call to matplotlib.use("TkAgg")
