import sys
sys.path.append("../..")
from WallLight_Emulator import WallLight
from Modules import Coefficient, Generator, Modifier, Function, Analyzer


if __name__ == '__main__':
    wallLight = WallLight()
    
    elements = []
    for i in range(6):
        dropPosEnable = Modifier.Monoflop(None)
        dropPosEnable.setParameterInput(0, Generator.Random(None))
        dropPosEnable.setParameterInput(1, Coefficient(1.0 - (1.0 / (WallLight.framerate * 4.0))))
        dropPosEnable.setParameterInput(2, Coefficient(4.0))       
        dropPosEnable.setParameterInput(3, Coefficient(0.0))
        
        dropPosRandom = Generator.Random(None)
        dropPosRandom.setParameterInput(1, Coefficient(0.5))
        dropPosRandom.setParameterInput(2, Coefficient(2.5))
        dropPos = Modifier.Sampler(None)
        dropPos.setParameterInput(0, dropPosRandom)
        dropPos.setParameterInput(1, dropPosEnable)
        
        dropRear = Generator.Ramp(None)
        dropRear.setParameterInput(0, dropPosEnable)
        dropRear.setParameterInput(1, Coefficient(0.3))
        dropRear.setParameterInput(2, Coefficient(1.0))     # Only 1 repetition
        dropRear.setParameterInput(3, dropPos)
        dropRear.setParameterInput(4, Coefficient(0.0))
        
        dropDepthRandom = Generator.Random(None)
        dropDepthRandom.setParameterInput(1, Coefficient(0.15))
        dropDepthRandom.setParameterInput(2, Coefficient(0.5))
        dropDepth = Modifier.Sampler(None)
        dropDepth.setParameterInput(0, dropDepthRandom)
        dropDepth.setParameterInput(1, dropPosEnable)
        
        dropFront = Modifier.Subtractor(None)
        dropFront.setParameterInput(0, dropRear)
        dropFront.setParameterInput(1, dropDepth)   # Drop Width
        
        dropFrontShort = Modifier.Adder(None)
        dropFrontShort.setParameterInput(0, dropFront)
        dropFrontShort.setParameterInput(1, Coefficient(0.03))
        
        dropElement = Function.Ramp(None)
        dropElement.setParameterInput(0, dropRear)
        dropElement.setParameterInput(1, dropFront)
        dropElement.setParameterInput(2, Coefficient(0.0))
        dropElement.setParameterInput(3, Coefficient(1.0))
        
        greenElement = Function.ColorGain(None)
        greenElement.setParameterInput(1, Coefficient(1.0))
        greenElement.setInput(0, dropElement)
        
        whiteElement = Function.Ramp(None)
        whiteElement.setParameterInput(0, dropFrontShort)
        whiteElement.setParameterInput(1, dropFront)
        whiteElement.setParameterInput(2, Coefficient(0.0))
        whiteElement.setParameterInput(3, Coefficient(1.5))
        
        element = Function.Adder(None)
        element.setInput(0, greenElement)
        element.setInput(1, whiteElement)
        
        enableInverted = Modifier.Invert(None)
        enableInverted.setParameterInput(0, dropPosEnable)
        
        brightnessIntegrator = Modifier.Integrator(None)
        brightnessIntegrator.setParameterInput(0, dropPosEnable)
        brightnessIntegrator.setParameterInput(1, Coefficient(5.0))
        brightnessIntegrator.setParameterInput(3, Coefficient(1.0))
        brightnessIntegrator.setParameterInput(4, enableInverted)
        
        brightness = Modifier.Product(None)
        brightness.setParameterInput(0, dropDepth)
        brightness.setParameterInput(1, brightnessIntegrator)
        brightness.setParameterInput(2, Coefficient(2.0))
        
        elementDimm = Function.Brightness(None)
        elementDimm.setParameterInput(0, brightness)
        elementDimm.setInput(0, element)
        
        elements.append(elementDimm)
    
    output = Function.ColorBlend(None)
    for i, e in enumerate(elements):
        output.setInput(i, e)
    
    # plotter = Analyzer.ParameterPlotter(None)
    # plotter.setParameterInput(0, dropPosEnable)
    # plotter.setParameterInput(1, enableInverted)
    # plotter.setParameterInput(2, brightnessIntegrator)
    
    
    wallLight.loadModules()
    wallLight.setOutput(output, 0)
    wallLight.start()
    wallLight.saveGraph("Animation_Matrix.json", "Matrix Animation")
    
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
