import numpy as np
import sys
sys.path.append("..")
from Modules import Modifier

class Constrain(Modifier):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "input", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "min", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "max", "module": None, "sourceIndex" : 0})
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        output = np.clip(self.parameterInputs[0]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"],
                         self.parameterInputs[1]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"],
                         self.parameterInputs[2]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"])
        self.parameterOutputs[0]["value"] = output
        return True