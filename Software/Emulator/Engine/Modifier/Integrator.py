import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Module, Modifier


class Integrator(Modifier):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "input", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "gain", "module": None, "sourceIndex": 0, "default": 1.0})
        self.parameterInputs.append({"name": "min", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "max", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "reset", "module": None, "sourceIndex": 0, "default": 0.0})
        self.output = 0.0
        self.t = -1.0
        
    def update(self, t):
        if super().update(t) == False:
            return False
        if(self.t == t):
            return True
        if(self.t < 0.0):
            self.t = -1.0 / Module.framerate      
        
        inputValue = self._getParameterValue(0)
        gain = self._getParameterValue(1)
        minValue = self._getParameterValue(2)
        maxValue = self._getParameterValue(3)
        reset = self._getParameterValue(4) >= 0.5
        
        if self.parameterInputs[2]["module"]:   # Check if minimum value has been set
            self.output = max(self.output, minValue)
        if self.parameterInputs[3]["module"]:   # Check if maximum value has been set
            self.output = min(self.output, maxValue)
        if reset:
            self.output = 0.0
        
        self.parameterOutputs[0]["value"] = self.output
        self.output += inputValue * (gain * (t - self.t))
        self.t = t
        return True
    
if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Modifier, Analyzer, Function
    wallLight = WallLight()
    
    freq = 0.1
    
    inputValue = 1.0
    gain = 2.0 * np.pi * freq
    minValue = 0.5
    maxValue = 0.5
    reset = 0.0
    
    sine = Generator.Sine(0)
    sine.setParameterInput(1, Coefficient(1000, freq))        # freq
    sine.setParameterInput(5, Coefficient(1001, -0.5))        # phase
    
    integrator = Modifier.Integrator(1)
    integrator.setParameterInput(0, sine)
    integrator.setParameterInput(1, Coefficient(1002, gain))
    # integrator.setParameterInput(2, Coefficient(1003, minValue))
    # integrator.setParameterInput(3, Coefficient(1004, maxValue))
    # integrator.setParameterInput(4, Coefficient(1005, reset))

    dirac0 = Function.Dirac(2)
    dirac0.setParameterInput(0, sine)
    dirac0.setParameterInput(2, Coefficient(1006, 0.0))         # smooth

    dirac1 = Function.Dirac(3)
    dirac1.setParameterInput(0, integrator)
    dirac1.setParameterInput(2, Coefficient(1007, 0.0))         # smooth

    colorGain0 = Function.ColorGain(4)
    colorGain0.setParameterInput(0, Coefficient(1008, 1.0))     # red
    colorGain0.setParameterInput(1, Coefficient(1009, 0.0))     # green
    colorGain0.setParameterInput(2, Coefficient(1010, 0.0))     # blue
    colorGain0.setInput(0, dirac0)

    colorGain1 = Function.ColorGain(5)
    colorGain1.setParameterInput(0, Coefficient(1011, 0.0))     # red
    colorGain1.setParameterInput(1, Coefficient(1012, 1.0))     # green
    colorGain1.setParameterInput(2, Coefficient(1013, 0.0))     # blue
    colorGain1.setInput(0, dirac1)

    adder = Function.Adder(6)
    adder.setInput(0, colorGain0)
    adder.setInput(1, colorGain1)
    
    plotter = Analyzer.ParameterPlotter(5000, autoMove=False, stepMode=True)
    plotter.setParameterInput(0, sine, 0)
    plotter.setParameterInput(1, integrator, 0)

    wallLight.addModule([sine, integrator, dirac0, dirac1, colorGain0, colorGain1, adder, plotter])
    wallLight.setOutput(adder, 0)
    wallLight.run()
