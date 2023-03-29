import numpy as np
import sys
sys.path.append("..")
from Modules import Function

class Fire(Function):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "intensity", "module": None, "sourceIndex": 0, "default": 0.5})   # Intensity level of sparks
        self.parameterInputs.append({"name": "ignition", "module": None, "sourceIndex": 0, "default": 0.85})   # Probability of a spark each attempt
        self.parameterInputs.append({"name": "cooling", "module": None, "sourceIndex": 0, "default": 0.5})     # Rate at which the pixels cool off
        self.parameterInputs.append({"name": "speed", "module": None, "sourceIndex": 0, "default": 0.5})
        self.parameterInputs.append({"name": "acceleration", "module": None, "sourceIndex": 0, "default": 0.2})
        
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
        self.blendSelf = 1
        self.blendNeighbor1 = 0.4
        self.blendNeighbor2 = 0.2
        self.blendTotal = self.blendSelf + self.blendNeighbor1 * 2 + self.blendNeighbor2 * 2
        self.offset = 3
        self.heat = np.zeros(Function.pixelcount + self.offset)
        
    def heatMap(self, temperature):
        temperature = max(0.0, min(1.0, temperature)) * 3.0
        if(temperature > 2.0):
            r = 1.0
            g = 0.8
            b = (temperature % 1.0) * 0.5
        elif(temperature > 1.0):
            r = 1.0
            g = 0.4 + (temperature % 1.0) * 0.4
            b = 0.0
        else:
            r = temperature % 1.0
            g = (temperature % 1.0) * 0.4
            b = 0.0
        return r, g, b
    
    
    def addHeat(self, arr, pos, val):
        posLower = int(pos)
        posUpper = posLower + 1
        posOffset = pos % 1.0
        if(0 <= posLower < len(arr)):
            arr[posLower] += val * (1.0 - posOffset)
        if(0 <= posUpper < len(arr)):
            arr[posUpper] += val * posOffset
        
    
    def update(self, t):
        if not super().update(t):
            return False
        
        intensity = self._getParameterValue(0)               
        ignition = self._getParameterValue(1)              
        cooling = self._getParameterValue(2)
        speed = self._getParameterValue(3)
        acceleration = self._getParameterValue(4)
         
        for i in range(Function.pixelcount):
            self.heat[i] = max(0, self.heat[i] - np.random.uniform(0.0, (cooling * 10.0) / Function.pixelcount))
            
        temp = np.zeros_like(self.heat)
        for i in range(Function.pixelcount):
            self.addHeat(temp, i * (acceleration / 10.0 + 1.0) + 100.0 * (speed / Function.framerate), self.heat[i])

        self.heat[0] = temp[0]
        self.heat[1] = temp[1]
        self.heat[-1] = temp[-1]
        self.heat[-2] = temp[-2]
        for i in range(2, len(self.heat) - 2):
            self.heat[i] = (temp[i] * self.blendSelf +
                            temp[i + 1] * self.blendNeighbor1 +
                            temp[i - 1] * self.blendNeighbor1 +
                            temp[i + 2] * self.blendNeighbor2 + 
                            temp[i - 2] * self.blendNeighbor2) / self.blendTotal
        
        if(np.random.rand() < ignition):
            self.heat[0] += np.random.uniform(0.0, intensity * 10.0)
            
        for i in range(Function.pixelcount):
            r, g, b = self.heatMap(self.heat[i + self.offset])
            self.outputs[0]["value"][i][0] = r
            self.outputs[0]["value"][i][1] = g
            self.outputs[0]["value"][i][2] = b
                                     
        return True
    

if __name__ == '__main__':
    from pathlib import Path
    sys.path.append(str(Path(__file__).parent.parent.parent))
    from WallLight_Emulator import WallLight
    from Modules import Coefficient, Generator, Analyzer, Function
    wallLight = WallLight()
    # wallLight.setFramerate(50)
    
    intensity = 0.5             
    ignition = 0.85            
    cooling = 0.5
    speed = 0.5
    acceleration = 0.2

    fire = Function.Fire()
    fire.setParameterInput(0, Coefficient(intensity))
    fire.setParameterInput(1, Coefficient(ignition))
    fire.setParameterInput(2, Coefficient(cooling))
    fire.setParameterInput(3, Coefficient(speed))
    fire.setParameterInput(4, Coefficient(acceleration))
    
    wallLight.addModule([fire])
    wallLight.setOutput(fire, 0)
    wallLight.run()