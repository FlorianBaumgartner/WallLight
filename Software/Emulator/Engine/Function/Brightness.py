import numpy as np
import sys
sys.path.append("..")
from Modules import Function


class Brightness(Function):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "brightness", "module": None, "sourceIndex" : 0})
        
        self.inputs.append({"name": "in", "module": None, "sourceIndex" : 0})
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
    def update(self, t):
        if not super().update(t):
            return False
        
        brightness = self.parameterInputs[0]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"]
        
        self.outputs[0]["value"] = self.inputs[0]["module"].outputs[self.inputs[0]["sourceIndex"]]["value"] * brightness
        return True