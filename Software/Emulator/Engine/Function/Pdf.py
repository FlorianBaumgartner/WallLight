import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Function

class Pdf(Function):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "mean", "module": None, "sourceIndex": 0, "default": 0.5})
        self.parameterInputs.append({"name": "var", "module": None, "sourceIndex": 0, "default": 1.0})
        
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
    
    def update(self, t):
        if not super().update(t):
            return False
        
        mean = self._getParameterValue(0)
        var = max(np.abs(self._getParameterValue(1)), 0.00001)
        c = np.exp(-(np.linspace(0.0, 1.0, Function.pixelcount) - mean)**2 / (2 * var))
        self.outputs[0]["value"] = np.vstack((c, c, c, c, c, c)).T                                            
        return True
    

if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Analyzer, Function
    wallLight = WallLight()
    
    mean = 0.5
    var = 0.01
    
    pdf = Function.Pdf(0)
    pdf.setParameterInput(0, Coefficient(2, mean))
    pdf.setParameterInput(1, Coefficient(3, var))
    
    inputPlotter = Analyzer.InputPlotter(1)
    inputPlotter.setParameterInput(0, Coefficient(4, 0))
    inputPlotter.setInput(0, pdf)
    
    wallLight.addModule([pdf, inputPlotter])
    wallLight.setOutput(pdf, 0)
    wallLight.run()