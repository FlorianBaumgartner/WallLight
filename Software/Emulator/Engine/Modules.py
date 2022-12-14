import numpy as np

class Module():
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

        
class Function(Module):
    pixelcount = 0
    def __init__(self, id):
        super().__init__(id)
        self.inputs = []
        self.outputs = []

    def setInput(self, index, source, sourceIndex=0):
        self.inputs[index]["module"] = source
        self.inputs[index]["sourceIndex"] = sourceIndex

    
class Sine(Generator):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "amplitude", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "offset", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "phase", "module": None, "sourceIndex" : 0})   # -1 means -180° ... +1 means + 180°  
        
    def update(self, t):
        self.ready = all([i["module"].ready for i in self.parameterInputs])
        if not self.ready:
            return False
        
        freq = self.parameterInputs[0]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"]
        rep = self.parameterInputs[1]["module"].parameterOutputs[self.parameterInputs[1]["sourceIndex"]]["value"]
        amplitude = self.parameterInputs[2]["module"].parameterOutputs[self.parameterInputs[2]["sourceIndex"]]["value"]
        offset = self.parameterInputs[3]["module"].parameterOutputs[self.parameterInputs[3]["sourceIndex"]]["value"]
        phase = self.parameterInputs[4]["module"].parameterOutputs[self.parameterInputs[4]["sourceIndex"]]["value"]
        phase = (phase + 1.0) % 2.0 - 1.0
          
        output = 0.0
        if(rep == -1) or (rep / freq < t):
            x = t * 2 * np.pi * freq + (phase / freq) * np.pi
            output = np.sin(x) * amplitude + offset
        self.parameterOutputs[0]["value"] = output
        return True

class Pdf(Function):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "mean", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "var", "module": None, "sourceIndex" : 0})
        
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
    
    def update(self, t):
        parameterInputsReady = all([i["module"].ready for i in self.parameterInputs])
        inputsReady = all([i["module"].ready for i in self.inputs])
        self.ready = parameterInputsReady and inputsReady
        if not self.ready:
            return False
        
        mu = self.parameterInputs[0]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"] * Function.pixelcount
        sigma = np.sqrt(self.parameterInputs[1]["module"].parameterOutputs[self.parameterInputs[1]["sourceIndex"]]["value"])
        c = (1 / (sigma * np.sqrt(2 * np.pi))) * np.exp(-0.5 * ((np.arange(Function.pixelcount) - mu) / sigma)**2)
        c *= 1 / np.max(c)
        self.outputs[0]["value"] = np.vstack((c, c, c, c, c, c)).T                                            
        return True
    
class Multiplier(Function):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "r", "module": None, "sourceIndex" : 0})    # Red Channel
        self.parameterInputs.append({"name": "g", "module": None, "sourceIndex" : 0})    # Green Channel
        self.parameterInputs.append({"name": "b", "module": None, "sourceIndex" : 0})    # Blue Channel
        self.parameterInputs.append({"name": "ww", "module": None, "sourceIndex" : 0})   # Warm-White Channel
        self.parameterInputs.append({"name": "cw", "module": None, "sourceIndex" : 0})   # Cold-White Channel
        self.parameterInputs.append({"name": "am", "module": None, "sourceIndex" : 0})   # Amber Channel
        
        self.inputs.append({"name": "in", "module": None, "sourceIndex" : 0})
        self.outputs.append({"name": "out", "value": np.zeros((Function.pixelcount, 6))})
        
    def update(self, t):
        self.ready = all([i["module"].ready for i in self.inputs])
        if not self.ready:
            return False
        
        r = self.parameterInputs[0]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"]
        g = self.parameterInputs[1]["module"].parameterOutputs[self.parameterInputs[1]["sourceIndex"]]["value"]
        b = self.parameterInputs[2]["module"].parameterOutputs[self.parameterInputs[2]["sourceIndex"]]["value"]
        ww = self.parameterInputs[3]["module"].parameterOutputs[self.parameterInputs[3]["sourceIndex"]]["value"]
        cw = self.parameterInputs[4]["module"].parameterOutputs[self.parameterInputs[4]["sourceIndex"]]["value"]
        am = self.parameterInputs[5]["module"].parameterOutputs[self.parameterInputs[5]["sourceIndex"]]["value"]
        
        self.outputs[0]["value"] = self.inputs[0]["module"].outputs[self.inputs[0]["sourceIndex"]]["value"] * np.array([r, g, b, ww, cw, am])
        return True
                                                                            
