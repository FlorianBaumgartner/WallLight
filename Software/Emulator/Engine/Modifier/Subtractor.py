import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Modifier

class Subtractor(Modifier):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "minuend", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "subtrahend", "module": None, "sourceIndex": 0, "default": 0.0})
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        minuend = self._getParameterValue(0)
        subtrahend = self._getParameterValue(1)
        
        self.parameterOutputs[0]["value"] = minuend - subtrahend
        return True
    

if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Modifier, Analyzer, Function
    wallLight = WallLight()
    
    subtractor = Modifier.Subtractor(0)
    subtractor.setParameterInput(0, Coefficient(1000, 0.8))        # minuend
    subtractor.setParameterInput(1, Coefficient(1001, 0.3))        # subtrahend

    rect = Function.Rect(1)
    rect.setParameterInput(0, Coefficient(1002, 0.0))              # start
    rect.setParameterInput(1, subtractor)                          # stop

    wallLight.addModule([subtractor, rect])
    wallLight.setOutput(rect, 0)
    wallLight.run()