import time
import sys
import numpy as np
from random import randint
from PyQt5.QtWidgets import QApplication, QWidget
from PyQt5.QtGui import QPainter, QColor
from PyQt5.QtCore import QTimer

from Engine.Engine import Engine


WINDOW_WIDTH = 250
WINDOW_HEIGHT = 1000
PIXELS_OFFSET = 50
FRAME_RATE = 60

NUM_PIXELS = 288

class MyWidget(QWidget):
    def __init__(self):
        super().__init__()
        self.engine = Engine(NUM_PIXELS, FRAME_RATE)

        self.outputBuffer = np.zeros((NUM_PIXELS, 6))        
        self.colors = [QColor(0, 0, 0) for _ in range(NUM_PIXELS)]
        self.pixels = []
        height = WINDOW_HEIGHT - PIXELS_OFFSET * 2
        hPixel = int(height / NUM_PIXELS)
        offset = int((WINDOW_HEIGHT - hPixel * NUM_PIXELS) / 2)
        for i in range(NUM_PIXELS):
            self.pixels.append((offset, offset + hPixel * i, hPixel * 3, hPixel))
            

        self.setStyleSheet("background-color: black;")
        self.setWindowTitle('WallLight Emulator')
        self.setGeometry(300, 300, WINDOW_WIDTH, WINDOW_HEIGHT)

        self.timer = QTimer()
        self.timer.timeout.connect(self.change_colors)
        self.timer.start(int(1000 / 60))

    def paintEvent(self, event):
        qp = QPainter()
        qp.begin(self)
        for pixels, color in zip(self.pixels, self.colors):
            qp.fillRect(*pixels, color)
        qp.end()

    def change_colors(self):
        # Get Pixel data
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
    widget.show()
    sys.exit(app.exec_())
