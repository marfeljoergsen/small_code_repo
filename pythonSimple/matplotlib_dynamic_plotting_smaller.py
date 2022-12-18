#!/usr/bin/env python
# From: https://pythonspot.com/matplotlib-update-plot/

import matplotlib.pyplot as plt
import numpy as np
import time

# Create data:
x = np.linspace(0, 10*np.pi, 100)
y = np.sin(x)

plt.ion() # turn on interactive mode

# Configure the plot:
fig = plt.figure()
ax = fig.add_subplot(111)
line1, = ax.plot(x, y, 'b-') # blue line with 'b-'

# Update plot in loop + delay to ensure screen updates
for phase in np.linspace(0, 10*np.pi, 100):
  line1.set_ydata(np.sin(0.5 * x + phase))
  fig.canvas.draw()
  plt.pause(1e-17)
  time.sleep(0.1)

plt.show()

