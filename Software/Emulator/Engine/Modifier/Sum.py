import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Modifier

class Sum(Modifier):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "input 0", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "input 1", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "input 2", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "input 3", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "input 4", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "input 5", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "input 6", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "input 7", "module": None, "sourceIndex": 0, "default": 0.0})
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        output = 0.0
        for i in range(8):
            output += self._getParameterValue(i)
        self.parameterOutputs[0]["value"] = output
        return True


if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Modifier, Analyzer, Function
    wallLight = WallLight()
    
    sumModule = Modifier.Sum(0)
    sumModule.setParameterInput(0, Coefficient(1000, 0.2))         # input 0
    sumModule.setParameterInput(1, Coefficient(1001, 0.3))         # input 1

    rect = Function.Rect(1)
    rect.setParameterInput(0, Coefficient(1002, 0.0))              # start
    rect.setParameterInput(1, sumModule)                           # stop

    wallLight.addModule([sumModule, rect])
    wallLight.setOutput(rect, 0)
    wallLight.run()
