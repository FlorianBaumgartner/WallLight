import numpy as np
import sys
sys.path.append("..")
from Modules import Function


class Multiply(Function):
    def __init__(self, id):
        super().__init__(id)
        self.inputs.append({"name": "in_a", "module": None, "sourceIndex" : 0})
        self.inputs.append({"name": "in_b", "module": None, "sourceIndex" : 0})
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
    def update(self, t):
        self.ready = all([i["module"].ready for i in self.inputs])
        if not self.ready:
            return False
        
        input_a = self.inputs[0]["module"].outputs[self.inputs[0]["sourceIndex"]]["value"]
        input_b = self.inputs[1]["module"].outputs[self.inputs[0]["sourceIndex"]]["value"]
        
        self.outputs[0]["value"] = input_a * input_b
        return True