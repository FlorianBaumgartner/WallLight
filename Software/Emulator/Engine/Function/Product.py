import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Function


class Product(Function):
    def __init__(self, id=None):
        super().__init__(id)
        self.inputs.append({"name": "input 0", "module": None, "sourceIndex": 0, "default": np.ones((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 1", "module": None, "sourceIndex": 0, "default": np.ones((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 2", "module": None, "sourceIndex": 0, "default": np.ones((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 3", "module": None, "sourceIndex": 0, "default": np.ones((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 4", "module": None, "sourceIndex": 0, "default": np.ones((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 5", "module": None, "sourceIndex": 0, "default": np.ones((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 6", "module": None, "sourceIndex": 0, "default": np.ones((Function.pixelcount, 6))})
        self.inputs.append({"name": "input 7", "module": None, "sourceIndex": 0, "default": np.ones((Function.pixelcount, 6))})
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
    def update(self, t):
        if not super().update(t):
            return False
        
        numConnectedInputs = 0
        output = np.ones((Function.pixelcount, 6))
        for i in range(8):
            output *= self._getInput(i)
            if(self._checkInputValid(i)):
                numConnectedInputs += 1
        if(numConnectedInputs == 0):
            output = np.zeros((Function.pixelcount, 6))
        self.outputs[0]["value"] = output
        return True
    
    
if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Modifier, Analyzer, Function
    wallLight = WallLight()
    
    pdf0 = Function.Pdf(0)
    pdf0.setParameterInput(0, Coefficient(1000, 0.35))
    pdf0.setParameterInput(1, Coefficient(1001, 0.01))
                           
    pdf1 = Function.Pdf(1)
    pdf1.setParameterInput(0, Coefficient(1002, 0.65))
    pdf1.setParameterInput(1, Coefficient(1003, 0.01))
    
    product = Function.Product(3)
    product.setInput(0, pdf0)
    product.setInput(1, pdf1)
    
    wallLight.addModule([pdf0, pdf1, product])
    wallLight.setOutput(product, 0)
    wallLight.run()
