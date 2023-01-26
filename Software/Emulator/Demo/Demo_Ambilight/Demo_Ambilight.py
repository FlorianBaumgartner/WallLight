import sys
import mss               # pip install mss
import numpy as np
from scipy import signal
sys.path.append("../..")
from WallLight_Emulator import WallLight


if __name__ == '__main__':
    wallLight = WallLight()    
    wallLight.loadGraph("Demo_Ambilight.json")
    wallLight.start()
    
    try:
        with mss.mss() as sct:
            while wallLight.isRunning():
                monitorIndex = min(len(sct.monitors), 1)
                raw = sct.grab(sct.monitors[monitorIndex])
                image = np.array(raw)[:,:,:3]
                
                pos = 0.9
                height = np.shape(image)[0]         # Get screen height in pixels
                width = np.shape(image)[1]          # Get screen width in pixels
                ambilightRow = np.flip(image[:,int(pos * width)] / 255, axis=0)
                
                # colors = signal.resample(ambilightRow[100:], 6)
                colors = np.array([ambilightRow[int((0.05 + (i / 5) * 0.9) * height)] for i in range(6)])
                for i, c in enumerate(colors):
                    wallLight.updateCoefficient(1001 + i * 4, c[2])
                    wallLight.updateCoefficient(1002 + i * 4, c[1])
                    wallLight.updateCoefficient(1003 + i * 4, c[1])

                wallLight.update()
    except KeyboardInterrupt:
        pass
    finally: 
        wallLight.stop()
