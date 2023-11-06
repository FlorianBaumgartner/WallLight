import os
import sys
import dxcam
import numpy as np
from pathlib import Path
from scipy import interpolate, signal
sys.path.append(os.path.abspath(os.path.join(os.path.join(os.path.dirname(__file__), os.path.pardir), os.path.pardir)))
from WallLight_Emulator import WallLight


if __name__ == '__main__':
    wallLight = WallLight()    
    wallLight.loadGraph(Path(__file__).parent / "Demo_Ambilight.json")
    wallLight.start()
    
    
    width = 200
    camera = dxcam.create()
    camera.start(region=(camera.width - 1 - width, 0, camera.width - 1, camera.height - 1))
    
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
        del camera

