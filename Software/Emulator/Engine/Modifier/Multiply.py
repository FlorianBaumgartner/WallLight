import numpy as np
import sys
sys.path.append("..")
from Modules import Modifier

class Multiply(Modifier):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "input_a", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "input_b", "module": None, "sourceIndex" : 0})
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        output = self.parameterInputs[0]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"] * \
                  self.parameterInputs[1]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"]
        self.parameterOutputs[0]["value"] = output
        return True