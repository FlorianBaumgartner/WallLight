import numpy as np
import sys
sys.path.append("..")
from Modules import Function


class Adder(Function):
    def __init__(self, id=None):
        super().__init__(id)
        self.inputs.append({"name": "input 0", "module": None, "sourceIndex": 0, "default": np.zeros((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 1", "module": None, "sourceIndex": 0, "default": np.zeros((Function.pixelcount, 6))})
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
    def update(self, t):
        if not super().update(t):
            return False
        
        input0 = self._getInput(0)
        input1 = self._getInput(1)
        
        self.outputs[0]["value"] = input0 + input1
        return True
