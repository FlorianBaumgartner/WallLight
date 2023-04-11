import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Module, Generator

class Dirac(Generator):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "freq", "module": None, "sourceIndex" : 0, "default": 1.0})
        self.parameterInputs.append({"name": "rep", "module": None, "sourceIndex" : 0, "default": -1.0})  # -1 mean endless
        self.parameterInputs.append({"name": "weight", "module": None, "sourceIndex" : 0, "default": 1.0})
        self.parameterInputs.append({"name": "offset", "module": None, "sourceIndex" : 0, "default": 0.0})
        self.parameterInputs.append({"name": "phase", "module": None, "sourceIndex" : 0, "default": 0.0})   # -1 means -180° ... +1 means + 180°  
        self.periodeCount = 0
        self.repetitionCount = 0

        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        enable = self._getParameterValue(0) >= 0.5
        freq = max(self._getParameterValue(1), 0.0001)
        rep = self._getParameterValue(2)
        weight = self._getParameterValue(3)
        offset = self._getParameterValue(4)
        phase = self._getParameterValue(5)
        phase = (phase + 1.0) % 2.0 - 1.0
        if phase < 0:
            phase += 2
        
        periode = 1.0 / freq
        phaseDelay = -(phase * 0.5) / freq

        output = offset
        if not enable:
            self.enableTime = t
            self.periodeCount = 0
            self.repetitionCount = 0
            t = 0
        else:
            t -= self.enableTime
            if(t == 0 and phaseDelay > 0):
                self.periodeCount += 1
            if(t + phaseDelay >= periode * self.periodeCount):
                self.periodeCount += 1
                if((rep < 0.0) or self.repetitionCount < rep):
                    output += weight
                    self.repetitionCount += 1           # TODO: Check if negative pahse should result in 1 less repetition (first is unser t=0.0)

        self.parameterOutputs[0]["value"] = output
        return True
    

if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Modifier, Analyzer, Function
    wallLight = WallLight()
    
    dirac = Generator.Dirac(0)
    dirac.setParameterInput(0, Coefficient(1000, 1.0))           # enable
    dirac.setParameterInput(1, Coefficient(1001, 1.0))           # freq
    dirac.setParameterInput(2, Coefficient(1002, -1.0))          # rep
    dirac.setParameterInput(3, Coefficient(1003, 1.0))           # weight
    dirac.setParameterInput(4, Coefficient(1004, 0.0))           # offset
    dirac.setParameterInput(5, Coefficient(1005, 0.0))           # phase

    rect = Function.Rect(1)
    rect.setParameterInput(0, Coefficient(1006, 0.0))            # start
    rect.setParameterInput(1, dirac)                             # stop

    plotter = Analyzer.ParameterPlotter(2, autoMove=False, stepMode=True)
    plotter.setParameterInput(0, dirac)

    wallLight.addModule([dirac, rect, plotter])
    wallLight.setOutput(rect, 0)
    wallLight.run()
