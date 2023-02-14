import sys
import time
import signal
import threading
import numpy as np
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QWidget, QLabel, QApplication, QVBoxLayout, QGridLayout
from PyQt5.QtCore import *
from PyQt5.QtGui import *
import pyqtgraph as pg
sys.path.append("..")
from Modules import Module, Analyzer


class ParameterPlotter(Analyzer):
    def __init__(self, id=None, autoMove=True, standalone=False, stepMode=False):
        super().__init__(id)
        
        self.parameterInputs.append({"name": "input", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "input", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "input", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "input", "module": None, "sourceIndex": 0, "default": 0.0})
        
        self.autoMove = autoMove
        self.standalone = standalone
        self.stepMode = stepMode
        self.standaloneT = 0
        self.updateFunction = None
        
        self.maxSampleTime = 60 * 5          # [s]
        self.maxTimeWidth = 5                # [s]
        self.manualControlTime = 5           # [s]
        self.x = []
        self.y = [[], [], [], []]
        self.yMin = -1.0
        self.yMax = 1.0
        self.running = False
        self.ready = False
        self.legendAdded = [False] * 4
        
        if self.standalone:
            def signal_handler(sig, frame):
                self.running = False
                self.widget.graphWidget.close()
            
            signal.signal(signal.SIGINT, signal_handler)
            threading.Thread(target=self.run, args=(self, )).start()
            while not self.running:
                time.sleep(0.1)
         
        else:
            self.widget = MainWindow(stepMode=self.stepMode)
            self.app = QtWidgets.QApplication(sys.argv)
            threading.Thread(target=self.app.exec_).start()
            self.widget.show()
        

    def stop(self):
        self.running = False
        self.widget.close()
        
        
    def run(self, ref):
        ref.app = QtWidgets.QApplication(sys.argv)
        ref.widget = MainWindow(stepMode=self.stepMode)
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
            while not any(self.parameterInputs):
                print("waiting")
                time.sleep(0.1)
        if not super().update(t):
            return False
        
        if any([i["module"] for i in self.parameterInputs]):    # Check if any input is connected to module
            self.widget.setWindowTitle(f"Parameter Plotter [{self.id}]")
        else:
            self.widget.setWindowTitle(f"Parameter Plotter [{self.id}]: Unconnected")
            return True
        
        for i in range(len(self.widget.data_line)):
            if not self.parameterInputs[i]["module"]:
                continue
            
            if not self.legendAdded[i]:
                self.legendAdded[i] = True
                module = self.parameterInputs[i]
                moduleName = f"{module['module'].superClassType}.{module['module'].__module__}"
                self.widget.legend.plotItem.legend.addItem(self.widget.legendItem[i], f"{moduleName} [{module['module'].id}] (Output: {module['sourceIndex']})")
            
            output = self._getParameterValue(i)
            self.yMin = min(self.yMin, output)
            self.yMax = max(self.yMax, output)
            if t not in self.x:
                self.x.append(t)
            self.y[i].append(output)
            
            maxSamples = int(self.maxSampleTime * Module.framerate)
            if(len(self.y[i]) > maxSamples):
                self.x = self.x[-maxSamples:]
            self.y[i] = self.y[i][-len(self.x):]
            
            self.widget.plotItem.setYRange(self.yMin * 1.2, self.yMax * 1.2, padding=0) 
            if(time.time() - self.widget.graphWidget.mouseEventTime > self.manualControlTime) and self.autoMove:
                self.widget.plotItem.setXRange(max(0, t - self.maxTimeWidth), t, padding=0)
    
            if(len(self.x) > 2):
                self.widget.data_line[i].setData(self.x, self.y[i])
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
        self.isClicked = False

    def wheelEvent(self,event):
        super().wheelEvent(event)
        self.mouseEventTime = time.time()
    
    def mouseReleaseEvent(self, event):
        super().mouseReleaseEvent(event)
        self.mouseEventTime = time.time()
        self.isClicked = False

    def mousePressEvent(self, event):
        super().mousePressEvent(event)
        self.mouseEventTime = time.time()
        self.isClicked = True
        
    def mouseMoveEvent(self, event):
        super().mouseMoveEvent(event)
        if self.isClicked:
            self.mouseEventTime = time.time()

        
