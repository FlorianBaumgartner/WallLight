import sys
import numpy as np
import os
from glob import glob

class Module():
    pixelcount = 0
    framerate = 0
    
    def getModuleFromId(modules, moduleId):
        for m in modules:
            if(m.id == moduleId):
                return m
        raise Exception(f"No module with ID: {moduleId} found!")
    
    def __init__(self, id):
        self.id = id
        self.ready = False
        self.parameterInputs = []
        self.parameterOutputs = []
    
    def update(self, t):
        return self.ready
    
    def end(self):
        pass
    
    def setParameterInput(self, index, source, sourceIndex=0):
        self.parameterInputs[index]["module"] = source
        self.parameterInputs[index]["sourceIndex"] = sourceIndex
    
class Coefficient(Module):
    def __init__(self, id, const):
        super().__init__(id)
        self.parameterOutputs.append({"name": "coefficient", "value": const})
        self.ready = True
    
    def updateValue(self, value):
        self.parameterOutputs[0]["value"] = value

class Generator(Module):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "enable", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "freq", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "rep", "module": None, "sourceIndex" : 0})  # -1 mean endless
        self.parameterOutputs.append({"name": "out", "value": 0.0})
        self.enableTime = 0.0
        
    def update(self, t):
        self.ready = all([i["module"].ready for i in self.parameterInputs])
        return self.ready
    
class Modifier(Module):
    def __init__(self, id):
        super().__init__(id)
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
        
    def update(self, t):
        parameterInputsReady = all([i["module"].ready for i in self.parameterInputs])
        inputsReady = all([i["module"].ready for i in self.inputs])
        self.ready = parameterInputsReady and inputsReady
        return self.ready
        
class Analyzer(Module):
    def __init__(self, id):
        super().__init__(id)
        self.inputs = []
        
    def setInput(self, index, source, sourceIndex=0):
        self.inputs[index]["module"] = source
        self.inputs[index]["sourceIndex"] = sourceIndex
        
    def update(self, t):
        parameterInputsReady = all([i["module"].ready for i in self.parameterInputs])
        inputsReady = all([i["module"].ready for i in self.inputs])
        self.ready = parameterInputsReady and inputsReady
        return self.ready


def addSubmodules(classRef, path):    
    import importlib.util
                                                                     
    submodules = []
    for file in glob(os.path.join(path, "*.py")):
        name = os.path.splitext(os.path.basename(file))[0]
        if(name == classRef.__name__):
            continue
        
        spec = importlib.util.spec_from_file_location(name, os.path.join(path, f"{name}.py"))
        foo = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(foo)
        module = getattr(foo, name)        
        submodules.append(module) if module not in submodules else submodules
            
    for m in submodules:
        setattr(classRef, m.__name__, m)
    return [m.__name__ for m in submodules]

addSubmodules(Generator, os.path.join(os.path.dirname(__file__), "Generator"))
addSubmodules(Function, os.path.join(os.path.dirname(__file__), "Function"))
addSubmodules(Modifier, os.path.join(os.path.dirname(__file__), "Modifier"))
addSubmodules(Analyzer, os.path.join(os.path.dirname(__file__), "Analyzer"))
