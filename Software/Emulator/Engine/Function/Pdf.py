import numpy as np
import sys
sys.path.append("..")
from Modules import Function

class Pdf(Function):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "mean", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "var", "module": None, "sourceIndex" : 0})
        
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
    
    def update(self, t):
        parameterInputsReady = all([i["module"].ready for i in self.parameterInputs])
        inputsReady = all([i["module"].ready for i in self.inputs])
        self.ready = parameterInputsReady and inputsReady
        if not self.ready:
            return False
        
        mu = self.parameterInputs[0]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"] * Function.pixelcount
        sigma = np.sqrt(self.parameterInputs[1]["module"].parameterOutputs[self.parameterInputs[1]["sourceIndex"]]["value"])
        c = (1 / (sigma * np.sqrt(2 * np.pi))) * np.exp(-0.5 * ((np.arange(Function.pixelcount) - mu) / sigma)**2)
        c *= 1 / np.max(c)
        self.outputs[0]["value"] = np.vstack((c, c, c, c, c, c)).T                                            
        return True
    