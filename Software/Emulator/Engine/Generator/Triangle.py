import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Generator

class Triangle(Generator):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "freq", "module": None, "sourceIndex" : 0, "default": 1.0})
        self.parameterInputs.append({"name": "rep", "module": None, "sourceIndex" : 0, "default": -1.0})  # -1 mean endless
        self.parameterInputs.append({"name": "amplitude", "module": None, "sourceIndex" : 0, "default": 1.0})
        self.parameterInputs.append({"name": "offset", "module": None, "sourceIndex" : 0, "default": 0.0})
        self.parameterInputs.append({"name": "phase", "module": None, "sourceIndex" : 0, "default": 0.0})   # -1 means -180° ... +1 means + 180°  

        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        enable = self._getParameterValue(0) >= 0.5
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
        
        x = 1.0 - np.abs(((t * freq - (phase / 2.0)) % 1.0) * 2.0 - 1.0)
        output = (x - 0.5) * 2 * amplitude + offset
        self.parameterOutputs[0]["value"] = output
        return True


if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Modifier, Analyzer, Function
    wallLight = WallLight()
    
    triangle = Generator.Triangle(0)
    triangle.setParameterInput(0, Coefficient(1000, 1.0))           # enable
    triangle.setParameterInput(1, Coefficient(1001, 1.0))           # freq
    triangle.setParameterInput(2, Coefficient(1002, 1.0))           # rep
    triangle.setParameterInput(3, Coefficient(1003, -1.0))          # amp
    triangle.setParameterInput(4, Coefficient(1004, 0.0))           # offset
    triangle.setParameterInput(5, Coefficient(1005, 0.0))           # phase

    rect = Function.Rect(1)
    rect.setParameterInput(0, Coefficient(1006, 0.0))               # start
    rect.setParameterInput(1, triangle)                             # stop

    plotter = Analyzer.ParameterPlotter(2)
    plotter.setParameterInput(0, triangle)

    wallLight.addModule([triangle, rect, plotter])
    wallLight.setOutput(rect, 0)
    wallLight.run()
