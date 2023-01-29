import numpy as np
import sys
sys.path.append("..")
from Modules import Modifier
from Utility import Utility

class RgbToHsv(Modifier):
    def __init__(self, id):
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
    from Modules import Modifier
    
    rgbToHsv = Modifier.RgbToHsv(0)