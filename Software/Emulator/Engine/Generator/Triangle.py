import numpy as np
import sys
sys.path.append("..")
from Modules import Generator

class Triangle(Generator):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "amplitude", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "offset", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "phase", "module": None, "sourceIndex" : 0})   # -1 means -180° ... +1 means + 180°  

        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        freq = self.parameterInputs[0]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"]
        rep = self.parameterInputs[1]["module"].parameterOutputs[self.parameterInputs[1]["sourceIndex"]]["value"]
        amplitude = self.parameterInputs[2]["module"].parameterOutputs[self.parameterInputs[2]["sourceIndex"]]["value"]
        offset = self.parameterInputs[3]["module"].parameterOutputs[self.parameterInputs[3]["sourceIndex"]]["value"]
        phase = self.parameterInputs[4]["module"].parameterOutputs[self.parameterInputs[4]["sourceIndex"]]["value"]
        phase = (phase + 1.0) % 2.0 - 1.0
          
        if(rep >= 0) and ((rep / freq) < t):            # End value always corresponds to t0
            t = 0
        x = 1.0 - np.abs(((t * freq - (phase / freq)) % 1.0) * 2.0 - 1.0)
        output = (x - 0.5) * 2 * amplitude + offset
        self.parameterOutputs[0]["value"] = output
        return True
    
if __name__ == '__main__':
    import time
    from Modules import Module, Coefficient, Analyzer
    Module.framerate = 60
        
    freq = 1
    rep = 1
    amp = 1.0
    offset = 0.0
    phase = -0.25
    
    triangle = Triangle(0)
    triangle.setParameterInput(0, Coefficient(0, freq))
    triangle.setParameterInput(1, Coefficient(1, rep))
    triangle.setParameterInput(2, Coefficient(2, amp))
    triangle.setParameterInput(3, Coefficient(3, offset))
    triangle.setParameterInput(4, Coefficient(4, phase))
    
    plotter = Analyzer.ParameterPlotter(1, standalone=True)
    plotter.setParameterInput(0, triangle, 0)
    
    def update(t):
        triangle.update(t)
        plotter.update(t)

    plotter.updateFunction = update
    while plotter.isRunning():
        time.sleep(0.1)
    