import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Modifier

class Subtractor(Modifier):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "minuend", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "subtrahend", "module": None, "sourceIndex": 0, "default": 0.0})
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        minuend = self._getParameterValue(0)
        subtrahend = self._getParameterValue(1)
        
        self.parameterOutputs[0]["value"] = minuend - subtrahend
        return True