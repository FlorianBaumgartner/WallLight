import sys
import numpy as np
import os
from glob import glob

class Module():
    revision = "0.1"
    pixelcount = 0
    framerate = 0
    modules = []
    
    def getModuleFromId(modules, moduleId):
        for m in modules:
            if(m.id == moduleId):
                return m
        raise Exception(f"ERROR: No module with ID: {moduleId} found!")
    
    def __init__(self, id, printInfo=True):
        self.superClassType = self.__class__.superClassType     # This will be set in the addSubmodules function
        self.id = id
        self.ready = False
        self.printInfo = printInfo
        self.parameterInputs = []
        self.parameterOutputs = []
        
        if id is None:
            self.id = Iterator(str(self.superClassType))
            Module.modules.append(self)
    
    def update(self, t):
        inputsReady = True
        parameterInputsReady = True
        for n, i in enumerate(self.parameterInputs):
            if i["module"]:                                     # Check if parameter is connected to module output
                parameterInputsReady &= i["module"].ready
            elif self.printInfo:
                print(f'INFO: ParameterInput "{i["name"]}" [{n}] of "{self.superClassType}.{self.__module__}" (ID: {self.id}) uses default value: {i["default"]}')

        if hasattr(self, "inputs"):
            for n, i in enumerate(self.inputs):
                if i["module"]:                                 # Check if input is connected to module output
                    inputsReady &= i["module"].ready
                elif self.printInfo:
                    print(f'INFO: Input "{i["name"]}" [{n}] of "{self.superClassType}.{self.__module__}" (ID: {self.id}) uses default value: {i["default"][0][0]:.1f} vector')

        self.printInfo = False
        self.ready = parameterInputsReady and inputsReady
        return self.ready
    
    def stop(self):
        pass
    
    def setParameterInput(self, index, source, sourceIndex=0):
        if(source.id == self.id):
            raise Exception(f'ERROR: ParameterInput of Module "{self.superClassType}.{self.__module__}" (ID: {self.id}) must not be connected to itself')
        try:
            self.parameterInputs[index]["module"] = source
            self.parameterInputs[index]["sourceIndex"] = sourceIndex
        except IndexError:
            raise Exception(f'ERROR: ParameterInput [{index}] of Module "{self.superClassType}.{self.__module__}" (ID: {self.id}) does not exist (Out of range)')
        
    def _getParameterValue(self, index):
        if not self.parameterInputs[index]["module"]:
            return self.parameterInputs[index]["default"]
        return self.parameterInputs[index]["module"].parameterOutputs[self.parameterInputs[index]["sourceIndex"]]["value"]


class Coefficient(Module):
    superClassType = "Coefficient"
    def __init__(self, *args):
        id = None
        value = 0.0
        if len(args) == 1:
            value = args[0]
        if len(args) == 2:
            id = args[0]
            value = args[1]
            
        super().__init__(id)
        self.parameterOutputs = [{"name": "coefficient", "value": value}]
        self.ready = True
          
    def updateValue(self, value):
        self.parameterOutputs[0]["value"] = value
        
    def getValue(self):
        return self.parameterOutputs[0]["value"]


class ColorVector(Module):
    superClassType = "ColorVector"
    def __init__(self, *args):
        id = None
        vector = np.zeros((Module.pixelcount, 6))
        if len(args) == 1:
            vector = args[0]
        if len(args) == 2:
            id = args[0]
            vector = args[1]

        super().__init__(id, printInfo=False)
        self.outputs = [{"name": "out", "value": vector}]
        self.ready = True
        
    def updateValue(self, value):
        self.outputs[0]["value"] = value


class Generator(Module):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterInputs.append({"name": "enable", "module": None, "sourceIndex" : 0, "default": 1.0})
        self.parameterOutputs.append({"name": "out", "value": 0.0})
        self.enableTime = 0.0

    
class Modifier(Module):
    def __init__(self, id=None):
        super().__init__(id)
        self.parameterOutputs.append({"name": "out", "value": 0.0})

    
class Function(Module):
    def __init__(self, id=None):
        super().__init__(id)
        self.inputs = []
        self.outputs = []

    def setInput(self, index, source, sourceIndex=0):
        if(source.id == self.id):
            raise Exception(f'ERROR: Input of Module "{self.superClassType}.{self.__module__}" (ID: {self.id}) must not be connected to itself')
        self.inputs[index]["module"] = source
        self.inputs[index]["sourceIndex"] = sourceIndex
        
    def _getInput(self, index):
        if not self.inputs[index]["module"]:
            return self.inputs[index]["default"]
        return self.inputs[index]["module"].outputs[self.inputs[index]["sourceIndex"]]["value"]

        
class Analyzer(Module):
    def __init__(self, id=None):
        super().__init__(id, printInfo=False)
        self.inputs = []
        
    def setInput(self, index, source, sourceIndex=0):
        if(source.id == self.id):
            raise Exception(f'ERROR: Input of Module "{self.superClassType}.{self.__module__}" (ID: {self.id}) must not be connected to itself')
        self.inputs[index]["module"] = source
        self.inputs[index]["sourceIndex"] = sourceIndex
        
    def _getInput(self, index):
        if not self.inputs[index]["module"]:
            return self.inputs[index]["default"]
        return self.inputs[index]["module"].outputs[self.inputs[index]["sourceIndex"]]["value"]


moduleId = 0
coefficientId = 1000
AnalyzerId = 5000
class Iterator():
    def __new__(cls, moduleType):
        global moduleId, coefficientId, AnalyzerId
        if moduleType in ["Coefficient", "ColorVector"]:
            id = coefficientId
            coefficientId += 1
        elif moduleType == "Analyzer":
            id = AnalyzerId
            AnalyzerId += 1
        else:
            id = moduleId
            moduleId += 1
        return id


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
        setattr(module, "superClassType", classRef.__name__)
        submodules.append(module) if module not in submodules else submodules
            
    for m in submodules:
        setattr(classRef, m.__name__, m)
    return [m.__name__ for m in submodules]

thisFile = os.path.abspath(__file__)
addSubmodules(Generator, os.path.join(os.path.dirname(thisFile), "Generator"))
addSubmodules(Function, os.path.join(os.path.dirname(thisFile), "Function"))
addSubmodules(Modifier, os.path.join(os.path.dirname(thisFile), "Modifier"))
addSubmodules(Analyzer, os.path.join(os.path.dirname(thisFile), "Analyzer"))
