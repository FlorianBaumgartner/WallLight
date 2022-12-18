import sys
import numpy as np
import os
from glob import glob

class Module():
    pixelcount = 0
    def __init__(self, id):
        self.id = id
        self.ready = False
        self.parameterInputs = []
        self.parameterOutputs = []
    
    def update(self, t):
        return self.ready
    
    def setParameterInput(self, index, source, sourceIndex=0):
        self.parameterInputs[index]["module"] = source
        self.parameterInputs[index]["sourceIndex"] = sourceIndex
    
class Coefficient(Module):
    def __init__(self, id, const):
        super().__init__(id)
        self.parameterOutputs.append({"name": "coefficient", "value": const})
        self.ready = True
    # TODO: Add possibitlity to update coefficient (interface)

class Generator(Module):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "freq", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "rep", "module": None, "sourceIndex" : 0})  # -1 mean endless
        self.parameterOutputs.append({"name": "out", "value": 0.0})
        
    def update(self, t):
        self.ready = all([i["module"].ready for i in self.parameterInputs])
        return self.ready
    
class Function(Module):
    def __init__(self, id):
        super().__init__(id)
        self.inputs = []
        self.outputs = []

    def setInput(self, index, source, sourceIndex=0):
        self.inputs[index]["module"] = source
        self.inputs[index]["sourceIndex"] = sourceIndex


def addSubmodules(classRef, path):                                                                         
    submodules = []
    for file in glob(os.path.join(path, "*.py")):
        name = os.path.splitext(os.path.basename(file))[0]
        if(name == classRef.__name__):
            continue
        sys.path.append(path) 
        module = __import__(name)        
        for member in dir(module):
            try:
                attr = getattr(module, member)
                attr.update
                if(member in ["Module", "Generator", "Modifier"]):
                    continue
                if(member.startswith("__")):
                    continue
                submodules.append(attr) if attr not in submodules else submodules
            except:
                pass
    for m in submodules:
        setattr(classRef, m.__name__, m)
    return [m.__name__ for m in submodules]

addSubmodules(Generator, os.path.join(os.path.dirname(__file__), "Generator"))
addSubmodules(Function, os.path.join(os.path.dirname(__file__), "Function"))
# addSubmodules(Modifier, os.path.join(os.path.dirname(__file__), "Modifier"))

