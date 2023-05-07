import os
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.join(os.path.dirname(__file__), os.path.pardir), os.path.pardir)))
from WallLight_Emulator import WallLight
from pathlib import Path
from Modules import Coefficient, Generator, Modifier, Function, Analyzer


if __name__ == '__main__':
    wallLight = WallLight()
    
    mainHeartBeat = Generator.Rect()
    mainHeartBeat.setParameterInput(1, Coefficient(0.6))            # Frequency
    mainHeartBeat.setParameterInput(3, Coefficient(0.5))            # Amplitude
    mainHeartBeat.setParameterInput(4, Coefficient(0.5))            # Offset
    mainHeartBeat.setParameterInput(6, Coefficient(0.15))            # Duty Cycle
    
    genRamp0 = Generator.Ramp()
    genRamp0.setParameterInput(0, mainHeartBeat)                # Enable
    genRamp0.setParameterInput(1, Coefficient(3.0))             # Frequency
    # genRamp0.setParameterInput(2, Coefficient(1.0))             # Repetition
    genRamp0.setParameterInput(3, Coefficient(0.0))             # Start
    genRamp0.setParameterInput(4, Coefficient(1.0))             # Stop
    genRamp0.setParameterInput(5, Coefficient(0.0))             # Phase
    
    mainHeartBeatDelay = Modifier.Monoflop()
    mainHeartBeatDelay.setParameterInput(0, mainHeartBeat)           # Input
    mainHeartBeatDelay.setParameterInput(1, Coefficient(0.5))       # Trigger Level
    mainHeartBeatDelay.setParameterInput(2, Coefficient(0.3))       # Pulse Length
    mainHeartBeatDelay.setParameterInput(3, Coefficient(0.0))       # Retrigger
    mainHeartBeatDelay.setParameterInput(4, Coefficient(1.0))       # Nagtive Edge
    
    genRamp1 = Generator.Ramp()
    genRamp1.setParameterInput(0, mainHeartBeatDelay)           # Enable
    genRamp1.setParameterInput(1, Coefficient(3.0))             # Frequency
    # genRamp1.setParameterInput(2, Coefficient(1.0))             # Repetition
    genRamp1.setParameterInput(3, Coefficient(0.0))             # Start
    genRamp1.setParameterInput(4, Coefficient(1.2))             # Stop
    genRamp1.setParameterInput(5, Coefficient(0.0))             # Phase
    
    genTriangleCombined = Modifier.Adder()
    genTriangleCombined.setParameterInput(0, genRamp0)
    genTriangleCombined.setParameterInput(1, genRamp1)
    
    expSmoothing = Modifier.ExpSmoothing()
    expSmoothing.setParameterInput(0, genTriangleCombined)
    expSmoothing.setParameterInput(1, Coefficient(0.15))        # Alpha
    
    
    plotter = Analyzer.ParameterPlotter()
    plotter.setParameterInput(0, genRamp0)
    plotter.setParameterInput(1, genRamp1)
    plotter.setParameterInput(2, genTriangleCombined)
    plotter.setParameterInput(3, expSmoothing)
    
    variance = Modifier.Multiplier()
    variance.setParameterInput(0, expSmoothing)
    variance.setParameterInput(1, Coefficient(0.05))
    
    pdf = Function.Pdf()
    pdf.setParameterInput(0, Coefficient(0.5))                  # Mean
    pdf.setParameterInput(1, variance)                 # Variance
    
    background = Function.Rect()
    backgroundColor = Function.ColorGain()
    backgroundColor.setParameterInput(0, Coefficient(0.4))
    backgroundColor.setParameterInput(2, Coefficient(0.4))
    
    # backgroundColor.setParameterInput(3, Coefficient(0.4))      # Amber
    backgroundColor.setInput(0, background)
    
    hue = Modifier.Multiplier()
    hue.setParameterInput(0, expSmoothing)
    hue.setParameterInput(1, Coefficient(0.15/2))
    
    hueOffset = Modifier.Adder()
    hueOffset.setParameterInput(0, hue)
    hueOffset.setParameterInput(1, Coefficient(-0.05))
    
    hsvToRgb = Modifier.HsvToRgb()
    hsvToRgb.setParameterInput(0, hue)
    
    colorGain = Function.ColorGain()
    colorGain.setParameterInput(0, hsvToRgb, 0)
    colorGain.setParameterInput(1, hsvToRgb, 1)
    colorGain.setParameterInput(2, hsvToRgb, 2)
    colorGain.setInput(0, pdf)
    
    brightness = Modifier.Adder()
    brightness.setParameterInput(0, expSmoothing)
    brightness.setParameterInput(1, Coefficient(0.5))
    
    foreground = Function.Brightness()
    foreground.setParameterInput(0, brightness)
    foreground.setInput(0, colorGain)
    
    combinedOutput = Function.Adder()
    combinedOutput.setInput(0, foreground)
    # combinedOutput.setInput(1, backgroundColor)
    
    
    wallLight.loadModules()
    wallLight.setOutput(combinedOutput, 0)
    wallLight.start()
    wallLight.saveGraph(Path(__file__).parent / "Animation_HeartBeat.json", "Heart Beat Animation")
    
    
    try:
        while wallLight.isRunning():
            wallLight.update()
    except KeyboardInterrupt:
        pass
    finally: 
        wallLight.stop()
