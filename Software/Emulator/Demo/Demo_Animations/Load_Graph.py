import sys
import time
sys.path.append("../..")
from WallLight_Emulator import WallLight
from Modules import Coefficient, Generator, Modifier, Function, Analyzer


if __name__ == '__main__':
    wallLight = WallLight()   
    wallLight.start()

    graphs = ["Animation_Matrix.json", "saved_graph.json"]
    
    t = time.time()
    i = 0
    try:
        while wallLight.isRunning():
            if(time.time() - t > 7):
                t = time.time()
                print("Load new graph...")
                wallLight.loadGraph(graphs[i])
                i = (i + 1) % len(graphs)
                
            wallLight.update()
    except KeyboardInterrupt:
        pass
    finally: 
        wallLight.stop()
