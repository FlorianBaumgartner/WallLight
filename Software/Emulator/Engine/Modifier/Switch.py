import numpy as np
import sys
sys.path.append("..")
from Modules import Modifier


class Switch(Modifier):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "input 0", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "input 1", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "switch", "module": None, "sourceIndex": 0, "default": 0.0})
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        input_0 = self._getParameterValue(0)
        input_1 = self._getParameterValue(1)
        switch = self._getParameterValue(2) >= 0.5
        
        self.parameterOutputs[0]["value"] = input_1 if switch else input_0     # 0 -> input_0, 1 -> input_1
        return True