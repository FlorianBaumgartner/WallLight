import sys
sys.path.append("../..")
from WallLight_Emulator import WallLight
from Modules import Coefficient, Generator, Modifier, Function, Analyzer


if __name__ == '__main__':
    wallLight = WallLight()
    
    triangleGenerator = Generator.Triangle()
    triangleGenerator.setParameterInput(3, Coefficient(100.0))
    triangleGenerator.setParameterInput(4, Coefficient(0.5))
    triangleGenerator.setParameterInput(5, Coefficient(0.5))
    
    functionTriangle = Function.Rect()
    
    colorGain = Function.ColorGain()
    colorGain.setParameterInput(0, triangleGenerator)
    colorGain.setParameterInput(1, triangleGenerator)
    colorGain.setParameterInput(2, triangleGenerator)
    colorGain.setInput(0, functionTriangle)
    
    
    wallLight.loadModules()
    wallLight.setOutput(colorGain, 0)
    wallLight.start()
    wallLight.saveGraph("Demo_SpeedTest.json", "Demo Speed-Test")
    
    
    try:
        while wallLight.isRunning():
            wallLight.update()
    except KeyboardInterrupt:
        pass
    finally: 
        wallLight.stop()
