import os
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.join(os.path.dirname(__file__), os.path.pardir), os.path.pardir)))
from WallLight_Emulator import WallLight
from pathlib import Path
from Modules import Coefficient, Generator, Modifier, Function, Analyzer


if __name__ == '__main__':
    wallLight = WallLight()

    periodeTime = 2.0

    rects = []
    for i in range(6):
        rects.append(Generator.Rect())
        rects[i].setParameterInput(1, Coefficient(1.0 / (periodeTime * 6)))     # freq
        rects[i].setParameterInput(3, Coefficient(0.5))                         # amplitude
        rects[i].setParameterInput(4, Coefficient(0.5))                         # offset
        rects[i].setParameterInput(5, Coefficient((1.0 / 6) * i * 2))           # phase
        rects[i].setParameterInput(6, Coefficient(1.0 / 6))                     # dutycicle

    rect = Function.Rect()

    colorGain = Function.ColorGain()
    colorGain.setParameterInput(0, rects[0])
    colorGain.setParameterInput(1, rects[1])
    colorGain.setParameterInput(2, rects[2])
    colorGain.setParameterInput(3, rects[3])
    colorGain.setParameterInput(4, rects[4])
    colorGain.setParameterInput(5, rects[5])

    colorGain.setInput(0, rect)
    
    plotter = Analyzer.ParameterPlotter()
    plotter.setParameterInput(0, rects[0])
    plotter.setParameterInput(1, rects[1])
    plotter.setParameterInput(2, rects[2])
    plotter.setParameterInput(3, rects[3])
    
    wallLight.loadModules()
    wallLight.setOutput(colorGain, 0)
    wallLight.start()
    wallLight.saveGraph(Path(__file__).parent / "Animation_WWA.json", "WWA Animation")
    
    
    try:
        while wallLight.isRunning():
            wallLight.update()
    except KeyboardInterrupt:
        pass
    finally: 
        wallLight.stop()
