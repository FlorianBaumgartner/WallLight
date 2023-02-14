import sys
sys.path.append("../..")
from WallLight_Emulator import WallLight
from Modules import Coefficient, Generator, Modifier, Function, Analyzer


if __name__ == '__main__':
    wallLight = WallLight()
    
    triangle = Generator.Triangle()
    triangle.setParameterInput(1, Coefficient(0.5))
    triangle.setParameterInput(3, Coefficient(0.5))
    triangle.setParameterInput(4, Coefficient(0.5))
    
    rect = Generator.Rect()
    rect.setParameterInput(1, Coefficient(0.5))
    rect.setParameterInput(3, Coefficient(0.5))
    rect.setParameterInput(4, Coefficient(0.5))
    rect.setParameterInput(5, Coefficient(0.5))
    
    easeIn = Modifier.EaseIn()
    easeIn.setParameterInput(0, triangle)
    easeIn.setParameterInput(1, Coefficient(0.0))
    
    easeOut = Modifier.EaseOut()
    easeOut.setParameterInput(0, triangle)
    easeOut.setParameterInput(1, Coefficient(0.0))
    
    switch = Modifier.Switch()
    switch.setParameterInput(0, easeOut)
    switch.setParameterInput(1, easeIn)
    switch.setParameterInput(2, rect)
    
    # amplitude = Generator.Ramp()
    # amplitude.setParameterInput(1, Coefficient(0.05))
    # amplitude.setParameterInput(3, Coefficient(2.0))
    # amplitude.setParameterInput(4, Coefficient(0.05))
    
    amplitude = Generator.Sine()
    amplitude.setParameterInput(1, Coefficient(0.05))
    amplitude.setParameterInput(3, Coefficient(0.5))
    amplitude.setParameterInput(4, Coefficient(0.5))
    
    
    height = Modifier.Multiplier()
    height.setParameterInput(0, switch)
    height.setParameterInput(1, amplitude)
    
    pulse = Function.Pulse()
    pulse.setParameterInput(0, height)
    pulse.setParameterInput(1, Coefficient(0.05))
    
    
    hueRandom = Generator.Random()
    
    hue = Modifier.Sampler()
    hue.setParameterInput(0, hueRandom)
    hue.setParameterInput(1, rect)
    hue.setParameterInput(2, Coefficient(2.0))
    
    color = Modifier.HsvToRgb()
    color.setParameterInput(0, hue)
    
    output = Function.ColorGain()
    output.setParameterInput(0, color, 0)
    output.setParameterInput(1, color, 1)
    output.setParameterInput(2, color, 2)
    output.setInput(0, pulse)
    
    
    # plotter = Analyzer.ParameterPlotter()
    # plotter.setParameterInput(0, amplitude)
    # plotter.setParameterInput(1, triangle)
    # plotter.setParameterInput(2, rect)
    
    
    
    wallLight.loadModules()
    wallLight.setOutput(output, 0)
    wallLight.start()
    
    # wallLight.listModules()
    # print(colorGain.id)
    # wallLight.saveGraph("saved_graph.json", "Test Graph")
    
    
    try:
        while wallLight.isRunning():
            wallLight.update()
    except KeyboardInterrupt:
        pass
    finally: 
        wallLight.stop()