class CustomLegend(pg.PlotWidget):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        
    def wheelEvent(self,event): pass
    def mouseReleaseEvent(self, event): pass
    def mousePressEvent(self, event): pass
    def mouseMoveEvent(self, event): pass


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self, *args, **kwargs):
        self.plotMode = "right" if kwargs.pop("stepMode", None) else None
        super(MainWindow, self).__init__(*args, **kwargs)
        self.widget = QWidget()
        self.widget.setStyleSheet('background-color: #212124')
        
        
        self.graphWidget = MainPlotWidget()
        self.setCentralWidget(self.graphWidget)
        self.setGeometry(300, 300, 700, 500)
        self.graphWidget.setBackground('#212124')        
        self.plotItem = self.graphWidget.getPlotItem()
        self.plotItem.showGrid(x=True, y=True, alpha=0.3)
        self.plotItem.getAxis('bottom').setGrid(False)             # Disable bottom axis grid, prevent bus as noted here: https://stackoverflow.com/questions/69816567/pyqtgraph-cuts-off-tick-labels-if-showgrid-is-called
        self.plotItem.showAxis('top')                              # Show top/right axis (and grid, since enabled here)
        self.plotItem.getAxis('top').setStyle(showValues=False)    # Hide tick labels on top/right
        self.plotItem.setMouseEnabled(x=True, y=False)
        self.plotItem.hideButtons()
       
        self.pen = [pg.mkPen(color=(0, 142, 211), width=3),
                    pg.mkPen(color=(211, 105, 0), width=3),
                    pg.mkPen(color=(211, 0, 172), width=3),
                    pg.mkPen(color=(0, 211, 18), width=3)]
        
        
        self.data_line = [self.graphWidget.plot([0, 0], [0, 0], pen=self.pen[0], stepMode=self.plotMode),
                          self.graphWidget.plot([0, 0], [0, 0], pen=self.pen[1], stepMode=self.plotMode),
                          self.graphWidget.plot([0, 0], [0, 0], pen=self.pen[2], stepMode=self.plotMode),
                          self.graphWidget.plot([0, 0], [0, 0], pen=self.pen[3], stepMode=self.plotMode)]
        
        self.legendItem = [pg.LegendItem(pen=self.pen[0]),
                           pg.LegendItem(pen=self.pen[1]),
                           pg.LegendItem(pen=self.pen[2]),
                           pg.LegendItem(pen=self.pen[3])]
        
        self.legend = CustomLegend()
        self.legend.setBackground('#212124')    
        self.legend.setMouseEnabled(x=False, y=False)
        self.legend.hideButtons()
        self.legend.setMaximumHeight(45)
        self.legend.hideAxis('left')
        self.legend.hideAxis('bottom')
        self.legend.addLegend()
        self.legend.plotItem.legend.setColumnCount(2)
        self.legend.plotItem.legend.anchor((-0.05, 0.15), (0.0, 0.0))
        
        
        self.layout = QVBoxLayout()
        self.widget.setLayout(self.layout)
        self.layout.addWidget(self.graphWidget)
        self.layout.addWidget(self.legend)
        self.setCentralWidget(self.widget)
        
        
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
    sine.setParameterInput(0, Coefficient(1000, enable))
    sine.setParameterInput(1, Coefficient(1001, freq))
    sine.setParameterInput(2, Coefficient(1002, rep))
    sine.setParameterInput(3, Coefficient(1003, amp))
    sine.setParameterInput(4, Coefficient(1004, offset))
    sine.setParameterInput(5, Coefficient(1005, phase))
    
    ramp = Generator.Ramp(1)
    rect = Generator.Rect(2)
    triangle = Generator.Triangle(3)
    
    plotter = Analyzer.ParameterPlotter(5000, standalone=True, stepMode=False)
    plotter.setParameterInput(0, sine, 0)
    plotter.setParameterInput(1, ramp, 0)
    plotter.setParameterInput(2, rect, 0)
    plotter.setParameterInput(3, triangle, 0)
    
    def update(t):
        sine.update(t)
        ramp.update(t)
        rect.update(t)
        triangle.update(t)
        plotter.update(t)

    plotter.updateFunction = update
    while not plotter.isReady(): pass    # Wait until plotter is ready
    while plotter.isRunning():
        time.sleep(0.1)
