import numpy as np
import sys
sys.path.append("..")
from Modules import Modifier

class Product(Modifier):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "input 0", "module": None, "sourceIndex": 0, "default": 1.0})
        self.parameterInputs.append({"name": "input 1", "module": None, "sourceIndex": 0, "default": 1.0})
        self.parameterInputs.append({"name": "input 2", "module": None, "sourceIndex": 0, "default": 1.0})
        self.parameterInputs.append({"name": "input 3", "module": None, "sourceIndex": 0, "default": 1.0})
        self.parameterInputs.append({"name": "input 4", "module": None, "sourceIndex": 0, "default": 1.0})
        self.parameterInputs.append({"name": "input 5", "module": None, "sourceIndex": 0, "default": 1.0})
        self.parameterInputs.append({"name": "input 6", "module": None, "sourceIndex": 0, "default": 1.0})
        self.parameterInputs.append({"name": "input 7", "module": None, "sourceIndex": 0, "default": 1.0})
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        output = 1.0
        for i in range(8):
            output *= self._getParameterValue(i)
        self.parameterOutputs[0]["value"] = output
        return True