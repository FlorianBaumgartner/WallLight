import numpy as np
import sys
sys.path.append("..")
from Modules import Function

class Dirac(Function):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "position", "module": None, "sourceIndex": 0, "default": 0.5})
        self.parameterInputs.append({"name": "weight", "module": None, "sourceIndex": 0, "default": 1.0})
        self.parameterInputs.append({"name": "smooth", "module": None, "sourceIndex": 0, "default": 1.0})
        
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
    
    def update(self, t):
        if not super().update(t):
            return False
        
        position = self._getParameterValue(0)
        weight = self._getParameterValue(1)
        smooth = self._getParameterValue(2) >= 0.5
        
        c = np.zeros(Function.pixelcount)
        for i in range(Function.pixelcount):
            if smooth:
                val = position * (Function.pixelcount - 1) - i
                if(val >= 0.0 and val < 1.0):
                    c[i] = weight * (1.0 - val)
                if(val > -1.0 and val < 0.0):
                    c[i] = weight * (1.0 + val)
            else:
                if(int(i + 0.5) == int(position * (Function.pixelcount - 1))):
                    c[i] = weight
                     
        self.outputs[0]["value"] = np.vstack((c, c, c, c, c, c)).T                                            
        return True
    

if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Analyzer, Function
    wallLight = WallLight()
    
    enable = 1.0
    freq = 0.1
    rep = -1.0
    amplitude = 0.55
    offset = 0.5
    phase = 0.0
    
    weight = 1.0
    smooth = 1.0
    
    plotChannel = 0.0
    
    triangle = Generator.Triangle(0)
    triangle.setParameterInput(0, Coefficient(3, enable))
    triangle.setParameterInput(1, Coefficient(4, freq))
    triangle.setParameterInput(2, Coefficient(5, rep))
    triangle.setParameterInput(3, Coefficient(6, amplitude))
    triangle.setParameterInput(4, Coefficient(7, offset))
    triangle.setParameterInput(5, Coefficient(8, phase))
    
    dirac = Function.Dirac(1)
    dirac.setParameterInput(0, triangle)
    dirac.setParameterInput(1, Coefficient(9, weight))
    dirac.setParameterInput(2, Coefficient(10, smooth))
    
    inputPlotter = Analyzer.InputPlotter(2)
    inputPlotter.setParameterInput(0, Coefficient(11, plotChannel))
    inputPlotter.setInput(0, dirac)
    
    
    wallLight.addModule([triangle, dirac, inputPlotter])
    wallLight.setOutput(dirac, 0)
    wallLight.run()