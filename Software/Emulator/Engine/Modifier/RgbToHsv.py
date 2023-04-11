import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Modifier
from Utility import Utility

class RgbToHsv(Modifier):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "red", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "green", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "blue", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterOutputs = [{"name": "hue", "value": 0.0},
                                 {"name": "sat", "value": 0.0},
                                 {"name": "val", "value": 0.0}]
                                 
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        red = self._getParameterValue(0)
        green = self._getParameterValue(1)
        blue = self._getParameterValue(2)
        
        hue, sat, val = Utility.rgbToHsv(red, green, blue)
        self.parameterOutputs[0]["value"] = hue
        self.parameterOutputs[1]["value"] = sat
        self.parameterOutputs[2]["value"] = val
        return True


if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Modifier, Analyzer, Function
    wallLight = WallLight()

    rgbToHsv = Modifier.RgbToHsv(0)
    rgbToHsv.setParameterInput(0, Coefficient(1000, 0.0))       # red
    rgbToHsv.setParameterInput(1, Coefficient(1001, 1.0))       # green
    rgbToHsv.setParameterInput(2, Coefficient(1002, 1.0))       # blue

    hsvToRgb = Modifier.HsvToRgb(1)
    hsvToRgb.setParameterInput(0, rgbToHsv, 0)                  # hue
    hsvToRgb.setParameterInput(1, rgbToHsv, 1)                  # sat
    hsvToRgb.setParameterInput(2, rgbToHsv, 1)                  # val

    rect = Function.Rect(2)
    
    colorGain = Function.ColorGain(3)
    colorGain.setParameterInput(0, hsvToRgb, 0)                 # red
    colorGain.setParameterInput(1, hsvToRgb, 1)                 # green
    colorGain.setParameterInput(2, hsvToRgb, 2)                 # blue
    colorGain.setInput(0, rect)

    wallLight.addModule([rgbToHsv, hsvToRgb, rect, colorGain])
    wallLight.setOutput(colorGain, 0)
    wallLight.run()
