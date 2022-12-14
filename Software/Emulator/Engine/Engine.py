import os
import sys
import numpy as np

sys.path.insert(0, os.path.dirname(__file__))
from Modules import Function
from Modules import *

class Engine():
    def __init__(self, pixelcount, framerate):
        self.pixelcount = pixelcount
        self.framerate = framerate
        self.pixels = np.zeros((pixelcount, 6))
        Function.pixelcount = self.pixelcount
        
        
        self.t = 0.0
        
        self.test_const_freq = Coefficient(0, 0.25)
        self.test_const_rep = Coefficient(1, -1)
        self.test_const_amplitude = Coefficient(2, 0.5)
        self.test_const_offset = Coefficient(3, 0.5)
        self.test_const_phase = Coefficient(4, 0.0)
        self.test_const_var = Coefficient(5, 50)
        
        self.test_multiplier_r = Coefficient(6, 0.0)
        self.test_multiplier_g = Coefficient(7, 1.0)
        self.test_multiplier_b = Coefficient(8, 1.0)
        self.test_multiplier_ww = Coefficient(9, 1.0)
        self.test_multiplier_cw = Coefficient(10, 1.0)
        self.test_multiplier_am = Coefficient(11, 1.0)
        
        self.test_sin = Sine(12)
        self.test_sin.setParameterInput(0, self.test_const_freq, 0)
        self.test_sin.setParameterInput(1, self.test_const_rep, 0)
        self.test_sin.setParameterInput(2, self.test_const_amplitude, 0)
        self.test_sin.setParameterInput(3, self.test_const_offset, 0)
        self.test_sin.setParameterInput(4, self.test_const_phase, 0)
        
        self.test_pdf = Pdf(13)
        self.test_pdf.setParameterInput(0, self.test_sin, 0)
        self.test_pdf.setParameterInput(1, self.test_const_var, 0)
        
        self.test_multiplier = Multiplier(14)
        self.test_multiplier.setParameterInput(0, self.test_multiplier_r, 0)
        self.test_multiplier.setParameterInput(1, self.test_multiplier_g, 0)
        self.test_multiplier.setParameterInput(2, self.test_multiplier_b, 0)
        self.test_multiplier.setParameterInput(3, self.test_multiplier_ww, 0)
        self.test_multiplier.setParameterInput(4, self.test_multiplier_cw, 0)
        self.test_multiplier.setParameterInput(5, self.test_multiplier_am, 0)
        self.test_multiplier.setInput(0, self.test_pdf, 0)
        
                
        self.modules = [self.test_const_freq,
                        self.test_const_rep,
                        self.test_const_amplitude,
                        self.test_const_offset,
                        self.test_const_phase,
                        self.test_const_var,
                        self.test_multiplier_r,
                        self.test_multiplier_g,
                        self.test_multiplier_b,
                        self.test_multiplier_ww,
                        self.test_multiplier_cw,
                        self.test_multiplier_am,
                        self.test_sin,
                        self.test_pdf,
                        self.test_multiplier]
        
    
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
                    
        # self.test_pdf.mean = self.test_sin.update(t)
        # self.pixels = self.test_pdf.update(t)
        self.pixels = self.test_multiplier.outputs[0]["value"]


if __name__ == '__main__':
    engine = Engine(288, 60)
    
    