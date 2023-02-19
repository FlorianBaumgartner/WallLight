import numpy as np
import sys
sys.path.append("..")
from Modules import Generator

class Rect(Generator):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "freq", "module": None, "sourceIndex" : 0, "default": 1.0})
        self.parameterInputs.append({"name": "rep", "module": None, "sourceIndex" : 0, "default": -1.0})  # -1 mean endless
        self.parameterInputs.append({"name": "amplitude", "module": None, "sourceIndex" : 0, "default": 1.0})
        self.parameterInputs.append({"name": "offset", "module": None, "sourceIndex" : 0, "default": 0.0})
        self.parameterInputs.append({"name": "phase", "module": None, "sourceIndex" : 0, "default": 0.0})   # -1 means -180° ... +1 means + 180°  
        self.parameterInputs.append({"name": "dutycicle", "module": None, "sourceIndex" : 0, "default": 0.5})

        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        enable = self._getParameterValue(0) >= 0.5
        freq = self._getParameterValue(1)
        rep = self._getParameterValue(2)
        amplitude = self._getParameterValue(3)
        offset = self._getParameterValue(4)
        phase = self._getParameterValue(5)
        dutycicle = self._getParameterValue(6)
        phase = (phase + 1.0) % 2.0 - 1.0
        
        if enable:
            t -= self.enableTime
            if(rep >= 0) and ((rep / freq) < t):            # End value always corresponds to t0
                t = 0
        else:
            self.enableTime = t
            t = 0

        output = (1.0 if((t * freq - (phase / (freq * 2))) % 1.0 <  dutycicle) else -1.0) * amplitude + offset
        self.parameterOutputs[0]["value"] = output
        return True
    
if __name__ == '__main__':
    import time
    from Modules import Module, Coefficient, Generator, Analyzer
    Module.framerate = 60
    
    enable = 1.0
    freq = 1.0
    rep = -1
    amp = 0.5
    offset = 0.5
    phase = 0.0
    dutycicle = 0.5
    
    enableCoeff = Coefficient(2, enable)
    
    ramp = Generator.Rect(0)
    ramp.setParameterInput(0, Coefficient(2, enable))
    ramp.setParameterInput(1, Coefficient(3, freq))
    ramp.setParameterInput(2, Coefficient(4, rep))
    ramp.setParameterInput(3, Coefficient(5, amp))
    ramp.setParameterInput(4, Coefficient(6, offset))
    ramp.setParameterInput(5, Coefficient(7, phase))
    ramp.setParameterInput(6, Coefficient(8, dutycicle))
    
    plotter = Analyzer.ParameterPlotter(1, standalone=True)
    plotter.setParameterInput(0, ramp, 0)
    
    def update(t):
        ramp.update(t)
        plotter.update(t)

    plotter.updateFunction = update
    while plotter.isRunning():
        time.sleep(0.1)