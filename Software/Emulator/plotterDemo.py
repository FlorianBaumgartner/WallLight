import numpy as np
from pyqtgraph.Qt import QtGui, QtCore
import pyqtgraph as pg

# pg.setConfigOptions(antialias=True)

class LinePlot(pg.GraphicsObject):
    def __init__(self, color, width):
        super(LinePlot, self).__init__()
        self.picture = QtGui.QPicture()
        self.color = color
        self.width = width

    def addPoints(self, x, y):
        self.x = x
        self.y = y
        self.generate()

    def generate(self):
        p = QtGui.QPainter(self.picture)
        p.setPen(pg.mkPen(self.color, width=self.width))
        for x in self.x:
            if self.y[x] != 0:
                p.drawLine(QtCore.QPointF(x, 0), QtCore.QPointF(x, self.y[x]))
        
    def paint(self, p, *args):
        p.drawPicture(0, 0, self.picture)

    def boundingRect(self):
        return QtCore.QRectF(self.picture.boundingRect())


if __name__ == '__main__':
    import sys
    import random
    
    x = np.arange(10)
    y = x * 0.1
    
    color = (0, 142, 211)

    plot = pg.plot()
    bars = LinePlot(color, 3)
    bars.addPoints(x, y)
    dots = pg.ScatterPlotItem(size=8, brush=pg.mkBrush(color), pen=pg.mkPen(None))
    dots.addPoints(x, y)
    
    plot.addItem(bars)
    plot.addItem(dots)
    plot.show()
