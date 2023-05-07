import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Modifier
from Utility import Utility

class HsvToRgb(Modifier):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "hue", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "sat", "module": None, "sourceIndex": 0, "default": 1.0})
        self.parameterInputs.append({"name": "val", "module": None, "sourceIndex": 0, "default": 1.0})
        self.parameterOutputs = [{"name": "red", "value": 0.0},
                                 {"name": "green", "value": 0.0},
                                 {"name": "blue", "value": 0.0}]
                                 
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        hue = self._getParameterValue(0)
        sat = self._getParameterValue(1)
        val = self._getParameterValue(2)
        
        red, green, blue = Utility.hsvToRgb(hue, sat, val)
        self.parameterOutputs[0]["value"] = red
        self.parameterOutputs[1]["value"] = green
        self.parameterOutputs[2]["value"] = blue
        return True


if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Modifier, Analyzer, Function
    wallLight = WallLight()
    
    hsvToRgb = Modifier.HsvToRgb(0)
    hsvToRgb.setParameterInput(0, Coefficient(1000, 0.2))       # hue
    hsvToRgb.setParameterInput(1, Coefficient(1001, 1.0))       # sat
    hsvToRgb.setParameterInput(2, Coefficient(1002, 1.0))       # val

    rect = Function.Rect(1)
    
    colorGain = Function.ColorGain(2)
    colorGain.setParameterInput(0, hsvToRgb, 0)                 # red
    colorGain.setParameterInput(1, hsvToRgb, 1)                 # green
    colorGain.setParameterInput(2, hsvToRgb, 2)                 # blue
    colorGain.setInput(0, rect)

    wallLight.addModule([hsvToRgb, rect, colorGain])
    wallLight.setOutput(colorGain, 0)
    wallLight.run()
