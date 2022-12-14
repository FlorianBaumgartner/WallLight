import time
import tkinter as tk
import tkinter.ttk as ttk


WINDOW_WIDTH = 250
WINDOW_HEIGHT = 1000
FRAME_RATE = 100

NUM_PIXELS = 288



def rgbToHex(r, g, b):
    return f"#{r%256:02X}{g%256:02X}{b%256:02X}"

def darkstyle(root):
    style = ttk.Style(root)
    root.tk.call('source', 'azure dark/azure dark.tcl')
    style.theme_use('azure')
    return style

def main_window():
    root = tk.Tk()
    root.title("WallLight Emulator")
    root.resizable(False, False)
    screenWidth = root.winfo_screenwidth()
    screenHeight = root.winfo_screenheight()
    windowX = int((screenWidth/2) - (WINDOW_WIDTH/2))
    windowY = int((screenHeight/2) - (WINDOW_HEIGHT/2))
    root.geometry(f"{WINDOW_WIDTH}x{WINDOW_HEIGHT}+{windowX}+{windowY}")
    style = darkstyle(root)
    
    canvas = tk.Canvas(root, width=WINDOW_WIDTH, height=WINDOW_HEIGHT)
    canvas.pack()
    
    

    try:
        t = 0
        running = True
        while running:
            if(time.time() - t > 1 / FRAME_RATE):
                t = time.time()
                h = (WINDOW_HEIGHT - 60) / NUM_PIXELS
                y = 30
                for i in range(NUM_PIXELS): 
                    y += h
                    color = rgbToHex(i + int(time.time() * 100), 0, 0)
                    canvas.create_rectangle(30, y, 30 + h * 2, y + h, fill=color, outline="")
                
                
                root.update()
                try:
                    root.winfo_exists()  # This raises an exception if the window has been closed
                except:
                    running = False
    except Exception as e:
        raise type(e)(e)   # Forward any exception to user 
    finally:    # Always close window on exit (even when exception occured)
        try:
            root.destroy()
        except:
            pass


main_window()

