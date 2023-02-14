import numpy as np
import sys
sys.path.append("..")
from Modules import Generator

class Ramp(Generator):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "freq", "module": None, "sourceIndex" : 0, "default": 1.0})
        self.parameterInputs.append({"name": "rep", "module": None, "sourceIndex" : 0, "default": -1.0})  # -1 mean endless
        self.parameterInputs.append({"name": "start", "module": None, "sourceIndex" : 0, "default": 0.0})
        self.parameterInputs.append({"name": "stop", "module": None, "sourceIndex" : 0, "default": 1.0})
        self.parameterInputs.append({"name": "phase", "module": None, "sourceIndex" : 0, "default": 0.0})   # -1 means -180° ... +1 means + 180°  
        self.outputValue = 0.0
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        enable = self._getParameterValue(0)
        freq = self._getParameterValue(1)
        rep = self._getParameterValue(2)
        start = self._getParameterValue(3)
        stop = self._getParameterValue(4)
        phase = self._getParameterValue(5)
        phase = (phase + 1.0) % 2.0 - 1.0           # Phase range: -1.0 ... 1.0
        amplitude = np.abs(start - stop)
        slope = amplitude * freq
        slope *= 1.0 if(start < stop) else -1.0
        
        if enable:
            t -= self.enableTime
            if(t == 0):
                self.outputValue = start
            elif(rep < 0) or ((rep / freq) >= t):            # End value is real end value
                self.outputValue = min(start, stop) + ((t + (phase / (freq * 2))) * slope) % amplitude
        else:
            self.enableTime = t
            t = 0

        self.parameterOutputs[0]["value"] = self.outputValue
        return True
    

if __name__ == '__main__':
    import time
    from Modules import Module, Coefficient, Generator, Analyzer
    Module.framerate = 60
    
    enable = 1.0
    freq = 1.0
    rep = 1.0
    start = 0.0
    stop = 1.0
    phase = 0.0
    
    ramp = Generator.Ramp(0)
    ramp.setParameterInput(0, Coefficient(2, enable))
    ramp.setParameterInput(1, Coefficient(3, freq))
    ramp.setParameterInput(2, Coefficient(4, rep))
    ramp.setParameterInput(3, Coefficient(5, start))
    ramp.setParameterInput(4, Coefficient(6, stop))
    ramp.setParameterInput(5, Coefficient(7, phase))
    
    plotter = Analyzer.ParameterPlotter(1, autoMove=False, standalone=True)
    plotter.setParameterInput(0, ramp, 0)
    
    def update(t):
        ramp.update(t)
        plotter.update(t)

    plotter.updateFunction = update
    while plotter.isRunning():
        time.sleep(0.1)
    