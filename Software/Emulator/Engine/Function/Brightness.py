import numpy as np
import sys
sys.path.append("..")
from Modules import Function


class Brightness(Function):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "brightness", "module": None, "sourceIndex" : 0, "default": 1.0})
        
        self.inputs.append({"name": "in", "module": None, "sourceIndex" : 0, "default": np.zeros((Function.pixelcount, 6))})
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
    def update(self, t):
        if not super().update(t):
            return False
        
        brightness = self._getParameterValue(0)
        
        self.outputs[0]["value"] = self._getInput(0) * brightness
        return True