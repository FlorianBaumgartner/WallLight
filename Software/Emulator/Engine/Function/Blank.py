import numpy as np
import sys
sys.path.append("..")
from Modules import Function


class Blank(Function):
    def __init__(self, id):
        super().__init__(id)
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
    def update(self, t):
        if not super().update(t):
            return False
        return True