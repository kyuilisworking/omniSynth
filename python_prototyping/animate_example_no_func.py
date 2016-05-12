import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

fig, ax = plt.subplots()
line, = ax.plot(np.arange(10))
ax.set_ylim(0, 1)

plt.show()

while (True):
    line.set_ydata(np.random.rand(10))