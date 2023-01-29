import numpy as np
import sys
sys.path.append("..")
from Modules import Module, Modifier


class ExpSmoothing(Modifier):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "input", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "alpha", "module": None, "sourceIndex": 0, "default": 0.5})
        self.output = 0.0
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        inputValue = self._getParameterValue(0)
        alpha = (np.clip(self._getParameterValue(1), 0.0, 1.0)**0.1) * 0.999
        
        self.output = alpha * self.output + (1.0 - alpha) * inputValue
        self.parameterOutputs[0]["value"] = self.output
        return True
    
if __name__ == '__main__':
    import time
    from Modules import Coefficient, Generator, Modifier, Analyzer
    Module.framerate = 60
    
    alpha = 0.5
    
    noise = Generator.Random(0)    
    
    smoothing = Modifier.ExpSmoothing(1)
    smoothing.setParameterInput(0, noise)
    smoothing.setParameterInput(1, Coefficient(1000, alpha))
    
    plotter = Analyzer.ParameterPlotter(2, standalone=True)
    plotter.setParameterInput(0, noise, 0)
    plotter.setParameterInput(1, smoothing, 0)
    
    def update(t):
        noise.update(t)
        smoothing.update(t)
        plotter.update(t)

    plotter.updateFunction = update
    while plotter.isRunning():
        time.sleep(0.1)
        
