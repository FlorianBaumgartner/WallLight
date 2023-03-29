import sys
sys.path.append("../..")
from WallLight_Emulator import WallLight
from Modules import Coefficient, Generator, Modifier, Function, Analyzer


if __name__ == '__main__':
    wallLight = WallLight()
    
    fire = Function.Fire()
    fire.setParameterInput(0, Coefficient(0.1))
    fire.setParameterInput(1, Coefficient(0.98))
    fire.setParameterInput(2, Coefficient(0.3))
    fire.setParameterInput(3, Coefficient(0.01))
    fire.setParameterInput(4, Coefficient(0.08))
    
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
