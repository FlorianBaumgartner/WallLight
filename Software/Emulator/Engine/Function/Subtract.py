import numpy as np
import sys
sys.path.append("..")
from Modules import Function


class Subtract(Function):
    def __init__(self, id):
        super().__init__(id)
        self.inputs.append({"name": "minuend", "module": None, "sourceIndex" : 0})
        self.inputs.append({"name": "subtrahend", "module": None, "sourceIndex" : 0})
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
    def update(self, t):
        if not super().update(t):
            return False
        
        minuend = self.inputs[0]["module"].outputs[self.inputs[0]["sourceIndex"]]["value"]
        subtrahend = self.inputs[1]["module"].outputs[self.inputs[0]["sourceIndex"]]["value"]
        
        self.outputs[0]["value"] = minuend - subtrahend
        return True