import numpy as np
import sys
sys.path.append("..")
from Modules import Modifier


class Mixer(Modifier):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "input 0", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "input 1", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "mix", "module": None, "sourceIndex": 0, "default": 0.0})
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        input_0 = self._getParameterValue(0)
        input_1 = self._getParameterValue(1)
        mix = np.clip(self._getParameterValue(2), 0.0, 1.0)
        
        self.parameterOutputs[0]["value"] = input_1 * mix + input_0 * (1.0 - mix)
        return True