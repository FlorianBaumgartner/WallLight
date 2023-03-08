import sys
sys.path.append("../..")
from WallLight_Emulator import WallLight
from Modules import Coefficient, Generator, Modifier, Function, Analyzer


if __name__ == '__main__':
    wallLight = WallLight()
    
    generator = Generator.Dirac()
    generator.setParameterInput(1, Coefficient(1))

    rect = Function.Rect()
    rect.setParameterInput(1, generator)
    
    color = Function.ColorGain()
    color.setInput(0, rect)
    color.setParameterInput(0, Generator.Random())
    color.setParameterInput(1, Generator.Random())
    color.setParameterInput(2, Generator.Random())
    

    wallLight.loadModules()
    wallLight.setOutput(color, 0)
    wallLight.start()
    wallLight.saveGraph("Demo_ModuleTest_Generator.json", "Demo Module-Test Generator")
    
    
    try:
        while wallLight.isRunning():
            wallLight.update()
    except KeyboardInterrupt:
        pass
    finally: 
        wallLight.stop()
