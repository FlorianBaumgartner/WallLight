import sys
import time
import signal
import threading
import numpy as np
from PyQt5 import QtWidgets
from PyQt5.QtCore import QTimer
import pyqtgraph as pg
sys.path.append("..")
from Modules import Module, Analyzer


class ParameterPlotter(Analyzer):
    def __init__(self, id, autoMove=True, standalone=False):
        super().__init__(id)
        
        self.parameterInputs.append({"name": "input", "module": None, "sourceIndex" : 0})
        
        self.autoMove = autoMove
        self.standalone = standalone
        self.standaloneT = 0
        self.updateFunction = None
        
        self.maxSampleTime = 60 * 5          # [s]
        self.maxTimeWidth = 5                # [s]
        self.manualControlTime = 5           # [s]
        self.x = []
        self.y = []
        self.yMax = 1.0;
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
            while not self.parameterInputs[0]:
                print("waiting")
                time.sleep(0.1)
        if not super().update(t):
            return False
        
        module = self.parameterInputs[0]
        moduleName = f"{module['module'].superClassType}.{module['module'].__module__}"
        self.widget.setWindowTitle(f"Parameter Plotter [{self.id}]: {moduleName} (ID: {module['module'].id}, Output: {module['sourceIndex']})")
        
        output = self.parameterInputs[0]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"]
        self.yMax = max(self.yMax, output)
        if t in self.x:
            self.y[self.x.index(t)] = output
            print("Same x value received -> overwrite y")
        else:
            self.x.append(t)
            self.y.append(output)
        
        maxSamples = self.maxSampleTime * Module.framerate
        if(len(self.x) > maxSamples):
            self.x = self.x[-maxSamples:]
            self.y = self.y[-maxSamples:]
        
        self.widget.plotItem.setYRange(-self.yMax * 1.2, self.yMax * 1.2, padding=0) 
        if(time.time() - self.widget.graphWidget.mouseEventTime > self.manualControlTime) and self.autoMove:
            self.widget.plotItem.setXRange(max(0, t - self.maxTimeWidth), t, padding=0)

        if(len(self.x) > 2):
            self.widget.data_line.setData(self.x, self.y)
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
    


class MainPlotWidget(pg.PlotWidget):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.mouseEventTime = 0

    def wheelEvent(self,event):
        super().wheelEvent(event)
        self.mouseEventTime = time.time()
    
    def mouseReleaseEvent(self, event):
        super().mouseReleaseEvent(event)
        self.mouseEventTime = time.time()

    def mousePressEvent(self, event):
        super().mousePressEvent(event)
        self.mouseEventTime = time.time()


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)
        self.graphWidget = MainPlotWidget()
        self.setCentralWidget(self.graphWidget)
        self.setGeometry(300, 300, 700, 500)
        self.graphWidget.setBackground('#212124')        
        self.plotItem = self.graphWidget.getPlotItem()

        self.plotItem.showGrid(x=True, y=True, alpha=0.3)
        self.plotItem.setMouseEnabled(x=True, y=False)
        self.plotItem.hideButtons()
       
        self.pen = pg.mkPen(color=(0, 142, 211), width=3)
        self.data_line = self.graphWidget.plot([0, 0], [0, 0], pen=self.pen)
        
    def closeEvent(self, event):
        event.accept()

if __name__ == '__main__':
    from Modules import Coefficient, Generator, Analyzer
    Module.framerate = 60
    
    enable = 1.0
    freq = 1
    rep = 1
    amp = 0.5
    offset = 0.5
    phase = 0
    
    sine = Generator.Sine(0)
    sine.setParameterInput(0, Coefficient(2, enable))
    sine.setParameterInput(1, Coefficient(3, freq))
    sine.setParameterInput(2, Coefficient(4, rep))
    sine.setParameterInput(3, Coefficient(5, amp))
    sine.setParameterInput(4, Coefficient(6, offset))
    sine.setParameterInput(5, Coefficient(7, phase))
    
    plotter = Analyzer.ParameterPlotter(1, standalone=True)
    plotter.setParameterInput(0, sine, 0)
    
    def update(t):
        sine.update(t)
        plotter.update(t)

    plotter.updateFunction = update
    while not plotter.isReady(): pass    # Wait until plotter is ready
    while plotter.isRunning():
        time.sleep(0.1)
