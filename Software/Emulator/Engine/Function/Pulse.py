import numpy as np
import sys
sys.path.append("..")
from Modules import Function

class Pulse(Function):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "position", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "width", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "low", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "high", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "smooth", "module": None, "sourceIndex" : 0})
        
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
    
    def update(self, t):
        if not super().update(t):
            return False
        
        position = self.parameterInputs[0]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"]
        width = self.parameterInputs[1]["module"].parameterOutputs[self.parameterInputs[1]["sourceIndex"]]["value"]
        low = self.parameterInputs[2]["module"].parameterOutputs[self.parameterInputs[2]["sourceIndex"]]["value"]
        high = self.parameterInputs[3]["module"].parameterOutputs[self.parameterInputs[3]["sourceIndex"]]["value"]
        smooth = self.parameterInputs[4]["module"].parameterOutputs[self.parameterInputs[4]["sourceIndex"]]["value"] >= 0.5
        
        start = position - width / 2.0
        stop = position + width / 2.0
        
        c = np.ones(Function.pixelcount) * low
        for i in range(Function.pixelcount):
            if(((i + 0.5) > start * Function.pixelcount) and ((i + 0.5) <= stop * Function.pixelcount)):
                c[i] = high
            if smooth:
                startDif = start * Function.pixelcount - i
                stopDif = stop * Function.pixelcount - i
                if(startDif > 0.0 and startDif < 1.0):
                    c[i] = low + (high - low) * startDif
                if(stopDif >= 0.0 and stopDif < 1.0):
                    c[i] = low + (high - low) * stopDif
        
        self.outputs[0]["value"] = np.vstack((c, c, c, c, c, c)).T                                            
        return True
    

if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Analyzer
    wallLight = WallLight()
    
    freq = 0.1
    rep = -1.0
    amplitude = 0.5
    offset = 0.5
    phase = 0.0
    
    position = 0.5
    low = 0.0
    high = 1.0
    smooth = 1.0
    
    plotChannel = 0.0
    
    triangle = Generator.Triangle(0)
    triangle.setParameterInput(0, Coefficient(2, freq))
    triangle.setParameterInput(1, Coefficient(3, rep))
    triangle.setParameterInput(2, Coefficient(4, amplitude))
    triangle.setParameterInput(3, Coefficient(5, offset))
    triangle.setParameterInput(4, Coefficient(6, phase))
    
    
    pulse = Pulse(1)
    pulse.setParameterInput(0, Coefficient(7, position))
    pulse.setParameterInput(1, triangle)
    pulse.setParameterInput(2, Coefficient(8, low))
    pulse.setParameterInput(3, Coefficient(9, high))
    pulse.setParameterInput(4, Coefficient(10, smooth))
    
    parameterPlotter = Analyzer.ParameterPlotter(11)
    parameterPlotter.setParameterInput(0, triangle)
    
    inputPlotter = Analyzer.InputPlotter(12)
    inputPlotter.setParameterInput(0, Coefficient(13, plotChannel))
    inputPlotter.setInput(0, pulse)
    
    
    wallLight.addModule([triangle, pulse, parameterPlotter, inputPlotter])
    wallLight.setOutput(pulse, 0)
    wallLight.run()