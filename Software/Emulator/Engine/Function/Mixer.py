import numpy as np
import sys
sys.path.append("..")
from Modules import Function


class Mixer(Function):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "mix", "module": None, "sourceIndex" : 0})
        
        self.inputs.append({"name": "input 0", "module": None, "sourceIndex" : 0})
        self.inputs.append({"name": "input 1", "module": None, "sourceIndex" : 0})
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
    def update(self, t):
        if not super().update(t):
            return False
        
        mix = np.clip(self.parameterInputs[0]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"], 0.0, 1.0)
        input_0 = self.inputs[0]["module"].outputs[self.inputs[0]["sourceIndex"]]["value"]
        input_1 = self.inputs[1]["module"].outputs[self.inputs[0]["sourceIndex"]]["value"]
        
        self.outputs[0]["value"] = input_1 * mix + input_0 * (1.0 - mix)
        return True