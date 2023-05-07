import os
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.join(os.path.dirname(__file__), os.path.pardir), os.path.pardir)))
from WallLight_Emulator import WallLight
from pathlib import Path
from Modules import Coefficient, Generator, Modifier, Function, Analyzer


if __name__ == '__main__':
    wallLight = WallLight()
    
    fire = Function.Fire()
    fire.setParameterInput(0, Coefficient(0.1))         # Intensity
    fire.setParameterInput(1, Coefficient(0.98))        # Ignition
    fire.setParameterInput(2, Coefficient(0.3))         # Cooling
    fire.setParameterInput(3, Coefficient(0.01))        # Speed
    fire.setParameterInput(4, Coefficient(0.08))        # Acceleration
    
    wallLight.loadModules()
    wallLight.setOutput(fire, 0)
    wallLight.start()
    wallLight.saveGraph("Demo_Fire.json", "Demo Fire")
    
    
    try:
        while wallLight.isRunning():
            wallLight.update()
    except KeyboardInterrupt:
        pass
    finally: 
        wallLight.stop()
