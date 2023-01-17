import sys
import time
import signal
import threading
import numpy as np
from PyQt5 import QtWidgets
from PyQt5.QtCore import QTimer
from pyqtgraph.Qt import QtGui, QtCore
import pyqtgraph as pg
sys.path.append("..")
from Modules import Module, Analyzer


class InputPlotter(Analyzer):  
    def __init__(self, id, standalone=False):
        super().__init__(id)
        
        self.parameterInputs.append({"name": "channel", "module": None, "sourceIndex": 0, "default": 0.0})
        self.inputs.append({"name": "input", "module": None, "sourceIndex": 0, "default": np.zeros((Analyzer.pixelcount, 6))})
        
        self.standalone = standalone
        self.standaloneT = 0
        self.updateFunction = None
        
        self.maxSampleTime = 60 * 5          # [s]
        self.maxTimeWidth = 5                # [s]
        self.manualControlTime = 5           # [s]
        self.x = []
        self.y = []
        self.yMin = 0.0
        self.yMax = 1.0
        self.running = False
        self.ready = False
        
        if self.standalone:
            def signal_handler(sig, frame):
                self.running = False
                self.widget.graphWidget.close()
            
            signal.signal(signal.SIGINT, signal_handler)
            threading.Thread(target=self.run, args=(self, )).start()
            while not self.running:
                time.sleep(0.1)
         
        else:
            self.widget = MainWindow()
            self.app = QtWidgets.QApplication(sys.argv)
            threading.Thread(target=self.app.exec_).start()
            self.widget.show()
        

    def end(self):
        self.running = False
        self.widget.close()
        
        
    def run(self, ref):
        ref.app = QtWidgets.QApplication(sys.argv)
        ref.widget = MainWindow()
        ref.widget.show()
        
        self.running = True
        setattr(ref, "app", ref.app)
        setattr(ref, "widget", ref.widget)
        
        if ref.standalone:
            ref.timer = QTimer()
            ref.timer.timeout.connect(ref.standaloneUpdate)
            ref.timer.start(int(1000 / Module.framerate))
        
        
        while ref.widget.graphWidget.isVisible():
            ref.app.processEvents()
        ref.widget.close()
        ref.app.quit()

            
    def isRunning(self):
        return self.widget.graphWidget.isVisible()
    
    def isReady(self):
        return self.ready
        
    def update(self, t):
        if(Module.framerate == 0):
            return False
        self.ready = True
        if self.standalone:
            while not self.inputs[0] or not self.parameterInputs[0]:
                print("waiting")
                time.sleep(0.1)
        if not super().update(t):
            return False
        
        channel = int(self._getParameterValue(0))
        output = self._getInput(0)[:,channel]
        self.widget.updateValues(output)
        
        if self.inputs[0]["module"]:
            module = self.inputs[0]
            moduleName = f"{module['module'].superClassType}.{module['module'].__module__}"
            self.widget.setWindowTitle(f"Input Plotter [{self.id}, Channel: {channel}]: {moduleName} (ID: {module['module'].id}, Output: {module['sourceIndex']})")
        else:
            self.widget.setWindowTitle(f"Input Plotter [{self.id}, Channel: {channel}]: Unconnected")
        
        self.yMin = min(self.yMin, np.min(output))
        self.yMax = max(self.yMax, np.max(output))
        self.widget.plotItem.setYRange(-self.yMin * 1.1 - 0.1, self.yMax * 1.1, padding=0)
        return True
    
    def standaloneUpdate(self):
        if not self.running:
            return
        self.standaloneT += 1 / Module.framerate
        if not self.isReady():      # Start time increment only when analyzer is ready
            self.standaloneT = 0
        if self.updateFunction:
            try:
                self.updateFunction(self.standaloneT)
            except Exception as e:
                self.end()
                print(e)
    


class LinePlot(pg.GraphicsObject):
    def __init__(self, color, width):
        super(LinePlot, self).__init__()
        self.picture = QtGui.QPicture()
        self.color = color
        self.width = width

    def setData(self, x, y):
        self.x = x
        self.y = y
        self.generate()

    def generate(self):
        p = QtGui.QPainter(self.picture)
        p.setPen(pg.mkPen(self.color, width=self.width))
        for x in self.x:
            y = self.y[x]
            if np.abs(y) > 0.01:
                p.drawLine(QtCore.QPointF(x, 0), QtCore.QPointF(x, y))
        
    def paint(self, p, *args):
        p.drawPicture(0, 0, self.picture)

    def boundingRect(self):
        return QtCore.QRectF(self.picture.boundingRect())


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)
        self.graphWidget = pg.plot()
        self.setCentralWidget(self.graphWidget)
        self.setGeometry(300, 300, 1920, 500)
        self.graphWidget.setBackground('#212124')   
        
        self.color = (0, 142, 211)
        self.plotItem = self.graphWidget.getPlotItem()
        self.plotItem.showGrid(x=True, y=True, alpha=0.3)
        self.plotItem.setMouseEnabled(x=True, y=False)
        self.plotItem.hideButtons()
        
        self.x = np.arange(Module.pixelcount)
        self.y = np.zeros(Module.pixelcount)
        
        self.bars = LinePlot(self.color, 3)
        self.dots = pg.ScatterPlotItem(size=8, brush=pg.mkBrush(self.color), pen=pg.mkPen(None))
        self.plotItem.addItem(self.bars)
        self.plotItem.addItem(self.dots)
        
        xRange = Module.pixelcount
        self.plotItem.setXRange(-0.02 * xRange, xRange * 1.02, padding=0)
    
    def closeEvent(self, event):
        event.accept()
        
    def updateValues(self, y):
        self.y = y
        self.bars.setData(self.x, self.y)
        self.dots.setData(self.x, self.y)


if __name__ == '__main__':
    from Modules import Coefficient, Generator, Function, Analyzer
    Module.framerate = 60
    Module.pixelcount = 288

    enable = 1.0
    freq = 0.5
    rep = -1.0
    amp = 0.3
    offset = 0.5
    phase = 0
    variance = 0.01
    colorChannel = 0.0
    
    sine = Generator.Sine(0)
    sine.setParameterInput(0, Coefficient(3, enable))
    sine.setParameterInput(1, Coefficient(4, freq))
    sine.setParameterInput(2, Coefficient(5, rep))
    sine.setParameterInput(3, Coefficient(6, amp))
    sine.setParameterInput(4, Coefficient(7, offset))
    sine.setParameterInput(5, Coefficient(8, phase))
    
    pdf = Function.Pdf(1)
    pdf.setParameterInput(0, sine)
    pdf.setParameterInput(1, Coefficient(9, variance))
    
    plotter = Analyzer.InputPlotter(2, standalone=True)
    plotter.setParameterInput(0, Coefficient(10, colorChannel))
    plotter.setInput(0, pdf)
    
    def update(t):
        sine.update(t)
        pdf.update(t)
        plotter.update(t)

    plotter.updateFunction = update
    while not plotter.isReady(): pass    # Wait until plotter is ready
    while plotter.isRunning():
        time.sleep(0.1)
