import sys
import numpy as np
sys.path.append("../..")
from pynput import keyboard
from WallLight_Emulator import WallLight


if __name__ == '__main__':
    wallLight = WallLight()    
    wallLight.loadGraph("Demo_OnOff.json")
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