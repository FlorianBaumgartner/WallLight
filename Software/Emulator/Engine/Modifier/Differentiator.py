import numpy as np
import sys
sys.path.append("..")
from Modules import Module, Modifier


class Differentiator(Modifier):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "input", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "gain", "module": None, "sourceIndex": 0, "default": 1.0})
        self.inputValue = None
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        inputValue = self._getParameterValue(0)
        gain = self._getParameterValue(1)
        
        if not self.inputValue:
            self.inputValue = inputValue
        
        output = (inputValue - self.inputValue) * gain * Module.framerate
        self.inputValue = inputValue
        self.parameterOutputs[0]["value"] = output
        return True
    
if __name__ == '__main__':
    import time
    from Modules import Coefficient, Generator, Modifier, Analyzer
    Module.framerate = 60
    
    freq = 0.1
    
    inputValue = 1.0
    gain = 1.0 / (2.0 * np.pi * freq)
    
    sine = Generator.Sine(0)
    sine.setParameterInput(1, Coefficient(1000, freq))         # freq
    
    triangle = Generator.Triangle(10)
    triangle.setParameterInput(1, Coefficient(1001, 0.1))      # freq
    triangle.setParameterInput(3, Coefficient(1002, 2.5))      # amplitude
    
    differentiator = Modifier.Differentiator(1)
    differentiator.setParameterInput(0, sine)
    differentiator.setParameterInput(1, Coefficient(3, gain))
    
    plotter = Analyzer.ParameterPlotter(2, standalone=True, autoMove=False, stepMode=False)
    plotter.setParameterInput(0, sine, 0)
    plotter.setParameterInput(1, differentiator, 0)
    
    def update(t):
        sine.update(t)
        triangle.update(t)
        differentiator.update(t)
        plotter.update(t)

    plotter.updateFunction = update
    while plotter.isRunning():
        time.sleep(0.1)