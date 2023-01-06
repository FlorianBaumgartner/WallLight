import numpy as np
import sys
sys.path.append("..")
from Modules import Function

class Rect(Function):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "start", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "stop", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "low", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "high", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "smooth", "module": None, "sourceIndex" : 0})
        
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
    
    def update(self, t):
        if not super().update(t):
            return False
        
        start = self.parameterInputs[0]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"]
        stop = self.parameterInputs[1]["module"].parameterOutputs[self.parameterInputs[1]["sourceIndex"]]["value"]
        low = self.parameterInputs[2]["module"].parameterOutputs[self.parameterInputs[2]["sourceIndex"]]["value"]
        high = self.parameterInputs[3]["module"].parameterOutputs[self.parameterInputs[3]["sourceIndex"]]["value"]
        smooth = self.parameterInputs[4]["module"].parameterOutputs[self.parameterInputs[4]["sourceIndex"]]["value"] >= 0.5
        
        if start > stop:
            start, stop = stop, start     # Swap high low values if necessary 
        
        c = np.ones(Function.pixelcount) * low
        for i in range(Function.pixelcount):
            if((i > int(start * Function.pixelcount)) and (i < int(stop * Function.pixelcount))):
                c[i] = high
            if smooth:
                startDif = start * Function.pixelcount - i
                stopDif = stop * Function.pixelcount - i
                if(startDif >= 0.0 and startDif < 1.0):
                    c[i] = low + (high - low) * startDif
                if(stopDif >= 0.0 and stopDif < 1.0):
                    c[i] = low + (high - low) * stopDif
        
        self.outputs[0]["value"] = np.vstack((c, c, c, c, c, c)).T                                            
        return True
    

if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient
    wallLight = WallLight()
    
    start = 0.025
    stop = 0.975
    low = 0.2
    high = 1.0
    smooth = 1.0
    
    rect = Rect(0)
    rect.setParameterInput(0, Coefficient(1, start))
    rect.setParameterInput(1, Coefficient(2, stop))
    rect.setParameterInput(2, Coefficient(3, low))
    rect.setParameterInput(3, Coefficient(4, high))
    rect.setParameterInput(4, Coefficient(5, smooth))
    
    wallLight.addModule(rect)
    wallLight.setOutput(rect, 0)
    wallLight.run()