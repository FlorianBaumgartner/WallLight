import numpy as np
import sys
sys.path.append("..")
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
    import time
    from Modules import Coefficient, Generator, Modifier, Analyzer
    Module.framerate = 60
    
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
    
    plotter = Analyzer.ParameterPlotter(5000, standalone=True)
    plotter.setParameterInput(0, sine, 0)
    plotter.setParameterInput(1, monoflop, 0)
    
    def update(t):
        sine.update(t)
        monoflop.update(t)
        plotter.update(t)

    plotter.updateFunction = update
    while plotter.isRunning():
        time.sleep(0.1)
        
