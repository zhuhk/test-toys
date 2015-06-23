
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
import numpy as np

# some data
x = np.arange(0, 10, 0.1)
y1 = np.sin(x)
y2 = np.cos(x)

# plot of the data
fig = plt.figure()
ax = fig.add_axes([0.1, 0.1, 0.6, 0.75])
ax.plot(x, y1,'-k', lw=2, label='black sin(x)')
ax.plot(x, y2,'-r', lw=2, label='red cos(x)')
ax.set_xlabel('x', size=22)
ax.set_ylabel('y', size=22)
ax.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.)
plt.savefig("ax")
