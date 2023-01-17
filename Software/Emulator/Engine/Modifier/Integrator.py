import numpy as np
import sys
sys.path.append("..")
from Modules import Module, Modifier


class Integrator(Modifier):
    def __init__(self, id):
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
        
        self.output += inputValue * (gain / Module.framerate)
        
        if self.parameterInputs[2]["module"]:   # Check if minimum value has been set
            self.output = max(self.output, minValue)
        if self.parameterInputs[3]["module"]:   # Check if maximum value has been set
            self.output = min(self.output, maxValue)
        if reset:
            self.output = 0.0
        
        self.parameterOutputs[0]["value"] = self.output
        return True
    
if __name__ == '__main__':
    import time
    from Modules import Coefficient, Modifier, Analyzer
    Module.framerate = 60
    
    inputValue = 1.0
    gain = 1.0
    minValue = 0.0
    maxValue = 1.0
    reset = 0.0
    
    integrator = Modifier.Integrator(0)
    integrator.setParameterInput(0, Coefficient(2, inputValue))
    integrator.setParameterInput(1, Coefficient(3, gain))
    integrator.setParameterInput(2, Coefficient(4, minValue))
    integrator.setParameterInput(3, Coefficient(5, maxValue))
    integrator.setParameterInput(4, Coefficient(6, reset))
    
    plotter = Analyzer.ParameterPlotter(1, standalone=True)
    plotter.setParameterInput(0, integrator, 0)
    
    def update(t):
        integrator.update(t)
        plotter.update(t)

    plotter.updateFunction = update
    while plotter.isRunning():
        time.sleep(0.1)