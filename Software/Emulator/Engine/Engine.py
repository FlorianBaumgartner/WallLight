import os
import sys
import json
import numpy as np

sys.path.insert(0, os.path.dirname(__file__))
from Modules import Module, Coefficient, Generator, Function


class Engine():
    def __init__(self, pixelcount, framerate):
        Module.pixelcount = pixelcount
        self.framerate = framerate
        self.pixels = np.zeros((pixelcount, 6))
        self.output = None
        self.outputIndex = 0
        self.t = 0.0
        
        
    def loadGraph(self, path):
        with open(path, "r") as f:
            data = json.load(f)
            name = data["name"]
            revision = data["revision"]
            self.modules = []
            
            # Add all coefficients objects
            for coef in data["coefficients"]:
                self.modules.append(Coefficient(coef["id"], coef["value"]))
                
            # Add all module objects
            for module in data["modules"]:
                classType = globals()[module["type"].split('.')[0]]
                moduleType = getattr(classType, module["type"].split('.')[-1])
                self.modules.append(moduleType(module["id"]))
               
            # Iterate throu all modules and set parameters and inputs
            for module in data["modules"]:
                if "parameter" in module:
                    for i, param in enumerate(module["parameter"]):
                        m = Module.getModuleFromId(self.modules, module["id"])
                        m.setParameterInput(i, Module.getModuleFromId(self.modules, param["id"]), param["index"])
                if "input" in module:
                    for i, param in enumerate(module["input"]):
                        m = Module.getModuleFromId(self.modules, module["id"])
                        m.setInput(i, Module.getModuleFromId(self.modules, param["id"]), param["index"])
                    
            self.output = Module.getModuleFromId(self.modules, data["output"]["id"])
            self.outputIndex = data["output"]["index"]
        
    
    def getPixelData(self):
        self.t += 1 / self.framerate
        self.update(self.t)
        return self.pixels
    
    def update(self, t):
        while True:
            allReady = True
            for m in self.modules:
                res = m.update(t)
                allReady = allReady and res
            if allReady:
                break

        self.pixels = self.output.outputs[self.outputIndex]["value"]


if __name__ == '__main__':
    from pathlib import Path
    
    engine = Engine(288, 60)
    engine.loadGraph(Path(__file__).parent.parent / "Graphs" / "test_graph.json")
    
    