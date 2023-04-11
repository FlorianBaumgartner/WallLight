import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Modifier


class Switch(Modifier):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "input 0", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "input 1", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "switch", "module": None, "sourceIndex": 0, "default": 0.0})
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        input_0 = self._getParameterValue(0)
        input_1 = self._getParameterValue(1)
        switch = self._getParameterValue(2) >= 0.5
        
        self.parameterOutputs[0]["value"] = input_1 if switch else input_0     # 0 -> input_0, 1 -> input_1
        return True


if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Modifier, Analyzer, Function
    wallLight = WallLight()
    
    switch = Modifier.Mixer(0)
    switch.setParameterInput(0, Coefficient(1000, 0.2))            # input 0
    switch.setParameterInput(1, Coefficient(1001, 0.8))            # input 1
    switch.setParameterInput(2, Coefficient(1002, 0.0))            # switch

    rect = Function.Rect(1)
    rect.setParameterInput(0, Coefficient(1001, 0.0))              # start
    rect.setParameterInput(1, switch)                              # stop

    wallLight.addModule([switch, rect])
    wallLight.setOutput(rect, 0)
    wallLight.run()
