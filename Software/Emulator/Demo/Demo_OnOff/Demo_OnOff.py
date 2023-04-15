import os
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.join(os.path.dirname(__file__), os.path.pardir), os.path.pardir)))
from pathlib import Path
from pynput import keyboard
from WallLight_Emulator import WallLight


if __name__ == '__main__':
    wallLight = WallLight()    
    wallLight.loadGraph(Path(__file__).parent / "Demo_OnOff.json")
    wallLight.start()
    
    onOff = False
    def on_press(key):
        global onOff
        if(str(key) == "Key.space"):
            onOff = not onOff
            print(f"Light state: {onOff}")
            
    keyListener = keyboard.Listener(on_press=on_press)
    keyListener.start()

    print("Press Space-Bar to toggle ON-OFF State")
    try:
        while wallLight.isRunning():
            wallLight.updateCoefficient(1000, onOff)
            wallLight.update()
    except KeyboardInterrupt:
        pass
    finally: 
        wallLight.stop()
        keyListener.stop()