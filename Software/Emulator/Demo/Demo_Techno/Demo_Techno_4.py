import os
import sys
from pathlib import Path
sys.path.append(os.path.abspath(os.path.join(os.path.join(os.path.dirname(__file__), os.path.pardir), os.path.pardir)))
from WallLight_Emulator import WallLight
from pathlib import Path
from Modules import Coefficient, Generator, Modifier, Function, Analyzer


if __name__ == '__main__':
    wallLight = WallLight()

    ramp = Generator.Ramp()
    ramp.setParameterInput(1, Coefficient(0.1))             # Frequency
    ramp.setParameterInput(3, Coefficient(0.0))             # Start
    ramp.setParameterInput(4, Coefficient(1.2))             # End

    pulse = Function.Pulse()
    pulse.setParameterInput(0, ramp)                        # Position
    pulse.setParameterInput(1, Coefficient(0.1))            # Width

    
    wallLight.loadModules()
    wallLight.setOutput(pulse, 0)
    wallLight.start()
    wallLight.saveGraph(Path(__file__).parent / "Demo_Techno_4.json", "Demo Techno 4")

    try:
        while wallLight.isRunning():
            wallLight.update()
    except KeyboardInterrupt:
        pass
    finally: 
        wallLight.stop()
