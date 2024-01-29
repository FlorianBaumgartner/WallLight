import os
import sys
from pathlib import Path
sys.path.append(os.path.abspath(os.path.join(os.path.join(os.path.dirname(__file__), os.path.pardir), os.path.pardir)))
from WallLight_Emulator import WallLight
from pathlib import Path
from Modules import Coefficient, Generator, Modifier, Function, Analyzer


if __name__ == '__main__':
    wallLight = WallLight()
    
    startX = Generator.Ramp()
    startX.setParameterInput(1, Coefficient(0.2))
    startX.setParameterInput(3, Coefficient(0.0))
    startX.setParameterInput(4, Coefficient(5.0))

    # startX.setParameterInput(2, Coefficient(1.0))
    startY = Generator.Ramp()

    # startY.setParameterInput(1, Coefficient(0.0))
    # startY.setParameterInput(2, Coefficient(1.0))


    triangle = Function.Ramp()
    triangle.setParameterInput(0, startX)
    # triangle.setParameterInput(1, startY)

    
    # plotter = Analyzer.ParameterPlotter()
    # plotter.setParameterInput(0, startX)
    
    
    wallLight.loadModules()
    wallLight.setOutput(triangle, 0)
    wallLight.start()
    wallLight.saveGraph(Path(__file__).parent / "Demo_Techno.json", "Demo Techno")
    
    
    try:
        while wallLight.isRunning():
            wallLight.update()
    except KeyboardInterrupt:
        pass
    finally: 
        wallLight.stop()
