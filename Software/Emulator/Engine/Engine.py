import os
import sys
import json
import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from Modules import Module, Coefficient, ColorVector, Generator, Modifier, Function, Analyzer


class Engine():
    def __init__(self, pixelcount, framerate):
        Module.pixelcount = pixelcount
        Module.framerate = framerate
        self.framerate = framerate
        self.pixels = np.zeros((pixelcount, 6))
        self.output = None
        self.outputIndex = 0
        self.modules = []
        self.t = 0.0
        self.graphName = None
        self.graphRevision = None
        
        
    def loadGraph(self, path):
        with open(path, "r") as f:
            data = json.load(f)
            self.graphName = data["name"]
            self.graphRevision = data["revision"]
            
            # Add all coefficients objects
            for coef in data["coefficients"]:
                self.modules.append(Coefficient(coef["id"], coef["value"]))
                
            # Add all module objects
            for module in data["modules"]:
                moduleType = classType = globals()[module["type"].split('.')[0]]
                if(len(module["type"].split('.')) > 1):
                    moduleType = getattr(classType, module["type"].split('.')[-1])
                self.modules.append(moduleType(module["id"]))
               
            # Iterate through all modules and set parameters and inputs
            for module in data["modules"]:
                if "parameter" in module:
                    for param in module["parameter"]:
                        m = Module.getModuleFromId(self.modules, module["id"])
                        m.setParameterInput(param["index"], Module.getModuleFromId(self.modules, param["id"]), param["output"])
                if "input" in module:
                    for inp in module["input"]:
                        m = Module.getModuleFromId(self.modules, module["id"])
                        m.setInput(inp["index"], Module.getModuleFromId(self.modules, inp["id"]), inp["output"])
             
            self.setOutput(Module.getModuleFromId(self.modules, data["output"]["id"]), data["output"]["output"])
     

    def setOutput(self, module, index):
        self.output = module
        self.outputIndex = index
        
    def addModule(self, module):
        if type(module) is not list: module = [module]
        for m in module:
            self.modules.append(m)
            
    def updateCoefficient(self, identity, value):
        m = Module.getModuleFromId(self.modules, identity)
        if not isinstance(m, Coefficient):
            raise Exception(f"ERROR: Module with ID [{identity}] is not a Coefficient")
        m.updateValue(value)
        
    def updateColorVector(self, identity, value):
        m = Module.getModuleFromId(self.modules, identity)
        if not isinstance(m, ColorVector):
            raise Exception(f"ERROR: Module with ID [{identity}] is not a ColorVector")
        m.updateValue(value)
        
    
    def getPixelData(self):
        self.t += 1 / self.framerate
        for m in self.modules:
            if hasattr(m, "isReady"):
                if not m.isReady():
                    self.t = 0
            
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
        if not self.output:
            raise Exception("ERROR: System output is not connected!")
        self.pixels = self.output.outputs[self.outputIndex]["value"]
        
    def end(self):
        for i in self.modules:
            i.end()


if __name__ == '__main__':
    from pathlib import Path
    
    engine = Engine(288, 60)
    engine.loadGraph(Path(__file__).parent.parent / "Graphs" / "test_graph.json")
    
    