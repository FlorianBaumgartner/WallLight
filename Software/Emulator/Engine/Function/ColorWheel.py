import numpy as np
import sys
sys.path.append("..")
from Modules import Function


class ColorWheel(Function):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "frequency", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "position", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "saturation", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "brightness", "module": None, "sourceIndex" : 0})

        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
    def update(self, t):
        if not super().update(t):
            return False
        
        freq = self.parameterInputs[0]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"]
        pos = self.parameterInputs[1]["module"].parameterOutputs[self.parameterInputs[1]["sourceIndex"]]["value"]
        sat = self.parameterInputs[2]["module"].parameterOutputs[self.parameterInputs[2]["sourceIndex"]]["value"]
        bright = self.parameterInputs[2]["module"].parameterOutputs[self.parameterInputs[3]["sourceIndex"]]["value"]
        
        for i in range(Function.pixelcount):
            hue = (i / Function.pixelcount) * freq + pos
            r, g, b = self.__hsvToRgb(hue, sat, bright)
            self.outputs[0]["value"][i, 0] = r
            self.outputs[0]["value"][i, 1] = g
            self.outputs[0]["value"][i, 2] = b
        # TODO: Add Gamma Correction as described in: https://github.com/adafruit/Adafruit_NeoPixel/blob/master/Adafruit_NeoPixel.cpp
        return True
    
    def __hsvToRgb(self, hue, sat, val):
        if sat == 0.0:
            return val, val, val
        hue = hue * 6.0
        if hue == 6.0:
            hue = 0.0
        i = int(hue)
        f = (hue) - i
        p, q, t = val * (1.0 - sat), val * (1.0 - sat * f), val * (1.0 - sat * (1.0 - f))
        i %= 6
        if i == 0:
            return val, t, p
        if i == 1:
            return q, val, p
        if i == 2:
            return p, val, t
        if i == 3:
            return p, q, val
        if i == 4:
            return t, p, val
        if i == 5:
            return val, p, q
        
        
if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient
    wallLight = WallLight()
    
    freq = 1.0
    pos = 0.0
    sat = 1.0
    bright = 1.0
    
    colorWheel = ColorWheel(0)
    colorWheel.setParameterInput(0, Coefficient(1, freq))
    colorWheel.setParameterInput(1, Coefficient(2, pos))
    colorWheel.setParameterInput(2, Coefficient(3, sat))
    colorWheel.setParameterInput(3, Coefficient(4, bright))
    
    wallLight.addModule(colorWheel)
    wallLight.setOutput(colorWheel, 0)
    wallLight.run()
    
    