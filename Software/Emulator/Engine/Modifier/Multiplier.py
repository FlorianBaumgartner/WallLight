import numpy as np
import sys
sys.path.append("..")
from Modules import Modifier

class Multiplier(Modifier):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "input 0", "module": None, "sourceIndex": 0, "default": 1.0})
        self.parameterInputs.append({"name": "input 1", "module": None, "sourceIndex": 0, "default": 1.0})

    def update(self, t):
        if super().update(t) == False:
            return False
        
        output = self._getParameterValue(0) * self._getParameterValue(1)
        self.parameterOutputs[0]["value"] = output
        return True