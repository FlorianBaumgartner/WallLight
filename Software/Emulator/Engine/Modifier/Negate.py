import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Modifier

class Negate(Modifier):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "input", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "enable", "module": None, "sourceIndex": 0, "default": 1.0})
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        inputValue = self._getParameterValue(0)
        enable = self._getParameterValue(1) >= 0.5
        
        self.parameterOutputs[0]["value"] = inputValue * (-1.0) if enable else inputValue
        return True
    

if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Modifier, Analyzer, Function
    wallLight = WallLight()
    
    negate = Modifier.Negate(0)
    negate.setParameterInput(0, Coefficient(1000, -0.5))        # input
    negate.setParameterInput(1, Coefficient(1001, 1.0))         # enable

    rect = Function.Rect(1)
    rect.setParameterInput(0, Coefficient(1002, 0.0))           # start
    rect.setParameterInput(1, negate)                           # stop

    wallLight.addModule([negate, rect])
    wallLight.setOutput(rect, 0)
    wallLight.run()
