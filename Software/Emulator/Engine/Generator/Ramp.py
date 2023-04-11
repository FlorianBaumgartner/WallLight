import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Generator

class Ramp(Generator):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "freq", "module": None, "sourceIndex" : 0, "default": 1.0})
        self.parameterInputs.append({"name": "rep", "module": None, "sourceIndex" : 0, "default": -1.0})  # -1 mean endless
        self.parameterInputs.append({"name": "start", "module": None, "sourceIndex" : 0, "default": 0.0})
        self.parameterInputs.append({"name": "stop", "module": None, "sourceIndex" : 0, "default": 1.0})
        self.parameterInputs.append({"name": "phase", "module": None, "sourceIndex" : 0, "default": 0.0})   # -1 means -180° ... +1 means + 180°  
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        enable = self._getParameterValue(0) >= 0.5
        freq = self._getParameterValue(1)
        rep = self._getParameterValue(2)
        start = self._getParameterValue(3)
        stop = self._getParameterValue(4)
        phase = self._getParameterValue(5)
        phase = (phase + 1.0) % 2.0 - 1.0           # Phase range: -1.0 ... 1.0
        amplitude = np.abs(start - stop)
        slope = amplitude * freq
        slope *= 1.0 if(start < stop) else -1.0
        
        output = start
        if enable:
            t -= self.enableTime
            if(t == 0):
                output = start
            elif(rep < 0) or ((rep / freq) >= t):            # End value is real end value
                output = min(start, stop) + ((t + (phase / 2.0)) * slope) % amplitude
            else:
                output = min(start, stop) + ((rep / freq + (phase / 2.0)) * slope)   # Stay at last value without modulo -> TODO: Check if this makes sence
        else:
            self.enableTime = t
            t = 0

        self.parameterOutputs[0]["value"] = output
        return True
    

if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Modifier, Analyzer, Function
    wallLight = WallLight()
    
    ramp = Generator.Ramp(0)
    ramp.setParameterInput(0, Coefficient(1000, 1.0))           # enable
    ramp.setParameterInput(1, Coefficient(1001, 1.0))           # freq
    ramp.setParameterInput(2, Coefficient(1002, 1.0))           # rep
    ramp.setParameterInput(3, Coefficient(1003, 0.0))           # start
    ramp.setParameterInput(4, Coefficient(1004, 1.0))           # stop
    ramp.setParameterInput(5, Coefficient(1005, 0.0))           # phase

    rect = Function.Rect(1)
    rect.setParameterInput(0, Coefficient(1006, 0.0))           # start
    rect.setParameterInput(1, ramp)                             # stop

    plotter = Analyzer.ParameterPlotter(2)
    plotter.setParameterInput(0, ramp)

    wallLight.addModule([ramp, rect, plotter])
    wallLight.setOutput(rect, 0)
    wallLight.run()
