import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

class Visualize_Data:

    def __init__(self, data_size, ylim, interval=1):
        '''
        Constructor
        Arguments:
            data_size - size of the data
            ylim - tuple of y lower limit, upper limit 
        '''
        self.data_size = data_size
        self.fig, self.ax = plt.subplots()
        self.ax.set_ylim(ylim[0], ylim[1])
        self.interval = 1
        self.line, = self.ax.plot(np.arange(data_size))
        self.current_data = None

    def update_data(self, data):
        '''
        Updates the data stored for the class
        Arguments: 
            data - the new data
        '''
        self.current_data = data

    def update_ani(self, data):
        self.line.set_ydata(data)
        return self.line

    def data_gen(self):
        while True: yield self.current_data

    def visualize(self):
        ani = animation.FuncAnimation(self.fig, self.update_ani, self.data_gen, interval=1)
        plt.show()
