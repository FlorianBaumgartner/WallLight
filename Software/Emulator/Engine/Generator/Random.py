import numpy as np
import time
import sys
sys.path.append("..")
from Modules import Generator

class Random(Generator):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "min", "module": None, "sourceIndex" : 0, "default": 0.0})
        self.parameterInputs.append({"name": "max", "module": None, "sourceIndex" : 0, "default": 1.0})
        np.random.seed(seed=int(time.time())) 
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        enable = self._getParameterValue(0)
        minimum = self._getParameterValue(1)
        maximum = self._getParameterValue(2)
        
        output = np.random.rand() * np.abs(maximum - minimum) + minimum
        if not enable:
            output = 0.0
        
        self.parameterOutputs[0]["value"] = output
        return True
    
if __name__ == '__main__':
    import time
    from Modules import Module, Coefficient, Generator, Analyzer
    Module.framerate = 60
    
    enable = 1.0
    minimum = -1.0
    maximum = 1.0
    
    random = Generator.Random(0)
    random.setParameterInput(0, Coefficient(2, enable))
    random.setParameterInput(1, Coefficient(3, minimum))
    random.setParameterInput(2, Coefficient(4, maximum))
    
    plotter = Analyzer.ParameterPlotter(1, standalone=True)
    plotter.setParameterInput(0, random, 0)
    
    def update(t):
        random.update(t)
        plotter.update(t)

    plotter.updateFunction = update
    while plotter.isRunning():
        time.sleep(0.1)
    