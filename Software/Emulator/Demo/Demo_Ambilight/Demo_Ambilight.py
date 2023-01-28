import sys
import dxcam
import numpy as np
from scipy import interpolate, signal
sys.path.append("../..")
from WallLight_Emulator import WallLight


if __name__ == '__main__':
    wallLight = WallLight()    
    wallLight.loadGraph("Demo_Ambilight.json")
    wallLight.start()
    
    pos = 0.9
    
    width = 100
    camera = dxcam.create()
    x = int(camera.width * pos)
    camera.start(region=(x, 0, x + width, camera.height - 1))
    
    try:
        while wallLight.isRunning():
            ambilightRow = np.flip(camera.get_latest_frame() / 255, axis=0)
            ambilightRow = np.mean(ambilightRow, axis=1)
            
            padding = np.zeros((250, 3))
            endPadding = np.ones((250, 3)) * ambilightRow[-1]
            ambilightRow = np.vstack([ambilightRow, endPadding, padding])
            colors = signal.resample(ambilightRow, 8)[1:-1]
            
            for i, c in enumerate(colors):
                wallLight.updateCoefficient(1001 + i * 4, c[0])
                wallLight.updateCoefficient(1002 + i * 4, c[1])
                wallLight.updateCoefficient(1003 + i * 4, c[2])

            wallLight.update()
    except KeyboardInterrupt:
        pass
    finally: 
        wallLight.stop()
        camera.stop()

