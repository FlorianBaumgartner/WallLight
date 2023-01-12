import numpy as np
import sys
sys.path.append("..")
from Modules import Generator

class Sine(Generator):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "amplitude", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "offset", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "phase", "module": None, "sourceIndex" : 0})   # -1 means -180° ... +1 means + 180°  
        
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        enable = self.parameterInputs[0]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"]
        freq = self.parameterInputs[1]["module"].parameterOutputs[self.parameterInputs[1]["sourceIndex"]]["value"]
        rep = self.parameterInputs[2]["module"].parameterOutputs[self.parameterInputs[2]["sourceIndex"]]["value"]
        amplitude = self.parameterInputs[3]["module"].parameterOutputs[self.parameterInputs[3]["sourceIndex"]]["value"]
        offset = self.parameterInputs[4]["module"].parameterOutputs[self.parameterInputs[4]["sourceIndex"]]["value"]
        phase = self.parameterInputs[5]["module"].parameterOutputs[self.parameterInputs[5]["sourceIndex"]]["value"]
        phase = (phase + 1.0) % 2.0 - 1.0
          
        
        if enable:
            t -= self.enableTime
            if(rep >= 0) and ((rep / freq) < t):            # End value always corresponds to t0
                t = 0
        else:
            self.enableTime = t
            t = 0
            
        x = t * 2 * np.pi * freq - (phase / freq) * np.pi
        output = np.sin(x) * amplitude + offset
        self.parameterOutputs[0]["value"] = output
        return True
    
if __name__ == '__main__':
    import time
    from Modules import Module, Generator, Coefficient, Analyzer
    Module.framerate = 60
        
    enable = 0.0
    freq = 1.0
    rep = 1.0
    amp = 0.5
    offset = 0.5
    phase = -0.5
    
    enableCoeff = Coefficient(2, enable)
    
    sine = Generator.Sine(0)
    sine.setParameterInput(0, enableCoeff)
    sine.setParameterInput(1, Coefficient(3, freq))
    sine.setParameterInput(2, Coefficient(4, rep))
    sine.setParameterInput(3, Coefficient(5, amp))
    sine.setParameterInput(4, Coefficient(6, offset))
    sine.setParameterInput(5, Coefficient(7, phase))
    
    plotter = Analyzer.ParameterPlotter(1, standalone=True)
    plotter.setParameterInput(0, sine, 0)
    
    def update(t):
        enableCoeff.updateValue(1.0 if((t % 4.0) >= 2.0) else 0.0)
        sine.update(t)
        plotter.update(t)

    plotter.updateFunction = update
    while plotter.isRunning():
        time.sleep(0.1)
    