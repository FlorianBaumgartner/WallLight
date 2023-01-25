import numpy as np
import sys
sys.path.append("..")
from Modules import Function
from Utility import Utility


class ColorInterpolate(Function):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "tension", "module": None, "sourceIndex": 0, "default": 0.5})
        
        self.inputs.append({"name": "in", "module": None, "sourceIndex" : 0, "default": np.ones((Function.pixelcount, 6)) * (-1.0)})
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
    def update(self, t):
        if not super().update(t):
            return False
        
        tension = self._getParameterValue(0)
        
        r  = self._getInput(0)[:,0]
        g  = self._getInput(0)[:,1]
        b  = self._getInput(0)[:,2]
        ww = self._getInput(0)[:,3]
        cw = self._getInput(0)[:,4]
        am = self._getInput(0)[:,5]
        
        # h, s, v = Utility.rgbToHsv(r, g, b)
        
        # h  = Utility.interpolate(h, tension)
        # s  = Utility.interpolate(s, tension)
        # v  = Utility.interpolate(v, tension)
        r  = Utility.interpolate(r, tension)
        g  = Utility.interpolate(g, tension)
        b  = Utility.interpolate(b, tension)
        ww = Utility.interpolate(ww, tension)
        cw = Utility.interpolate(cw, tension)
        am = Utility.interpolate(am, tension)
        
        # r, g, b = Utility.hsvToRgb(h, s, v)

        self.outputs[0]["value"] = np.vstack([r.T, g.T, b.T, ww.T, cw.T, am.T]).T
        return True
    

if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Analyzer, Function
    wallLight = WallLight()
    
    pos0 = 0.2
    pos1 = 0.5
    pos2 = 0.7
    
    tension = 0.5
    
    plotChannel = 0.0
    
    sine = Generator.Sine(10)
    sine.setParameterInput(1, Coefficient(1020, 1.0))
    sine.setParameterInput(3, Coefficient(1020, 0.5))
    sine.setParameterInput(5, Coefficient(1020, 0.5))
    
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
    colorInterpolate.setParameterInput(0, sine)
    colorInterpolate.setInput(0, colorInsert2)

    
    inputPlotter = Analyzer.InputPlotter(5000)
    inputPlotter.setParameterInput(0, Coefficient(1012, plotChannel))
    inputPlotter.setInput(0, colorInterpolate)
      
    wallLight.addModule([sine, colorInsert0, colorInsert1, colorInsert2, colorInterpolate, inputPlotter])
    wallLight.setOutput(colorInterpolate, 0)
    wallLight.run()