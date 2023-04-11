import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Modifier

class Constrain(Modifier):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "input", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "min", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "max", "module": None, "sourceIndex": 0, "default": 0.0})
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        output = self._getParameterValue(0)
        minimum = self._getParameterValue(1)
        maximum = self._getParameterValue(2)
        
        if self._checkParameterValid(1) and self._checkParameterValid(2):  # When both inputs are availabe swap min and max value if they are reversed
            if(minimum > maximum):
                temp = minimum
                minimum = maximum
                maximum = temp
        
        if self._checkParameterValid(1):        # Check if minimum value has been set  
            output = max(output, minimum)
        if self._checkParameterValid(2):        # Check if maximum value has been set
            output = min(output, maximum)
        
        self.parameterOutputs[0]["value"] = output
        return True


if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Modifier, Analyzer, Function
    wallLight = WallLight()

    sine = Generator.Sine(0)
    sine.setParameterInput(3, Coefficient(1000, 0.5))              # amplitude
    sine.setParameterInput(4, Coefficient(1001, 0.5))              # offset

    constrainModule = Modifier.Constrain(1)
    constrainModule.setParameterInput(0, sine)                     # input
    constrainModule.setParameterInput(1, Coefficient(1002, 0.3))   # minimum
    constrainModule.setParameterInput(2, Coefficient(1003, 0.7))   # maximum

    rect = Function.Rect(2)
    rect.setParameterInput(0, Coefficient(1004, 0.0))              # start
    rect.setParameterInput(1, constrainModule)                     # stop

    wallLight.addModule([sine, constrainModule, rect])
    wallLight.setOutput(rect, 0)
    wallLight.run()
