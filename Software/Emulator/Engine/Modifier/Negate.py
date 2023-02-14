import numpy as np
import sys
sys.path.append("..")
from Modules import Modifier

class Negate(Modifier):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "input", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "enable", "module": None, "sourceIndex": 0, "default": 1.0})
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        inputValue = self._getParameterValue(0)
        enable = self._getParameterValue(1) >= 0.5
        
        self.parameterOutputs[0]["value"] = inputValue * (-1.0) if enable else inputValue
        return True