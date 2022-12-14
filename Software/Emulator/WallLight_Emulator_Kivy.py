import time
from kivy.config import Config
from kivy.core.window import Window
from kivy.app import App
from kivy.uix.widget import Widget
from kivy.graphics import Rectangle, Color
from kivy.clock import Clock

WINDOW_WIDTH = 250
WINDOW_HEIGHT = 1000
FRAME_RATE = 60

NUM_PIXELS = 288

class Touch(Widget):
    def __init__(self, **kwargs):
        super(Touch, self).__init__(**kwargs)
        with self.canvas:
            self.pixels = []
            for i in range(NUM_PIXELS):
                self.pixels.append(Color(rgba=(0, 0, 0, 1)))
                Rectangle(pos=(50, 50), size=(10, 10))
            Clock.schedule_interval(self.update, 1/FRAME_RATE)

    def update(self, *args):
        for c in self.pixels:
            c.rgba = (((time.time() * 100) % 100)/100, 0, 0, 1)

class App(App):
    def build(self):
        return Touch()

if __name__ == '__main__':
    Config.set('kivy', 'exit_on_escape', '0')
    Config.set('graphics', 'resizable', False)
    Window.size = (WINDOW_WIDTH, WINDOW_HEIGHT)
    App().run()
    Window.close()
