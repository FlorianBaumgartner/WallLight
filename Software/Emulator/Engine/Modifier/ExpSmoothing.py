import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Module, Modifier


class ExpSmoothing(Modifier):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "input", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "alpha", "module": None, "sourceIndex": 0, "default": 0.5})
        self.output = 0.0
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        inputValue = self._getParameterValue(0)
        alpha = (np.clip(self._getParameterValue(1), 0.0, 1.0)**0.1) * 0.999
        
        self.output = alpha * self.output + (1.0 - alpha) * inputValue
        self.parameterOutputs[0]["value"] = self.output
        return True
        

if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Modifier, Analyzer, Function
    wallLight = WallLight()
    
    alpha = 0.5
    
    noise = Generator.Random(0)
    
    smoothing = Modifier.ExpSmoothing(1)
    smoothing.setParameterInput(0, noise)
    smoothing.setParameterInput(1, Coefficient(1000, alpha))

    dirac0 = Function.Dirac(2)
    dirac0.setParameterInput(0, noise)
    dirac0.setParameterInput(2, Coefficient(1003, 0.0))         # smooth

    dirac1 = Function.Dirac(3)
    dirac1.setParameterInput(0, smoothing)
    dirac1.setParameterInput(2, Coefficient(1004, 0.0))         # smooth

    colorGain0 = Function.ColorGain(4)
    colorGain0.setParameterInput(0, Coefficient(1005, 1.0))     # red
    colorGain0.setParameterInput(1, Coefficient(1006, 0.0))     # green
    colorGain0.setParameterInput(2, Coefficient(1007, 0.0))     # blue
    colorGain0.setInput(0, dirac0)

    colorGain1 = Function.ColorGain(5)
    colorGain1.setParameterInput(0, Coefficient(1008, 0.0))     # red
    colorGain1.setParameterInput(1, Coefficient(1009, 1.0))     # green
    colorGain1.setParameterInput(2, Coefficient(1010, 0.0))     # blue
    colorGain1.setInput(0, dirac1)

    adder = Function.Adder(6)
    adder.setInput(0, colorGain0)
    adder.setInput(1, colorGain1)
    
    plotter = Analyzer.ParameterPlotter(5000)
    plotter.setParameterInput(0, noise, 0)
    plotter.setParameterInput(1, smoothing, 0)

    wallLight.addModule([noise, smoothing, dirac0, dirac1, colorGain0, colorGain1, adder, plotter])
    wallLight.setOutput(adder, 0)
    wallLight.run()
