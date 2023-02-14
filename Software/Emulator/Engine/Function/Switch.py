import numpy as np
import sys
sys.path.append("..")
from Modules import Function


class Switch(Function):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "switch", "module": None, "sourceIndex": 0, "default": 0.0})
        
        self.inputs.append({"name": "input 0", "module": None, "sourceIndex": 0, "default": np.zeros((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 1", "module": None, "sourceIndex": 0, "default": np.zeros((Function.pixelcount, 6))})
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
    def update(self, t):
        if not super().update(t):
            return False
        
        switch = self._getParameterValue(0) >= 0.5
        input_0 = self._getInput(0)
        input_1 = self._getInput(1)
        
        self.outputs[0]["value"] = input_1 if switch else input_0     # 0 -> input_0, 1 -> input_1
        return True