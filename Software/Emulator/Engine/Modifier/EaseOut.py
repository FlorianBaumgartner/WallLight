import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Modifier

class EaseOut(Modifier):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "input", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "type", "module": None, "sourceIndex": 0, "default": 0.0})

        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        x = self._getParameterValue(0)
        easeType = int(self._getParameterValue(1) + 0.5)
        
        output = 0
        if(x >= 1.0):
            output = 1.0
        elif(x <= 0.0):
            output = 0.0
        else:
            if easeType == 0:       # Sine
                output = np.sin((x * np.pi) / 2.0)
            elif easeType == 1:     # Quad
                output = 1.0 - (1.0 - x)**2
            elif easeType == 2:     # Cubic
                output = 1.0 - (1.0 - x)**3
            elif easeType == 3:     # Quart
                output = 1.0 - (1.0 - x)**4
            elif easeType == 4:     # Quint
                output = 1.0 - (1.0 - x)**5
            elif easeType == 5:     # Expo
                output = 1.0 - 2.0**(-10.0 * x)
            elif easeType == 6:     # Circ
                output = np.sqrt(1.0 - (x - 1.0)**2)
            elif easeType == 7:     # Back
                output = 1.0 + 2.70158 * (x - 1.0)**3 + 1.70158 * (x - 1.0)**2
            elif easeType == 8:     # Elastic
                output = 1.0 - (1.0 - x)**4 * np.cos(x * np.pi * 4.586) * 1.0377               # Original: 4.5
            elif easeType == 9:     # Bounce
                output = 1.0 - 2.0**(-6.0 * x) * np.abs(np.cos(x * np.pi * 3.612)) * 1.065     # Original: 3.5
            else:
                raise Exception("Easy-Function type out of bound!")
            
        self.parameterOutputs[0]["value"] = output
        return True


if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Modifier, Analyzer, Function
    wallLight = WallLight()
    
    enable = 1.0
    freq = 1.0
    rep = 1
    start = 0
    stop = 1.0
    phase = 0.0
    
    easeType = 9.0
    
    ramp = Generator.Ramp(0)
    ramp.setParameterInput(0, Coefficient(3, enable))
    ramp.setParameterInput(1, Coefficient(4, freq))
    ramp.setParameterInput(2, Coefficient(5, rep))
    ramp.setParameterInput(3, Coefficient(6, start))
    ramp.setParameterInput(4, Coefficient(7, stop))
    ramp.setParameterInput(5, Coefficient(8, phase))
    
    easeOut = Modifier.EaseOut(1)
    easeOut.setParameterInput(0, ramp)
    easeOut.setParameterInput(1, Coefficient(9, easeType))
    
    plotter = Analyzer.ParameterPlotter(2, autoMove=False)
    plotter.setParameterInput(0, ramp)
    plotter.setParameterInput(1, easeOut)
    
    rect = Function.Rect(3)
    rect.setParameterInput(0, Coefficient(10, 0.0))
    rect.setParameterInput(1, easeOut)
    
    wallLight.addModule([ramp, easeOut, plotter, rect])
    wallLight.setOutput(rect, 0)
    wallLight.run()
