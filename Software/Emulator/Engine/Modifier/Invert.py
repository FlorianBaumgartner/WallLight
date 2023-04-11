import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Modifier

class Invert(Modifier):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "input", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "enable", "module": None, "sourceIndex": 0, "default": 1.0})
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        inputValue = np.clip(self._getParameterValue(0), 0.0, 1.0)
        enable = self._getParameterValue(1) >= 0.5
        
        self.parameterOutputs[0]["value"] = 1.0 - inputValue if enable else inputValue
        return True
    

if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Modifier, Analyzer, Function
    wallLight = WallLight()
    
    invert = Modifier.Invert(0)
    invert.setParameterInput(0, Coefficient(1000, 0.2))         # input
    invert.setParameterInput(1, Coefficient(1001, 1.0))         # enable

    rect = Function.Rect(1)
    rect.setParameterInput(0, Coefficient(1002, 0.0))           # start
    rect.setParameterInput(1, invert)                           # stop

    wallLight.addModule([invert, rect])
    wallLight.setOutput(rect, 0)
    wallLight.run()
