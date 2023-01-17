import numpy as np
import sys
sys.path.append("..")
from Modules import Function


class ColorGain(Function):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "r", "module": None, "sourceIndex": 0, "default": 1.0})    # Red Channel
        self.parameterInputs.append({"name": "g", "module": None, "sourceIndex": 0, "default": 1.0})    # Green Channel
        self.parameterInputs.append({"name": "b", "module": None, "sourceIndex": 0, "default": 1.0})    # Blue Channel
        self.parameterInputs.append({"name": "ww", "module": None, "sourceIndex": 0, "default": 1.0})   # Warm-White Channel
        self.parameterInputs.append({"name": "cw", "module": None, "sourceIndex": 0, "default": 1.0})   # Cold-White Channel
        self.parameterInputs.append({"name": "am", "module": None, "sourceIndex": 0, "default": 1.0})   # Amber Channel
        
        self.inputs.append({"name": "in", "module": None, "sourceIndex" : 0, "default": np.zeros((Function.pixelcount, 6))})
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
    def update(self, t):
        if not super().update(t):
            return False
        
        r = self.parameterInputs[0]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"]
        g = self.parameterInputs[1]["module"].parameterOutputs[self.parameterInputs[1]["sourceIndex"]]["value"]
        b = self.parameterInputs[2]["module"].parameterOutputs[self.parameterInputs[2]["sourceIndex"]]["value"]
        ww = self.parameterInputs[3]["module"].parameterOutputs[self.parameterInputs[3]["sourceIndex"]]["value"]
        cw = self.parameterInputs[4]["module"].parameterOutputs[self.parameterInputs[4]["sourceIndex"]]["value"]
        am = self.parameterInputs[5]["module"].parameterOutputs[self.parameterInputs[5]["sourceIndex"]]["value"]
        
        self.outputs[0]["value"] = self.inputs[0]["module"].outputs[self.inputs[0]["sourceIndex"]]["value"] * np.array([r, g, b, ww, cw, am])
        return True