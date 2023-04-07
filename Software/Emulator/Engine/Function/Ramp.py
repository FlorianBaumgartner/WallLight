import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Function

class Ramp(Function):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "start", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "stop", "module": None, "sourceIndex": 0, "default": 1.0})
        self.parameterInputs.append({"name": "low", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "high", "module": None, "sourceIndex": 0, "default": 1.0})
        self.parameterInputs.append({"name": "clip", "module": None, "sourceIndex": 0, "default": 1.0})
        
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
    
    def update(self, t):
        if not super().update(t):
            return False
        
        start = self._getParameterValue(0)
        stop = self._getParameterValue(1)
        low = self._getParameterValue(2)
        high = self._getParameterValue(3)
        clip = self._getParameterValue(4) >= 0.5

        dx = stop - start
        dy = high - low
        if(dx == 0):
            self.outputs[0]["value"] = np.ones((Function.pixelcount, 6)) * low
            return True
        
        m = dy / dx
        x0 = int(min(start, stop) * Function.pixelcount + 0.5)
        x0f = x0 - (min(start, stop) * Function.pixelcount)
        y0 = 0.0 if m >= 0 else high
        c = np.ones(Function.pixelcount) * low
        for i in range(int(np.abs(dx) * Function.pixelcount + 0.5)):
            if(0 <= (x0 + i) < Function.pixelcount):
                c[x0 + i] = low + y0 + (m * (i + x0f)) / Function.pixelcount
        
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
    
    start = -0.1
    stop = 0.9
    low = 0.0
    high = 1.0
    clip = 0.0
    
    gen = Generator.Triangle(0)
    gen.setParameterInput(1, Coefficient(1005, 0.2))
    gen.setParameterInput(3, Coefficient(1006, 0.1))
    gen.setParameterInput(4, Coefficient(1007, 0.5))
    
    
    ramp = Function.Ramp(1)
    ramp.setParameterInput(0, Coefficient(1000, start))
    ramp.setParameterInput(1, gen)#ëCoefficient(1001, stop))
    ramp.setParameterInput(2, Coefficient(1002, low))
    ramp.setParameterInput(3, Coefficient(1003, high))
    ramp.setParameterInput(4, Coefficient(1004, clip))
    
    inputPlotter = Analyzer.InputPlotter(2)
    inputPlotter.setInput(0, ramp)
    
    
    wallLight.addModule([gen, ramp, inputPlotter])
    wallLight.setOutput(ramp, 0)
    wallLight.run()