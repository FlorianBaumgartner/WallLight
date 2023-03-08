import os
import sys
import time
import signal
import traceback
import numpy as np
from PyQt5.QtWidgets import QApplication, QWidget, QDesktopWidget
from PyQt5.QtGui import QPainter, QColor
from PyQt5.QtCore import Qt, QTimer

sys.path.append(os.path.join(os.path.dirname(__file__), "Engine"))
from Engine import Engine
from Modules import Module


class WallLight():
    WINDOW_WIDTH = 250
    WINDOW_HEIGHT = 1000
    PIXELS_OFFSET = 50
    
    framerate = 50
    pixelcount = 288

    def __init__(self):
        self.app = QApplication(sys.argv)
        self.mainWidget = MainWidget()
        self.running = False
        
        def signal_handler(sig, frame):
            self.mainWidget.close()
        signal.signal(signal.SIGINT, signal_handler)
        
    def __del__(self):
        self.stop()
        
    def start(self):
        if self.running:
            return
        self.mainWidget.show()
        self.mainWidget.startTimer()
        
    def stop(self):
        if not self.running:
            return
        self.app.quit()
        
    def isRunning(self):
        return self.mainWidget.isVisible()
    
    def update(self):
        self.app.processEvents()
    
    def run(self):
        self.start()
        while self.isRunning():
            self.update()
        self.stop()

    def loadGraph(self, path):
        self.mainWidget.engine.loadGraph(path)
        
    def saveGraph(self, path, name):
        self.mainWidget.engine.saveGraph(path, name)
        
    def loadModules(self):
        self.mainWidget.engine.loadModules()
        
    def listModules(self):
        print(self.mainWidget.engine)
        
    def setOutput(self, module, index):
        self.mainWidget.engine.setOutput(module, index)
        
    def updateCoefficient(self, identity, value):
        self.mainWidget.engine.updateCoefficient(identity, value)
        
    def updateColorVector(self, identity, value):
        self.mainWidget.engine.updateColorVector(identity, value)
        
    def addModule(self, module):
        self.mainWidget.engine.addModule(module)


    

class MainWidget(QWidget):
    def __init__(self):
        super().__init__()
        self.engine = Engine(WallLight.pixelcount, WallLight.framerate)
        self.tFps = time.time()

        self.outputBuffer = np.zeros((WallLight.pixelcount, 6))        
        self.colors = [QColor(0, 0, 0) for _ in range(WallLight.pixelcount)]
        self.pixels = []
        height = WallLight.WINDOW_HEIGHT - WallLight.PIXELS_OFFSET * 2
        hPixel = int(height / WallLight.pixelcount)
        offset = int((WallLight.WINDOW_HEIGHT - hPixel * WallLight.pixelcount) / 2)
        for i in range(WallLight.pixelcount):
            self.pixels.append((offset, offset + hPixel * i, 20, hPixel))
        
        self.timer = QTimer()
        self.timer.timeout.connect(self.change_colors)
        
        self.setStyleSheet("background-color: black;")
        self.setWindowTitle('WallLight Emulator')
        self.setGeometry(300, 300, WallLight.WINDOW_WIDTH, WallLight.WINDOW_HEIGHT)
        self.setWindowFlags(Qt.WindowStaysOnTopHint)
        self.center()
        self.setWindowFlags(self.windowFlags() & ~Qt.WindowStaysOnTopHint)
        self.show()
        
    def stop(self):
        self.close()
        self.timer.stop()
        self.engine.stop()
        
    def startTimer(self):
        self.timer.start(int(1000 / WallLight.framerate))
        
    def closeEvent(self, event):
        self.stop()
        event.accept()


    def center(self):
        qr = self.frameGeometry()
        cp = QDesktopWidget().availableGeometry().center()
        qr.moveCenter(cp)
        self.move(qr.topLeft())
        self.show()

    def paintEvent(self, event):
        qp = QPainter()
        qp.begin(self)
        for pixels, color in zip(self.pixels, self.colors):
            qp.fillRect(*pixels, color)
        qp.end()
        

    def change_colors(self):
        try:
            self.outputBuffer = self.engine.getPixelData()
        except Exception as e:
            self.end()
            traceback.print_exc()
        
        self.outputBuffer = np.clip(self.outputBuffer, 0.0, 1.0)    # Constrain values in range from 0.0 ... 1.0
        for i, c in enumerate(self.outputBuffer):
            r = int(c[0] * 255)
            g = int(c[1] * 255)
            b = int(c[2] * 255)
            self.colors[WallLight.pixelcount - i - 1].setRgb(r, g, b)
        self.update()
        
        # print(f"Real FPS: {1 / (time.time() - self.tFps):.2f}")
        self.tFps = time.time()


if __name__ == '__main__':
    wallLight = WallLight()    
    
    # path = "Graphs/integrate_differentiate_test.json"
    # path = "Graphs/color_vector_test.json"      # wallLight.updateColorVector(0, np.ones((WallLight.pixelcount, 6)))
    # path = "Graphs/generator_dirac_test.json"
    # path = "Graphs/input_plotter_test.json"
    # path = "Graphs/rainbow_rect.json"
    path = "Graphs/rect_test.json"
    # path = "Graphs/rect_triangle_test.json"
    # path = "Graphs/rect_triangle_color_test.json"
    # path = "Graphs/rainbow.json"
    # path = "Graphs/test_graph_analyzer_dual.json"
    # path = "Graphs/sine_analyzer.json"
    # path = "Graphs/test_graph.json"
    # path = "Graphs/test_error.json"
    
    wallLight.loadGraph(path)
    wallLight.start()
    while wallLight.isRunning():
        wallLight.update()
    wallLight.stop()
