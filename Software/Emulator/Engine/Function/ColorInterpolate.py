import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Function
from Utility import Utility


class ColorInterpolate(Function):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "mode", "module": None, "sourceIndex": 0, "default": 0.0})
        
        self.inputs.append({"name": "in", "module": None, "sourceIndex" : 0, "default": np.ones((Function.pixelcount, 6)) * (-1.0)})
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
    def update(self, t):
        if not super().update(t):
            return False
        
        mode = self._getParameterValue(0) >= 0.5    # 0.0 = RGB, 1.0 = HSV
        
        inputVector = np.copy(self._getInput(0))
        
        r  = inputVector[:,0]
        g  = inputVector[:,1]
        b  = inputVector[:,2]
        ww = inputVector[:,3]
        cw = inputVector[:,4]
        am = inputVector[:,5]
        
        if mode:
            mask = np.clip(r + 1.0, 0.0, 1.0)  
            h, s, v = Utility.rgbToHsv(np.clip(r, 0.0, 1.0), np.clip(g, 0.0, 1.0), np.clip(b, 0.0, 1.0))  
            h  = Utility.interpolate((h + 1.0) * mask - 1.0)
            s  = Utility.interpolate((s + 1.0) * mask - 1.0)
            v  = Utility.interpolate((v + 1.0) * mask - 1.0)
            r, g, b = Utility.hsvToRgb(h, s, v)
        else:
            r  = Utility.interpolate(r)
            g  = Utility.interpolate(g)
            b  = Utility.interpolate(b)
            
        ww = Utility.interpolate(ww)
        cw = Utility.interpolate(cw)
        am = Utility.interpolate(am)
        
        self.outputs[0]["value"] = np.vstack([r.T, g.T, b.T, ww.T, cw.T, am.T]).T
        return True
    

if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Analyzer, Function
    wallLight = WallLight()
    
    pos0 = 0.1
    pos1 = 0.5
    pos2 = 0.9
    
    mode = 0.0    # 0.0 = RGB, 1.0 = HSV
    
    plotChannel = 0
    
    colorInsert0 = Function.ColorInsert(0)
    colorInsert0.setParameterInput(0, Coefficient(1000, pos0))
    colorInsert0.setParameterInput(1, Coefficient(1001, 1.0))   # Red
    colorInsert0.setParameterInput(2, Coefficient(1001, 0.0))   # Green
    colorInsert0.setParameterInput(3, Coefficient(1002, 1.0))   # Blue
    
    colorInsert1 = Function.ColorInsert(1)
    colorInsert1.setParameterInput(0, Coefficient(1003, pos1))
    colorInsert1.setParameterInput(1, Coefficient(1004, 0.0))   # Red
    colorInsert1.setParameterInput(2, Coefficient(1005, 1.0))   # Green
    colorInsert1.setParameterInput(3, Coefficient(1006, 1.0))   # Blue
    colorInsert1.setInput(0, colorInsert0)
    
    colorInsert2 = Function.ColorInsert(2)
    colorInsert2.setParameterInput(0, Coefficient(1007, pos2))
    colorInsert2.setParameterInput(1, Coefficient(1008, 1.0))   # Red
    colorInsert2.setParameterInput(2, Coefficient(1009, 1.0))   # Green
    colorInsert2.setParameterInput(3, Coefficient(1010, 0.0))   # Blue
    colorInsert2.setInput(0, colorInsert1)
    
    colorInterpolate = Function.ColorInterpolate(3)
    colorInterpolate.setParameterInput(0, Coefficient(1011, mode))
    colorInterpolate.setInput(0, colorInsert2)
    
    inputPlotter = Analyzer.InputPlotter(5000)
    inputPlotter.setParameterInput(0, Coefficient(1012, plotChannel))
    inputPlotter.setInput(0, colorInterpolate)
    
      
    wallLight.addModule([colorInsert0, colorInsert1, colorInsert2, colorInterpolate, inputPlotter])
    wallLight.setOutput(colorInterpolate, 0)
    # wallLight.setOutput(colorInsert2, 0)
    wallLight.run()