import numpy as np
import sys
sys.path.append("..")
from Modules import Function


class ColorBlend(Function):
    def __init__(self, id):
        super().__init__(id)
        self.inputs.append({"name": "input 0", "module": None, "sourceIndex": 0, "default": np.zeros((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 1", "module": None, "sourceIndex": 0, "default": np.zeros((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 2", "module": None, "sourceIndex": 0, "default": np.zeros((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 3", "module": None, "sourceIndex": 0, "default": np.zeros((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 4", "module": None, "sourceIndex": 0, "default": np.zeros((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 5", "module": None, "sourceIndex": 0, "default": np.zeros((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 6", "module": None, "sourceIndex": 0, "default": np.zeros((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 7", "module": None, "sourceIndex": 0, "default": np.zeros((Function.pixelcount, 6))})
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
    def update(self, t):
        if not super().update(t):
            return False
        
        inputVector = self._getInput(0)
        inputVector = np.maximum(self._getInput(1), inputVector)
        inputVector = np.maximum(self._getInput(2), inputVector)
        inputVector = np.maximum(self._getInput(3), inputVector)
        inputVector = np.maximum(self._getInput(4), inputVector)
        inputVector = np.maximum(self._getInput(5), inputVector)
        inputVector = np.maximum(self._getInput(6), inputVector)
        inputVector = np.maximum(self._getInput(7), inputVector)
        
        self.outputs[0]["value"] = inputVector
        return True
