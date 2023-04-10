import os
import sys
import numpy as np
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))
from Modules import Module, Modifier


class Monoflop(Modifier):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "input", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "trigger level", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "pulse length", "module": None, "sourceIndex": 0, "default": 0.0})
        self.parameterInputs.append({"name": "retrigger", "module": None, "sourceIndex": 0, "default": 1.0})
        self.parameterInputs.append({"name": "mode", "module": None, "sourceIndex": 0, "default": 0.0})
        self.modes = [">", "<", "<>"]
        self.trigger = False
        self.triggerActiveTime = -1.0
        self.inputValue = 0.0
        
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        inputValue = self._getParameterValue(0)
        triggerLevel = self._getParameterValue(1)
        pulseLength = self._getParameterValue(2)
        retrigger = self._getParameterValue(3) >= 0.5
        mode = self.modes[int(self._getParameterValue(4) + 0.5)]
        
        trigger = False
        if mode == ">" and (inputValue > triggerLevel):
            trigger = True
        elif mode == "<" and (inputValue < triggerLevel):
            trigger = True
        elif mode == "<>":
            if(inputValue > triggerLevel) and (self.inputValue < triggerLevel):
                trigger = True
            if(inputValue < triggerLevel) and (self.inputValue > triggerLevel):
                trigger = True
            
        if((trigger and not self.trigger) and (self.triggerActiveTime < t)) or retrigger:
            self.triggerActiveTime = t + pulseLength
        self.trigger = trigger
        self.inputValue = inputValue
        
        output = 1.0 if(self.triggerActiveTime >= t) else 0.0  
        self.parameterOutputs[0]["value"] = output
        return True

if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Modifier, Analyzer, Function
    wallLight = WallLight()
    
    triggerLevel = 0.8
    pulseLength = 0.2
    retrigger = 0.0
    mode = 0.0      # 0.0 = input > triggerLevel, 1.0 = input < triggerLevel, 2.0 = input has crossed triggerLevel
    
    sine = Generator.Sine(0)
    
    monoflop = Modifier.Monoflop(1)
    monoflop.setParameterInput(0, sine)
    monoflop.setParameterInput(1, Coefficient(1000, triggerLevel))
    monoflop.setParameterInput(2, Coefficient(1001, pulseLength))
    monoflop.setParameterInput(3, Coefficient(1002, retrigger))
    monoflop.setParameterInput(4, Coefficient(1003, mode))

    dirac0 = Function.Dirac(2)
    dirac0.setParameterInput(0, sine)
    dirac0.setParameterInput(2, Coefficient(1004, 0.0))         # smooth

    dirac1 = Function.Dirac(3)
    dirac1.setParameterInput(0, monoflop)
    dirac1.setParameterInput(2, Coefficient(1005, 0.0))         # smooth

    colorGain0 = Function.ColorGain(4)
    colorGain0.setParameterInput(0, Coefficient(1006, 1.0))     # red
    colorGain0.setParameterInput(1, Coefficient(1007, 0.0))     # green
    colorGain0.setParameterInput(2, Coefficient(1008, 0.0))     # blue
    colorGain0.setInput(0, dirac0)

    colorGain1 = Function.ColorGain(5)
    colorGain1.setParameterInput(0, Coefficient(1009, 0.0))     # red
    colorGain1.setParameterInput(1, Coefficient(1010, 1.0))     # green
    colorGain1.setParameterInput(2, Coefficient(1011, 0.0))     # blue
    colorGain1.setInput(0, dirac1)

    adder = Function.Adder(6)
    adder.setInput(0, colorGain0)
    adder.setInput(1, colorGain1)
    
    plotter = Analyzer.ParameterPlotter(5000)
    plotter.setParameterInput(0, sine, 0)
    plotter.setParameterInput(1, monoflop, 0)

    wallLight.addModule([sine, monoflop, dirac0, dirac1, colorGain0, colorGain1, adder, plotter])
    wallLight.setOutput(adder, 0)
    wallLight.run()
