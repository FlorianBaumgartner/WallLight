import numpy as np
import sys
sys.path.append("..")
from Modules import Function


class ColorInsert(Function):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "pos", "module": None, "sourceIndex": 0, "default": 0.5})  # Position
        self.parameterInputs.append({"name": "r", "module": None, "sourceIndex": 0, "default": 0.0})    # Red Channel
        self.parameterInputs.append({"name": "g", "module": None, "sourceIndex": 0, "default": 0.0})    # Green Channel
        self.parameterInputs.append({"name": "b", "module": None, "sourceIndex": 0, "default": 0.0})    # Blue Channel
        self.parameterInputs.append({"name": "ww", "module": None, "sourceIndex": 0, "default": 0.0})   # Warm-White Channel
        self.parameterInputs.append({"name": "cw", "module": None, "sourceIndex": 0, "default": 0.0})   # Cold-White Channel
        self.parameterInputs.append({"name": "am", "module": None, "sourceIndex": 0, "default": 0.0})   # Amber Channel
        
        self.inputs.append({"name": "in", "module": None, "sourceIndex" : 0, "default": np.ones((Function.pixelcount, 6)) * (-1.0)})
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
    def update(self, t):
        if not super().update(t):
            return False
        
        pos = self._getParameterValue(0)
        r = self._getParameterValue(1)
        g = self._getParameterValue(2)
        b = self._getParameterValue(3)
        ww = self._getParameterValue(4)
        cw = self._getParameterValue(5)
        am = self._getParameterValue(6)
        
        self.outputs[0]["value"] = np.copy(self._getInput(0))
        if(0.0 <= pos <= 1.0):
            discretePos = int(pos * (Function.pixelcount - 1) + 0.5)
            self.outputs[0]["value"][discretePos] = np.array([r, g, b, ww, cw, am])
        return True
    

if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Analyzer, Function
    wallLight = WallLight()
    
    pos0 = 0.2
    pos1 = 0.5
    pos2 = 0.7
    
    plotChannel = 0.0
    
    colorInsert0 = Function.ColorInsert(0)
    colorInsert0.setParameterInput(0, Coefficient(1000, pos0))
    colorInsert0.setParameterInput(1, Coefficient(1001, 1.0))   # Red
    colorInsert0.setParameterInput(2, Coefficient(1001, 0.0))   # Green
    colorInsert0.setParameterInput(3, Coefficient(1002, 1.0))   # Blue
    
    colorInsert1 = Function.ColorInsert(1)
    colorInsert1.setParameterInput(0, Coefficient(1003, pos1))
    colorInsert1.setParameterInput(1, Coefficient(1004, 0.0))   # Red
    colorInsert1.setParameterInput(2, Coefficient(1005, 1.0))   # Green
    colorInsert1.setParameterInput(3, Coefficient(1006, 1.0))   # Blue
    colorInsert1.setInput(0, colorInsert0)
    
    colorInsert2 = Function.ColorInsert(2)
    colorInsert2.setParameterInput(0, Coefficient(1007, pos2))
    colorInsert2.setParameterInput(1, Coefficient(1008, 1.0))   # Red
    colorInsert2.setParameterInput(2, Coefficient(1009, 1.0))   # Green
    colorInsert2.setParameterInput(3, Coefficient(1010, 0.0))   # Blue
    colorInsert2.setInput(0, colorInsert1)

    
    inputPlotter = Analyzer.InputPlotter(5000)
    inputPlotter.setParameterInput(0, Coefficient(1011, plotChannel))
    inputPlotter.setInput(0, colorInsert2)
      
    wallLight.addModule([colorInsert0, colorInsert1, colorInsert2, inputPlotter])
    wallLight.setOutput(colorInsert2, 0)
    wallLight.run()