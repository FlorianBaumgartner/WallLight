import numpy as np
import sys
sys.path.append("..")
from Modules import Modifier

class EaseIn(Modifier):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "input", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "type", "module": None, "sourceIndex" : 0})

        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        x = self.parameterInputs[0]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"]
        easeType = int(self.parameterInputs[1]["module"].parameterOutputs[self.parameterInputs[1]["sourceIndex"]]["value"] + 0.5)
        
        output = 0
        if(x >= 1.0):
            output = 1.0
        elif(x <= 0.0):
            output = 0.0
        else:
            if easeType == 0:       # Sine
                output = 1 - np.cos((x * np.pi) / 2.0)
            elif easeType == 1:     # Cubic
                pass
            elif easeType == 2:     # Quint
                pass
            elif easeType == 3:     # Circ
                pass
            elif easeType == 4:     # Elastic
                pass
            elif easeType == 5:     # Quad
                pass
            elif easeType == 6:     # Quart
                pass
            elif easeType == 7:     # Expo
                pass
            elif easeType == 8:     # Back
                pass
            elif easeType == 9:     # Bounce
                output = 2**(6 * (x - 1)) * np.abs(np.sin(x * np.pi * 3.612)) / 0.939
            else:
                raise Exception("Easy-Function type out of bound!")
            
        self.parameterOutputs[0]["value"] = output
        return True


if __name__ == '__main__':
    import time
    from Modules import Module, Coefficient, Generator, Analyzer
    Module.framerate = 60
    
    freq = 0.1
    rep = 1
    start = 0
    stop = 1.0
    phase = 0.0
    
    easyType = 9.0
    
    ramp = Generator.Ramp(0)
    ramp.setParameterInput(0, Coefficient(3, freq))
    ramp.setParameterInput(1, Coefficient(4, rep))
    ramp.setParameterInput(2, Coefficient(5, start))
    ramp.setParameterInput(3, Coefficient(6, stop))
    ramp.setParameterInput(4, Coefficient(7, phase))
    
    easeIn = EaseIn(1)
    easeIn.setParameterInput(0, ramp)
    easeIn.setParameterInput(1, Coefficient(8, easyType))
    
    plotter = Analyzer.ParameterPlotter(2, autoMove=False, standalone=True)
    plotter.setParameterInput(0, easeIn, 0)
    
    def update(t):
        ramp.update(t)
        easeIn.update(t)
        plotter.update(t)

    plotter.updateFunction = update
    while plotter.isRunning():
        time.sleep(0.1)