import os
import sys
from pathlib import Path
sys.path.append(os.path.abspath(os.path.join(os.path.join(os.path.dirname(__file__), os.path.pardir), os.path.pardir)))
from WallLight_Emulator import WallLight
from pathlib import Path
from Modules import Coefficient, Generator, Modifier, Function, Analyzer


if __name__ == '__main__':
    wallLight = WallLight()

    
    dots = []
    for i in range(4):          # Why the fuck do more than 4 crash the firmware ?!
        sine = Generator.Sine()
        sine.setParameterInput(1, Coefficient(0.03 * (i + 1)))    # Frequency
        sine.setParameterInput(3, Coefficient(0.5))    # Amplitude
        sine.setParameterInput(4, Coefficient(0.5))    # Offset
        sine.setParameterInput(5, Coefficient(0.0))    # Phase
        
        dot = Function.Pdf()
        dot.setParameterInput(0, sine)
        dot.setParameterInput(1, Coefficient(0.0015))

        hsvToRgb = Modifier.HsvToRgb()
        hsvToRgb.setParameterInput(0, Coefficient(float(i) * 0.08 + 0.7))
        hsvToRgb.setParameterInput(2, Coefficient(1.0))
        
        color = Function.ColorGain()
        color.setInput(0, dot)
        color.setParameterInput(0, hsvToRgb, 0)
        color.setParameterInput(1, hsvToRgb, 1)
        color.setParameterInput(2, hsvToRgb, 2)
        
        dots.append(color)
    
    merge = Function.ColorBlend()
    for i, d in enumerate(dots):
        merge.setInput(i, d)
    
    wallLight.loadModules()
    wallLight.setOutput(merge, 0)
    wallLight.start()
    wallLight.saveGraph(Path(__file__).parent / "Demo_Techno_2.json", "Demo Techno 2")
    
    
    try:
        while wallLight.isRunning():
            wallLight.update()
    except KeyboardInterrupt:
        pass
    finally: 
        wallLight.stop()
