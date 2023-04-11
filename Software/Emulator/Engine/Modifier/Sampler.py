import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Module, Modifier


class Sampler(Modifier):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "input", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "trigger", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "mode", "module": None, "sourceIndex": 0, "default": 0.0})
        self.modes = ["rising", "falling", "both"]
        self.output = 0.0
        self.trigger = False
        
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        inputValue = self._getParameterValue(0)
        trigger = self._getParameterValue(1) >= 0.5
        mode = self.modes[int(self._getParameterValue(2) + 0.5)]
        
        if mode == "rising":
            if not self.trigger and trigger:
                self.output = inputValue
        elif mode == "falling":
            if self.trigger and not trigger:
                self.output = inputValue
        elif mode == "both":
            if self.trigger != trigger:
                self.output = inputValue
        
        self.trigger = trigger
        self.parameterOutputs[0]["value"] = self.output
        return True


if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Modifier, Analyzer, Function
    wallLight = WallLight()
    
    mode = 1.0      # 0.0 = rising edge, 1.0 = falling edge, 2.0 = rising and falling edge
    
    sine = Generator.Sine(0)  
    rect = Generator.Rect(1)
    rect.setParameterInput(1, Coefficient(1000, 0.3))
    
    sampler = Modifier.Sampler(2)
    sampler.setParameterInput(0, sine)
    sampler.setParameterInput(1, rect)
    sampler.setParameterInput(2, Coefficient(1001, mode))

    dirac0 = Function.Dirac(3)
    dirac0.setParameterInput(0, sine)
    dirac0.setParameterInput(2, Coefficient(1003, 0.0))         # smooth

    dirac1 = Function.Dirac(4)
    dirac1.setParameterInput(0, sampler)
    dirac1.setParameterInput(2, Coefficient(1004, 0.0))         # smooth

    colorGain0 = Function.ColorGain(5)
    colorGain0.setParameterInput(0, Coefficient(1005, 1.0))     # red
    colorGain0.setParameterInput(1, Coefficient(1006, 0.0))     # green
    colorGain0.setParameterInput(2, Coefficient(1007, 0.0))     # blue
    colorGain0.setInput(0, dirac0)

    colorGain1 = Function.ColorGain(6)
    colorGain1.setParameterInput(0, Coefficient(1008, 0.0))     # red
    colorGain1.setParameterInput(1, Coefficient(1009, 1.0))     # green
    colorGain1.setParameterInput(2, Coefficient(1010, 0.0))     # blue
    colorGain1.setInput(0, dirac1)

    adder = Function.Adder(7)
    adder.setInput(0, colorGain0)
    adder.setInput(1, colorGain1)
    
    plotter = Analyzer.ParameterPlotter(5000)
    plotter.setParameterInput(0, sine, 0)
    plotter.setParameterInput(1, rect, 0)
    plotter.setParameterInput(2, sampler, 0)

    wallLight.addModule([sine, rect, sampler, dirac0, dirac1, colorGain0, colorGain1, adder, plotter])
    wallLight.setOutput(adder, 0)
    wallLight.run()
