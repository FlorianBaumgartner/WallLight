import sys
sys.path.append("../..")
from WallLight_Emulator import WallLight
from Modules import Coefficient, Generator, Modifier, Function, Analyzer


if __name__ == '__main__':
    wallLight = WallLight()   
    
    noise = Generator.Random()
    noise.setParameterInput(1, Coefficient(0.0))
    noise.setParameterInput(2, Coefficient(1.0))
    
    monoflop = Modifier.Monoflop()
    monoflop.setParameterInput(0, noise)
    monoflop.setParameterInput(1, Coefficient(1.0 - (1.0 / (WallLight.framerate * 2.0))))
    monoflop.setParameterInput(2, Coefficient(4.0))       
    monoflop.setParameterInput(3, Coefficient(0.0))
    
    speedNoise = Generator.Random()
    speedNoise.setParameterInput(1, Coefficient(1.5))     # Minimum height -> min speed
    speedNoise.setParameterInput(2, Coefficient(4.0))     # Maximum height -> maxspeed
    negNoise = Generator.Random()
    speedNoiseNeg = Modifier.Negate()
    speedNoiseNeg.setParameterInput(0, speedNoise)
    speedNoiseNeg.setParameterInput(1, negNoise)
    speed = Modifier.Sampler()
    speed.setParameterInput(0, speedNoiseNeg)
    speed.setParameterInput(1, monoflop)
    speedNeg = Modifier.Multiplier()
    speedNeg.setParameterInput(0, speed)
    speedNeg.setParameterInput(1, Coefficient(-1.0))
    
    ramp = Generator.Ramp()
    ramp.setParameterInput(0, monoflop)
    ramp.setParameterInput(1, Coefficient(1.0 / 4.0))
    ramp.setParameterInput(2, Coefficient(1.0))
    ramp.setParameterInput(3, speedNeg)
    ramp.setParameterInput(4, speed)
    
    widthNoise = Generator.Random()
    widthNoise.setParameterInput(1, Coefficient(0.2))  # Minimum width
    widthNoise.setParameterInput(2, Coefficient(0.7))  # Maximum Width
    width = Modifier.Sampler()
    width.setParameterInput(0, widthNoise)
    width.setParameterInput(1, monoflop)
    
    triangle = Function.Triangle()
    triangle.setParameterInput(0, ramp)
    triangle.setParameterInput(1, width)
    triangle.setParameterInput(2, Coefficient(0.0))
    triangle.setParameterInput(3, Coefficient(1.5))

    hueNoise = Generator.Random()
    hue = Modifier.Sampler()
    hue.setParameterInput(0, hueNoise)
    hue.setParameterInput(1, monoflop)
    
    hsvToRgb = Modifier.HsvToRgb()
    hsvToRgb.setParameterInput(0, hue)
    
    colorGain = Function.ColorGain()
    colorGain.setParameterInput(0, hsvToRgb, 0)
    colorGain.setParameterInput(1, hsvToRgb, 1)
    colorGain.setParameterInput(2, hsvToRgb, 2)
    colorGain.setInput(0, triangle)
    
    colorSum = Function.Sum()
    colorSum.setInput(0, colorGain)
    
    plotter = Analyzer.ParameterPlotter()
    plotter.setParameterInput(0, monoflop, 0)
    plotter.setParameterInput(1, ramp, 0)
    
    
    wallLight.loadModules()
    wallLight.setOutput(colorSum, 0)
    wallLight.start()
    
    # wallLight.listModules()
    # print(colorGain.id)
    wallLight.saveGraph("saved_graph.json", "Test Graph")
    
    
    try:
        while wallLight.isRunning():
            wallLight.update()
    except KeyboardInterrupt:
        pass
    finally: 
        wallLight.stop()
