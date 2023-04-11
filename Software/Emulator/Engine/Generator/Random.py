import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Generator
import time

class Random(Generator):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "min", "module": None, "sourceIndex" : 0, "default": 0.0})
        self.parameterInputs.append({"name": "max", "module": None, "sourceIndex" : 0, "default": 1.0})
        np.random.seed(seed=int(time.time())) 
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        enable = self._getParameterValue(0) >= 0.5
        minimum = self._getParameterValue(1)
        maximum = self._getParameterValue(2)
        
        output = np.random.rand() * np.abs(maximum - minimum) + minimum
        if not enable:
            output = 0.0
        
        self.parameterOutputs[0]["value"] = output
        return True


if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Modifier, Analyzer, Function
    wallLight = WallLight()
    
    random = Generator.Random(0)
    random.setParameterInput(0, Coefficient(1000, 1.0))           # enable
    random.setParameterInput(1, Coefficient(1001, 0.0))           # minimum
    random.setParameterInput(2, Coefficient(1002, 1.0))           # maximum

    rect = Function.Rect(1)
    rect.setParameterInput(0, Coefficient(1003, 0.0))             # start
    rect.setParameterInput(1, random)                             # stop

    plotter = Analyzer.ParameterPlotter(2)
    plotter.setParameterInput(0, random)

    wallLight.addModule([random, rect, plotter])
    wallLight.setOutput(rect, 0)
    wallLight.run()
