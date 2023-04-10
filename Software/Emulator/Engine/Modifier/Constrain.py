import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
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
        minimum = self._getParameterValue(1)
        maximum = self._getParameterValue(2)
        
        if self._checkParameterValid(1) and self._checkParameterValid(2):  # When both inputs are availabe swap min and max value if they are reversed
            temp = minimum
            minimum = maximum
            maximum = temp
        
        if self._checkParameterValid(1):        # Check if minimum value has been set  
            output = max(output, minimum)
        if self._checkParameterValid(2):        # Check if maximum value has been set
            output = min(output, maximum)
        
        self.parameterOutputs[0]["value"] = output
        return True
