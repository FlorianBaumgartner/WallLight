import numpy as np
import sys
sys.path.append("..")
from Modules import Module, Modifier


class Integrator(Modifier):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "input", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "gain", "module": None, "sourceIndex": 0, "default": 1.0})
        self.parameterInputs.append({"name": "min", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "max", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "reset", "module": None, "sourceIndex": 0, "default": 0.0})
        self.output = 0.0
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        
        inputValue = self._getParameterValue(0)
        gain = self._getParameterValue(1)
        minValue = self._getParameterValue(2)
        maxValue = self._getParameterValue(3)
        reset = self._getParameterValue(4) >= 0.5
        
        if self.parameterInputs[2]["module"]:   # Check if minimum value has been set
            self.output = max(self.output, minValue)
        if self.parameterInputs[3]["module"]:   # Check if maximum value has been set
            self.output = min(self.output, maxValue)
        if reset:
            self.output = 0.0
        
        self.parameterOutputs[0]["value"] = self.output
        self.output += inputValue * (gain / Module.framerate)
        return True
    
if __name__ == '__main__':
    import time
    from Modules import Coefficient, Generator, Modifier, Analyzer
    Module.framerate = 60
    
    freq = 0.1
    
    inputValue = 1.0
    gain = 2 * np.pi * freq
    minValue = 0.0
    maxValue = 1.0
    reset = 0.0
    
    sine = Generator.Sine(0)
    sine.setParameterInput(1, Coefficient(1000, freq))        # freq
    sine.setParameterInput(5, Coefficient(1001, -0.5))        # phase
    
    integrator = Modifier.Integrator(1)
    integrator.setParameterInput(0, sine)
    integrator.setParameterInput(1, Coefficient(3, gain))
    # integrator.setParameterInput(2, Coefficient(4, minValue))
    # integrator.setParameterInput(3, Coefficient(5, maxValue))
    integrator.setParameterInput(4, Coefficient(6, reset))
    
    plotter = Analyzer.ParameterPlotter(2, standalone=True, autoMove=False, stepMode=True)
    plotter.setParameterInput(0, sine, 0)
    plotter.setParameterInput(1, integrator, 0)
    
    def update(t):
        sine.update(t)
        integrator.update(t)
        plotter.update(t)

    plotter.updateFunction = update
    while plotter.isRunning():
        time.sleep(0.1)