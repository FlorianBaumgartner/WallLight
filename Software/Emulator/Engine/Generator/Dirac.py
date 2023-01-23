import numpy as np
import sys
sys.path.append("..")
from Modules import Module, Generator

class Dirac(Generator):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "freq", "module": None, "sourceIndex" : 0, "default": 1.0})
        self.parameterInputs.append({"name": "rep", "module": None, "sourceIndex" : 0, "default": -1.0})  # -1 mean endless
        self.parameterInputs.append({"name": "weight", "module": None, "sourceIndex" : 0, "default": 1.0})
        self.parameterInputs.append({"name": "offset", "module": None, "sourceIndex" : 0, "default": 0.0})
        self.parameterInputs.append({"name": "phase", "module": None, "sourceIndex" : 0, "default": 0.0})   # -1 means -180° ... +1 means + 180°  

        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        enable = self._getParameterValue(0)
        freq = self._getParameterValue(1)
        rep = self._getParameterValue(2)
        weight = self._getParameterValue(3)
        offset = self._getParameterValue(4)
        phase = self._getParameterValue(5)
        phase = (phase + 1.0) % 2.0 - 1.0
        if phase < 0:
            phase += 2
        
        discreteinterval = max(int(Module.framerate / freq), 1)
        discretePhaseDelay = int((Module.framerate * phase * 0.5) / freq + 0.5)
        
        output = offset
        if not enable:
            self.enableTime = t
            t = 0
        else:
            t -= self.enableTime
            x = int(t * Module.framerate + 0.5)
            if x < 0:
                x += discreteinterval
            if(x % discreteinterval == discretePhaseDelay):
                if((rep < 0.0) or (rep / freq) * Module.framerate > x):
                    output += weight

        self.parameterOutputs[0]["value"] = output
        return True
    
if __name__ == '__main__':
    import time
    from Modules import Module, Generator, Coefficient, Analyzer
    Module.framerate = 60
        
    enable = 1.0
    freq = 1.0
    rep = -1
    weight = 1.0
    offset = 0.0
    phase = 0.0
    

    dirac = Generator.Dirac(0)
    dirac.setParameterInput(0, Coefficient(2, enable))
    dirac.setParameterInput(1, Coefficient(3, freq))
    dirac.setParameterInput(2, Coefficient(4, rep))
    dirac.setParameterInput(3, Coefficient(5, weight))
    dirac.setParameterInput(4, Coefficient(6, offset))
    dirac.setParameterInput(5, Coefficient(7, phase))
    
    plotter = Analyzer.ParameterPlotter(1, standalone=True, autoMove=True)
    plotter.setParameterInput(0, dirac, 0)
    
    def update(t):
        dirac.update(t)
        plotter.update(t)

    plotter.updateFunction = update
    while plotter.isRunning():
        time.sleep(0.1)
    