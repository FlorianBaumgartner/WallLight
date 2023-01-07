import sys
import pyaudio
import numpy as np
sys.path.append("..")

from WallLight_Emulator import WallLight




if __name__ == '__main__':
    wallLight = WallLight()    
    
    CHUNK = 2**11
    RATE = 44100
    movingAvr = 0.0
    p = pyaudio.PyAudio()
    stream = p.open(format=pyaudio.paInt16, channels=1, rate=RATE, input=True, frames_per_buffer=CHUNK)

    # wallLight.loadGraph("Demo_Audio_Analyzer.json")
    wallLight.loadGraph("Demo_Audio.json")
    wallLight.start()
    while wallLight.isRunning():
        data = np.frombuffer(stream.read(CHUNK), dtype=np.int16)
        peak = np.max(np.abs(data)) * 2   
        val = 0.20 * np.log(peak / 2**16) + 1.0
        avrLevel = 0.2#0.08
        movingAvr = val * avrLevel + movingAvr * (1.0 - avrLevel)
        movingAvr = np.maximum(movingAvr, val)
        
        wallLight.updateCoefficient(1, movingAvr)
        wallLight.update()
        
    wallLight.stop()
    stream.stop_stream()
    stream.close()
    p.terminate()
