import numpy as np
import sys
sys.path.append("..")
from Modules import Generator

class Ramp(Generator):
    def __init__(self, id):
        super().__init__(id)
        self.parameterInputs.append({"name": "start", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "stop", "module": None, "sourceIndex" : 0})
        self.parameterInputs.append({"name": "phase", "module": None, "sourceIndex" : 0})   # -1 means -180° ... +1 means + 180°  
        
    def update(self, t):
        if super().update(t) == False:
            return False
        
        freq = self.parameterInputs[0]["module"].parameterOutputs[self.parameterInputs[0]["sourceIndex"]]["value"]
        rep = self.parameterInputs[1]["module"].parameterOutputs[self.parameterInputs[1]["sourceIndex"]]["value"]
        start = self.parameterInputs[2]["module"].parameterOutputs[self.parameterInputs[2]["sourceIndex"]]["value"]
        stop = self.parameterInputs[3]["module"].parameterOutputs[self.parameterInputs[3]["sourceIndex"]]["value"]
        phase = self.parameterInputs[4]["module"].parameterOutputs[self.parameterInputs[4]["sourceIndex"]]["value"]
        phase = (phase + 1.0) % 2.0 - 1.0           # Phase range: -1.0 ... 1.0
        amplitude = np.abs(start - stop)
        slope = amplitude * freq
        slope *= 1.0 if(start < stop) else -1.0
        
        
        if(rep >= 0) and ((rep / freq) < t):            # End value always corresponds to t0
            t = 0
        output = start + ((t + (phase / (freq * 2))) * slope) % amplitude
        self.parameterOutputs[0]["value"] = output
        return True
    

if __name__ == '__main__':
    import keyboard
    import matplotlib
    import matplotlib.pyplot as plt
    from Modules import Coefficient
    
    run = True
    def signal_handler():
        global run
        run = False
    keyboard.add_hotkey('ctrl+c', signal_handler)
    
    ramp = Ramp(0)
    
    freq = 1
    rep = 1
    start = -1.0
    stop = 1.0
    phase = -1.0
    
    ramp.setParameterInput(0, Coefficient(0, freq))
    ramp.setParameterInput(1, Coefficient(1, rep))
    ramp.setParameterInput(2, Coefficient(2, start))
    ramp.setParameterInput(3, Coefficient(3, stop))
    ramp.setParameterInput(4, Coefficient(4, phase))
    
    y = []
    fig, ax = plt.subplots()
    FRAME_RATE = 60
    t = 0
    try:
        while run:
            ramp.update(t)
            out = ramp.parameterOutputs[0]["value"]
            print(out)
            y.append(out)
            ax.plot(range(len(y)), y, 'b-')
            plt.draw()
            t += 1 / FRAME_RATE
            plt.pause(1 / FRAME_RATE)
    except KeyboardInterrupt:
        pass
    finally:
        matplotlib.pyplot.close()


    
    