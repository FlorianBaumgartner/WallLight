import numpy as np
import sys
sys.path.append("..")
from Modules import Function

class Triangle(Function):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "position", "module": None, "sourceIndex": 0, "default": 0.5})
        self.parameterInputs.append({"name": "width", "module": None, "sourceIndex": 0, "default": 1.0})
        self.parameterInputs.append({"name": "low", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "high", "module": None, "sourceIndex": 0, "default": 1.0})
        self.parameterInputs.append({"name": "clip", "module": None, "sourceIndex": 0, "default": 1.0})
        
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
    
    def update(self, t):
        if not super().update(t):
            return False
        
        position = self._getParameterValue(0)
        width = self._getParameterValue(1)
        low = self._getParameterValue(2)
        high = self._getParameterValue(3)
        clip = self._getParameterValue(4) >= 0.5
        
        dy = high - low
        if(width == 0):
            self.outputs[0]["value"] = np.ones((Function.pixelcount, 6)) * low
            return True
        
        m = (dy / width) * 2.0
        x0 = int(position * Function.pixelcount + 0.5)
        x0f = x0 - (position * Function.pixelcount)
        c = np.ones(Function.pixelcount) * low
        for i in range(int(width * Function.pixelcount + 0.5)):
            if(0 <= (x0 + i) < Function.pixelcount):
                c[x0 + i] = high - (m * (i + x0f)) / Function.pixelcount
            if(0 <= (x0 - i) < Function.pixelcount):
                c[x0 - i] = high - (m * (i - x0f)) / Function.pixelcount
        
        if clip:
            c = np.clip(c, 0.0, 1.0)
        
        self.outputs[0]["value"] = np.vstack((c, c, c, c, c, c)).T                                            
        return True
    

if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Analyzer, Function
    wallLight = WallLight()
    
    position = 0.1
    width = 0.5
    low = 0.0
    high = 1.0
    clip = 1.0
    
    gen = Generator.Triangle(0)
    gen.setParameterInput(1, Coefficient(1005, 0.02))
    gen.setParameterInput(3, Coefficient(1006, 0.1))
    gen.setParameterInput(4, Coefficient(1007, 0.5))
    
    
    triangle = Function.Triangle(1)
    triangle.setParameterInput(0, gen) #Coefficient(1000, position))
    triangle.setParameterInput(1, Coefficient(1001, width))
    triangle.setParameterInput(2, Coefficient(1002, low))
    triangle.setParameterInput(3, Coefficient(1003, high))
    triangle.setParameterInput(4, Coefficient(1004, clip))
    
    inputPlotter = Analyzer.InputPlotter(2)
    inputPlotter.setInput(0, triangle)
    
    
    wallLight.addModule([gen, triangle, inputPlotter])
    wallLight.setOutput(triangle, 0)
    wallLight.run()