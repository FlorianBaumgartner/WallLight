import numpy as np
import sys
sys.path.append("..")
from Modules import Generator

class Triangle(Generator):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "amplitude", "module": None, "sourceIndex" : 0, "default": 1.0})
        self.parameterInputs.append({"name": "offset", "module": None, "sourceIndex" : 0, "default": 0.0})
        self.parameterInputs.append({"name": "phase", "module": None, "sourceIndex" : 0, "default": 0.0})   # -1 means -180° ... +1 means + 180°  

        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        enable = self._getParameterValue(0)
        freq = self._getParameterValue(1)
        rep = self._getParameterValue(2)
        amplitude = self._getParameterValue(3)
        offset = self._getParameterValue(4)
        phase = self._getParameterValue(5)
        phase = (phase + 1.0) % 2.0 - 1.0
          
        if enable:
            t -= self.enableTime
            if(rep >= 0) and ((rep / freq) < t):            # End value always corresponds to t0
                t = 0
        else:
            self.enableTime = t
            t = 0
        
        x = 1.0 - np.abs(((t * freq - (phase / freq)) % 1.0) * 2.0 - 1.0)
        output = (x - 0.5) * 2 * amplitude + offset
        self.parameterOutputs[0]["value"] = output
        return True
    
if __name__ == '__main__':
    import time
    from Modules import Module, Coefficient, Generator, Analyzer
    Module.framerate = 60
    
    enable = 1.0
    freq = 1.0
    rep = 1.0
    amp = 1.0
    offset = 0.0
    phase = -0.25
    
    triangle = Generator.Triangle(0)
    triangle.setParameterInput(0, Coefficient(2, enable))
    triangle.setParameterInput(1, Coefficient(3, freq))
    triangle.setParameterInput(2, Coefficient(4, rep))
    triangle.setParameterInput(3, Coefficient(5, amp))
    triangle.setParameterInput(4, Coefficient(6, offset))
    triangle.setParameterInput(5, Coefficient(7, phase))
    
    plotter = Analyzer.ParameterPlotter(1, standalone=True)
    plotter.setParameterInput(0, triangle, 0)
    
    def update(t):
        triangle.update(t)
        plotter.update(t)

    plotter.updateFunction = update
    while plotter.isRunning():
        time.sleep(0.1)
    