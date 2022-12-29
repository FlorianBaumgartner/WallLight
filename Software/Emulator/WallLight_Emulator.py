import time
import sys
import signal
import numpy as np
from random import randint
from PyQt5.QtWidgets import QApplication, QWidget, QDesktopWidget
from PyQt5.QtGui import QPainter, QColor
from PyQt5.QtCore import Qt, QTimer

from Engine.Engine import Engine


WINDOW_WIDTH = 250
WINDOW_HEIGHT = 1000
PIXELS_OFFSET = 50
FRAME_RATE = 60

NUM_PIXELS = 288


class MyWidget(QWidget):
    def __init__(self):
        super().__init__()
        
        path = "Graphs/test_graph_analyzer.json"
        # path = "Graphs/sine_analyzer.json"
        # path = "Graphs/test_graph.json"
        
        self.engine = Engine(NUM_PIXELS, FRAME_RATE)
        self.engine.loadGraph(path)

        self.outputBuffer = np.zeros((NUM_PIXELS, 6))        
        self.colors = [QColor(0, 0, 0) for _ in range(NUM_PIXELS)]
        self.pixels = []
        height = WINDOW_HEIGHT - PIXELS_OFFSET * 2
        hPixel = int(height / NUM_PIXELS)
        offset = int((WINDOW_HEIGHT - hPixel * NUM_PIXELS) / 2)
        for i in range(NUM_PIXELS):
            self.pixels.append((offset, offset + hPixel * i, hPixel * 3, hPixel))
            
            
        self.timer = QTimer()
        self.timer.timeout.connect(self.change_colors)
        self.timer.start(int(1000 / FRAME_RATE))

        self.setStyleSheet("background-color: black;")
        self.setWindowTitle('WallLight Emulator')
        self.setGeometry(300, 300, WINDOW_WIDTH, WINDOW_HEIGHT)
        self.setWindowFlags(Qt.WindowStaysOnTopHint)
        self.center()
        self.setWindowFlags(self.windowFlags() & ~Qt.WindowStaysOnTopHint)
        self.show()
        
    def closeEvent(self, event):
        self.engine.end()
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
        self.outputBuffer = self.engine.getPixelData()
        for i, c in enumerate(self.outputBuffer):
            r = int(c[0] * 255) % 256
            g = int(c[1] * 255) % 256
            b = int(c[2] * 255) % 256
            self.colors[i].setRgb(r, g, b)
        self.update()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    widget = MyWidget()
    def signal_handler(sig, frame):
        widget.close()
    signal.signal(signal.SIGINT, signal_handler)
    widget.show()
    while widget.isVisible():
        app.processEvents()
