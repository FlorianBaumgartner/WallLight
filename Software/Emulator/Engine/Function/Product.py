import numpy as np
import sys
sys.path.append("..")
from Modules import Function


class Product(Function):
    def __init__(self, id=None):
        super().__init__(id)
        self.inputs.append({"name": "input 0", "module": None, "sourceIndex": 0, "default": np.ones((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 1", "module": None, "sourceIndex": 0, "default": np.ones((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 2", "module": None, "sourceIndex": 0, "default": np.ones((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 3", "module": None, "sourceIndex": 0, "default": np.ones((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 4", "module": None, "sourceIndex": 0, "default": np.ones((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 5", "module": None, "sourceIndex": 0, "default": np.ones((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 6", "module": None, "sourceIndex": 0, "default": np.ones((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 7", "module": None, "sourceIndex": 0, "default": np.ones((Function.pixelcount, 6))})
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
    def update(self, t):
        if not super().update(t):
            return False
        
        input0 = self._getInput(0)
        input1 = self._getInput(1)
        input2 = self._getInput(2)
        input3 = self._getInput(3)
        input4 = self._getInput(4)
        input5 = self._getInput(5)
        input6 = self._getInput(6)
        input7 = self._getInput(7)
        
        self.outputs[0]["value"] = input0 * input1 * input2 * input3 * input4 * input5 * input6 * input7
        return True
