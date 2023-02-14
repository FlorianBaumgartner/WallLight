import numpy as np
import sys
sys.path.append("..")
from Modules import Modifier

class Constrain(Modifier):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "input", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "min", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "max", "module": None, "sourceIndex": 0, "default": 0.0})
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        output = self._getParameterValue(0)
        
        if self.parameterInputs[1]["module"]:   # Check if minimum value has been set
            output = max(output, self._getParameterValue(1))
        if self.parameterInputs[2]["module"]:   # Check if maximum value has been set
            output = min(output, self._getParameterValue(2))
        
        self.parameterOutputs[0]["value"] = output
        return True