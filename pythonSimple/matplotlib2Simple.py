#!/usr/bin/python2

import matplotlib

#If nothing is shown with the TKagg-line commented-out, add
#"backend: TkAgg" to the file: ~/.config/matplotlib/matplotlibrc
#and rerun and test that things are working correctly:
#matplotlib.use("TKagg")

import matplotlib.pyplot as plt
plt.plot([1,2,3,4])
plt.ylabel('some numbers')
plt.show()

