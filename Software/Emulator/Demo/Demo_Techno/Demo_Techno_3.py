import os
import sys
from pathlib import Path
sys.path.append(os.path.abspath(os.path.join(os.path.join(os.path.dirname(__file__), os.path.pardir), os.path.pardir)))
from WallLight_Emulator import WallLight
from pathlib import Path
from Modules import Coefficient, Generator, Modifier, Function, Analyzer


if __name__ == '__main__':
    wallLight = WallLight()

    numInserts = 4
    insert = None
    for i in range(numInserts):
        amplitude = (0.5 / numInserts) * 0.85
        offset = (i + 1) * (1.0 / numInserts) - (0.5 / numInserts)
        frequency = 0.1 * (i + 1) * 0.5
        pos = Generator.Sine()
        pos.setParameterInput(1, Coefficient(frequency))                    # Frequency
        pos.setParameterInput(3, Coefficient(amplitude))                    # Amplitude
        pos.setParameterInput(4, Coefficient(offset))                       # Offset

        hueOffset = Generator.Sine()
        hueOffset.setParameterInput(1, Coefficient(0.02))                   # Frequency
        hueOffset.setParameterInput(3, Coefficient(0.4))                    # Amplitude
        hueOffset.setParameterInput(4, Coefficient(0.5))                    # Offset

        hueScale = Generator.Sine()
        hueScale.setParameterInput(1, Coefficient(0.05))                    # Frequency
        hueScale.setParameterInput(3, Coefficient(0.03))                    # Amplitude
        hueScale.setParameterInput(4, Coefficient(0.07))                    # Offset

        hueProduct = Modifier.Multiplier()
        hueProduct.setParameterInput(0, hueScale)                           # Offset
        hueProduct.setParameterInput(1, Coefficient(i))                     

        hueValue = Modifier.Adder()
        hueValue.setParameterInput(0, hueProduct)                           # Slow chaning general hue over time
        hueValue.setParameterInput(1, hueOffset)                            # Hue offset within the bar

        hueOffset = 0.5
        hueScale = 0.1
        hsv = Modifier.HsvToRgb()
        # hsv.setParameterInput(0, Coefficient(hueOffset + i * hueScale))     # Hue
        hsv.setParameterInput(0, hueValue)                                  # Hue


        insertOld = insert
        insert = Function.ColorInsert()
        insert.setParameterInput(0, pos)                                    # Position
        insert.setParameterInput(1, hsv, 0)                                 # Red
        insert.setParameterInput(2, hsv, 1)                                 # Green
        insert.setParameterInput(3, hsv, 2)                                 # Blue
        if(insertOld != None):
            insert.setInput(0, insertOld)
    
    interpolate = Function.ColorInterpolate()
    interpolate.setInput(0, insert)

    
    wallLight.loadModules()
    wallLight.setOutput(interpolate, 0)
    # wallLight.setOutput(insert, 0)
    wallLight.start()
    wallLight.saveGraph(Path(__file__).parent / "Demo_Techno_3.json", "Demo Techno 3")

    try:
        while wallLight.isRunning():
            wallLight.update()
    except KeyboardInterrupt:
        pass
    finally: 
        wallLight.stop()
