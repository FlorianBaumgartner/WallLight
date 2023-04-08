import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Function


class ColorGain(Function):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "r", "module": None, "sourceIndex": 0, "default": 0.0})    # Red Channel
        self.parameterInputs.append({"name": "g", "module": None, "sourceIndex": 0, "default": 0.0})    # Green Channel
        self.parameterInputs.append({"name": "b", "module": None, "sourceIndex": 0, "default": 0.0})    # Blue Channel
        self.parameterInputs.append({"name": "ww", "module": None, "sourceIndex": 0, "default": 0.0})   # Warm-White Channel
        self.parameterInputs.append({"name": "cw", "module": None, "sourceIndex": 0, "default": 0.0})   # Cold-White Channel
        self.parameterInputs.append({"name": "am", "module": None, "sourceIndex": 0, "default": 0.0})   # Amber Channel
        
        self.inputs.append({"name": "in", "module": None, "sourceIndex" : 0, "default": np.zeros((Function.pixelcount, 6))})
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
    def update(self, t):
        if not super().update(t):
            return False
        
        r = self._getParameterValue(0)
        g = self._getParameterValue(1)
        b = self._getParameterValue(2)
        ww = self._getParameterValue(3)
        cw = self._getParameterValue(4)
        am = self._getParameterValue(5)
        
        self.outputs[0]["value"] = self._getInput(0) * np.array([r, g, b, ww, cw, am])
        return True