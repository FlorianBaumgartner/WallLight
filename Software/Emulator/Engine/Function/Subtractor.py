import numpy as np
import sys
sys.path.append("..")
from Modules import Function


class Subtractor(Function):
    def __init__(self, id):
        super().__init__(id)
        self.inputs.append({"name": "minuend", "module": None, "sourceIndex": 0, "default": np.zeros((Function.pixelcount, 6))})
        self.inputs.append({"name": "subtrahend", "module": None, "sourceIndex": 0, "default": np.zeros((Function.pixelcount, 6))})
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
    def update(self, t):
        if not super().update(t):
            return False
        
        minuend = self._getInput(0)
        subtrahend = self._getInput(1)
        
        self.outputs[0]["value"] = minuend - subtrahend
        return True