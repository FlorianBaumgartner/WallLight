import sys
sys.path.append("../..")
from WallLight_Emulator import WallLight
from Modules import Coefficient, Generator, Modifier, Function, Analyzer


if __name__ == '__main__':
    wallLight = WallLight()
    
    triangleGenerator1 = Generator.Triangle()
    triangleGenerator1.setParameterInput(3, Coefficient(0.5))
    triangleGenerator1.setParameterInput(4, Coefficient(0.5))
    triangleGenerator1.setParameterInput(5, Coefficient(0.5))
    
    triangleGenerator2 = Generator.Triangle()
    triangleGenerator2.setParameterInput(3, Coefficient(0.5))
    triangleGenerator2.setParameterInput(4, Coefficient(0.5))
    
    functionTriangle = Function.Rect()
    
    colorGain1 = Function.ColorGain()
    colorGain1.setParameterInput(0, triangleGenerator1)
    colorGain1.setInput(0, functionTriangle)
    
    colorGain2 = Function.ColorGain()
    colorGain2.setParameterInput(2, triangleGenerator2)
    colorGain2.setInput(0, functionTriangle)
    
    functionAdder = Function.Adder()
    functionAdder.setInput(0, colorGain1)
    functionAdder.setInput(1, colorGain2)
    
    wallLight.loadModules()
    wallLight.setOutput(functionAdder, 0)
    wallLight.start()
    wallLight.saveGraph("Demo_MultiPath.json", "Demo Multi-Path")
    
    
    try:
        while wallLight.isRunning():
            wallLight.update()
    except KeyboardInterrupt:
        pass
    finally: 
        wallLight.stop()
