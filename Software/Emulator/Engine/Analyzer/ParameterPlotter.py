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
    def __init__(self, id, standalone=False):
        super().__init__(id)
        self.standalone = standalone
        self.standaloneT = 0
        self.updateFunction = None
        
        self.maxTimeWidth = 5
        self.manualControlTime = 5
        self.x = []
        self.y = []
        self.yMax = 1.0;
        self.running = False
        
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
        # print("Parameter Plotter run terminated")
            
    def isRunning(self):
        return self.widget.graphWidget.isVisible()
        
    def update(self, t):
        if(Module.framerate == 0):
            return False
        if self.standalone:
            while not self.parameterInputs[0]:
                print("waiting")
                time.sleep(0.1)
        if super().update(t) == False:
            return False
        output = self.parameterInputs[0]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"]
        self.yMax = max(self.yMax, output)
        self.x.append(t)
        self.y.append(output)        
        
        self.widget.plotItem.setYRange(-self.yMax * 1.2, self.yMax * 1.2, padding=0) 
        if(time.time() - self.widget.graphWidget.mouseEventTime > self.manualControlTime):
            self.widget.plotItem.setXRange(max(0, t - self.maxTimeWidth), t, padding=0)

        if(len(self.x) > 2):
            self.widget.data_line.setData(self.x, self.y)
        return True
    
    def standaloneUpdate(self):
        if not self.running:
            return
        self.standaloneT += 1 / Module.framerate
        if self.updateFunction:
            self.updateFunction(self.standaloneT)
    


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
        self.graphWidget.setBackground('#212124')        
        self.plotItem = self.graphWidget.getPlotItem()

        # axBottom = self.plotItem.getAxis('bottom') #get x axis
        # axBottom.setPen('w') 
        # xTicks = [0.5, 0.1]
        # axBottom.setTickSpacing(xTicks[0], xTicks[1]) #set x ticks (major and minor)
        # axLeft = self.plotItem.getAxis('left') #get y axis
        # axLeft.setPen('w') 
        # yTicks = [0.5, 0.1]
        # axLeft.setTickSpacing(yTicks[0], yTicks[1]) #set y ticks (major and minor)
        self.plotItem.showGrid(x=True, y=True, alpha=0.3)
        self.plotItem.setMouseEnabled(x=True, y=False)
        self.plotItem.hideButtons()
       
        self.pen = pg.mkPen(color=(0, 142, 211), width=3)
        self.data_line = self.graphWidget.plot([0, 0], [0, 0], pen=self.pen)
        
    def closeEvent(self, event):
        # print("ParameterPlotter close event")
        event.accept()

if __name__ == '__main__':
    import time
    from Modules import Coefficient, Generator
    Module.framerate = 60
    

    freq = 1
    rep = 1
    amp = 0.5
    offset = 0.5
    phase = -0.5
    
    sine = Generator.Sine(0)
    sine.setParameterInput(0, Coefficient(0, freq))
    sine.setParameterInput(1, Coefficient(1, rep))
    sine.setParameterInput(2, Coefficient(2, amp))
    sine.setParameterInput(3, Coefficient(3, offset))
    sine.setParameterInput(4, Coefficient(4, phase))
    
    plotter = ParameterPlotter(1, standalone=True)
    plotter.setParameterInput(0, sine, 0)
    
    def update(t):
        sine.update(t)
        plotter.update(t)

    plotter.updateFunction = update
    while plotter.isRunning():
        time.sleep(0.1)
