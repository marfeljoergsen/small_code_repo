#!/usr/bin/env python
# From: https://block.arch.ethz.ch/blog/2016/08/dynamic-plotting-with-matplotlib/
#  - but modified for python3

import matplotlib.pyplot as plt
import time
import random

ysample = random.sample(range(-50, 50), 100)

xdata = []
ydata = []

plt.show()

axes = plt.gca()
axes.set_xlim(0, 100)
axes.set_ylim(-50, +50)
line, = axes.plot(xdata, ydata, 'r-')

for i in range(100):
  xdata.append(i)
  ydata.append(ysample[i])
  line.set_xdata(xdata)
  line.set_ydata(ydata)
  plt.draw()
  plt.pause(1e-17)
  time.sleep(0.1)

# Add this, to let the plotwindow stay, after animation finishes:
#plt.show()

