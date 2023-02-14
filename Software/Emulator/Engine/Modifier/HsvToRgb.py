import numpy as np
import sys
sys.path.append("..")
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
    from Modules import Modifier
    
    hsvToRgb = Modifier.HsvToRgb(0)