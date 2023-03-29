import numpy as np
import sys
sys.path.append("..")
from Modules import Function
from Utility import Utility


class ColorWheel(Function):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "cycles", "module": None, "sourceIndex": 0, "default": 1.0})
        self.parameterInputs.append({"name": "position", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "saturation", "module": None, "sourceIndex": 0, "default": 1.0})
        self.parameterInputs.append({"name": "brightness", "module": None, "sourceIndex": 0, "default": 1.0})

        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
    def update(self, t):
        if not super().update(t):
            return False
        
        cycles = self._getParameterValue(0)
        pos = self._getParameterValue(1)
        sat = self._getParameterValue(2)
        bright = self._getParameterValue(3)
        
        for i in range(Function.pixelcount):
            hue = (i / Function.pixelcount) * cycles + pos
            r, g, b = Utility.hsvToRgb(hue, sat, bright)
            self.outputs[0]["value"][i, 0] = r
            self.outputs[0]["value"][i, 1] = g
            self.outputs[0]["value"][i, 2] = b
        # TODO: Add Gamma Correction as described in: https://github.com/adafruit/Adafruit_NeoPixel/blob/master/Adafruit_NeoPixel.cpp
        return True
        
        
if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Function, Generator
    wallLight = WallLight()
    
    freq = 1.0
    pos = 0.0
    sat = 1.0
    bright = 1.0
    
    generator = Generator.Ramp()
    generator.setParameterInput(1, Coefficient(0.1))
    generator.setParameterInput(3, Coefficient(0.0))
    generator.setParameterInput(4, Coefficient(1.0))
    
    colorWheel = Function.ColorWheel()
    colorWheel.setParameterInput(0, Coefficient(freq))
    colorWheel.setParameterInput(1, generator)
    colorWheel.setParameterInput(2, Coefficient(sat))
    colorWheel.setParameterInput(3, Coefficient(bright))
    
    wallLight.addModule([generator, colorWheel])
    wallLight.setOutput(colorWheel, 0)
    wallLight.run()
    
    