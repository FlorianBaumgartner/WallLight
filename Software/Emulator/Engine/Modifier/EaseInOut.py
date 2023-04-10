import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Modifier

class EaseInOut(Modifier):
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
                output = -(np.cos(np.pi * x) - 1.0) / 2.0
            elif easeType == 1:     # Quad
                output = 2.0 * x**2 if(x < 0.5) else x * (4.0 - 2.0 * x) - 1.0
            elif easeType == 2:     # Cubic
                output = 4.0 * x**3 if(x < 0.5) else 1.0 - (-2.0 * x + 2.0)**3 / 2.0
            elif easeType == 3:     # Quart
                output = 8.0 * x**4 if(x < 0.5) else 1.0 - (-2.0 * x + 2.0)**4 / 2.0
            elif easeType == 4:     # Quint
                output = 16.0 * x**5 if(x < 0.5) else 1.0 - (-2.0 * x + 2.0)**5 / 2.0
            elif easeType == 5:     # Expo
                output = 2.0**(20.0 * x - 10.0) / 2.0 if(x < 0.5) else (2.0 - 2.0**(-20.0 * x + 10.0)) / 2.0
            elif easeType == 6:     # Circ
                output = (1.0 - np.sqrt(1.0 - (2.0 * x)**2)) / 2.0 if(x < 0.5) else (np.sqrt(1.0 - (-2.0 * x + 2.0)**2) + 1.0) / 2.0
            elif easeType == 7:     # Back
                output = ((2.0 * x)**2 * (7.1898 * x - 2.5949)) / 2.0 if(x < 0.5) else ((2.0 * x - 2.0)**2 * (3.5949 * (x * 2.0 - 2.0) + 2.5949) + 2) / 2.0
            elif easeType == 8:     # Elastic
                output = -(2.0**(20.0 * x - 10.0) * np.sin((20.0 * x - 11.125) * ((2.0 * np.pi) / 4.5))) / 2.0 if(x < 0.5) else (2.0**(-20.0 * x + 10.0) * np.sin((20.0 * x - 11.125) * ((2.0 * np.pi) / 4.5))) / 2.0 + 1.0
            elif easeType == 9:     # Bounce
                output = 8.0 * 2.0**(8.0 * (x - 1.0)) * np.abs(np.sin(x * np.pi * 7.0)) if(x < 0.5) else 1.0 - 8.0 * 2.0**(-8.0 * x) * np.abs(np.sin(x * np.pi * 7.0))
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
    
    easeInOut = Modifier.EaseInOut(1)
    easeInOut.setParameterInput(0, ramp)
    easeInOut.setParameterInput(1, Coefficient(9, easeType))
    
    plotter = Analyzer.ParameterPlotter(2, autoMove=False)
    plotter.setParameterInput(0, ramp)
    plotter.setParameterInput(1, easeInOut)
    
    rect = Function.Rect(3)
    rect.setParameterInput(0, Coefficient(10, 0.0))
    rect.setParameterInput(1, easeInOut)
    
    wallLight.addModule([ramp, easeInOut, plotter, rect])
    wallLight.setOutput(rect, 0)
    wallLight.run()
