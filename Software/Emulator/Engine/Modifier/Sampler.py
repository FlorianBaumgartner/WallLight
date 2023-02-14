import numpy as np
import sys
sys.path.append("..")
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
    import time
    from Modules import Coefficient, Generator, Modifier, Analyzer
    Module.framerate = 60
    
    mode = 1.0      # 0.0 = rising edge, 1.0 = falling edge, 2.0 = rising and falling edge
    
    sine = Generator.Sine(0)  
    rect = Generator.Rect(1)
    rect.setParameterInput(1, Coefficient(1000, 0.3))
    
    sampler = Modifier.Sampler(2)
    sampler.setParameterInput(0, sine)
    sampler.setParameterInput(1, rect)
    sampler.setParameterInput(2, Coefficient(1001, mode))
    
    plotter = Analyzer.ParameterPlotter(5000, standalone=True)
    plotter.setParameterInput(0, sine, 0)
    plotter.setParameterInput(1, rect, 0)
    plotter.setParameterInput(2, sampler, 0)
    
    def update(t):
        sine.update(t)
        rect.update(t)
        sampler.update(t)
        plotter.update(t)

    plotter.updateFunction = update
    while plotter.isRunning():
        time.sleep(0.1)
        
