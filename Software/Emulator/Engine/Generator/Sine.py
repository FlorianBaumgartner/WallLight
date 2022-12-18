import numpy as np
import sys
sys.path.append("..")
from Modules import Generator

class Sine(Generator):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "amplitude", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "offset", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "phase", "module": None, "sourceIndex" : 0})   # -1 means -180° ... +1 means + 180°  
        

        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        freq = self.parameterInputs[0]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"]
        rep = self.parameterInputs[1]["module"].parameterOutputs[self.parameterInputs[1]["sourceIndex"]]["value"]
        amplitude = self.parameterInputs[2]["module"].parameterOutputs[self.parameterInputs[2]["sourceIndex"]]["value"]
        offset = self.parameterInputs[3]["module"].parameterOutputs[self.parameterInputs[3]["sourceIndex"]]["value"]
        phase = self.parameterInputs[4]["module"].parameterOutputs[self.parameterInputs[4]["sourceIndex"]]["value"]
        phase = (phase + 1.0) % 2.0 - 1.0
          
        output = 0.0
        if(rep == -1) or (rep / freq < t):
            x = t * 2 * np.pi * freq + (phase / freq) * np.pi
            output = np.sin(x) * amplitude + offset
        self.parameterOutputs[0]["value"] = output
        return True