import os
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.join(os.path.dirname(__file__), os.path.pardir), os.path.pardir)))
from WallLight_Emulator import WallLight
from pathlib import Path
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
    rect.setParameterInput(5, Coefficient(1.0))
    
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
    amplitude.setParameterInput(1, Coefficient(0.05))           # freq
    amplitude.setParameterInput(3, Coefficient(0.4))            # ampitude
    amplitude.setParameterInput(4, Coefficient(0.45))
    amplitude.setParameterInput(5, Coefficient(0.5))
    
    
    height = Modifier.Multiplier()
    height.setParameterInput(0, switch)
    height.setParameterInput(1, amplitude)

    heightOffset = Modifier.Adder()
    heightOffset.setParameterInput(0, height)
    heightOffset.setParameterInput(1, Coefficient(0.1))
    
    pdf = Function.Pdf()
    pdf.setParameterInput(0, heightOffset)
    pdf.setParameterInput(1, Coefficient(0.001))
    
    
    hueRandom = Generator.Random()
    
    hue = Modifier.Sampler()
    hue.setParameterInput(0, hueRandom)
    hue.setParameterInput(1, rect)
    hue.setParameterInput(2, Coefficient(1.0))      # Falling edge
    
    color = Modifier.HsvToRgb()
    color.setParameterInput(0, hue)
    
    output = Function.ColorGain()
    output.setParameterInput(0, color, 0)
    output.setParameterInput(1, color, 1)
    output.setParameterInput(2, color, 2)
    output.setInput(0, pdf)
    
    plotter = Analyzer.ParameterPlotter()
    plotter.setParameterInput(0, triangle)
    plotter.setParameterInput(1, rect)
    plotter.setParameterInput(2, amplitude)
    # plotter.setParameterInput(2, easeIn)
    # plotter.setParameterInput(3, easeOut)
    

    wallLight.loadModules()
    wallLight.setOutput(output, 0)
    wallLight.start()
    wallLight.saveGraph(Path(__file__).parent / "Animation_Bounce.json", "Bounce Animation")
    
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
