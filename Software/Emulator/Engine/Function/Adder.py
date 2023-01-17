import numpy as np
import sys
sys.path.append("..")
from Modules import Function


class Adder(Function):
    def __init__(self, id):
        super().__init__(id)
        self.inputs.append({"name": "in_a", "module": None, "sourceIndex": 0, "default": np.zeros((Function.pixelcount, 6))})
        self.inputs.append({"name": "in_b", "module": None, "sourceIndex": 0, "default": np.zeros((Function.pixelcount, 6))})
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
    def update(self, t):
        if not super().update(t):
            return False
        
        input_a = self._getInput(0)
        input_b = self._getInput(1)
        
        self.outputs[0]["value"] = input_a + input_b
        return True