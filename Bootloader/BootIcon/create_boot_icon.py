from PIL import Image
import numpy as np
import re
from pathlib import Path

def create_png_header(width, height):
    # PNG Signature
    png_signature = b"\x89PNG\r\n\x1a\n"

    # IHDR Chunk
    ihdr_length = b"\x00\x00\x00\x0d"
    ihdr_type = b"IHDR"
    ihdr_data = (
        width.to_bytes(4, byteorder='big') +     # Image width (4 bytes)
        height.to_bytes(4, byteorder='big') +    # Image height (4 bytes)
        b"\x08" +                               # Bit depth: 8 bits per channel (1 byte)
        b"\x00" +                               # Color type: Grayscale (1 byte)
        b"\x00\x00\x00\x00"                      # Compression method, filter method, interlace method (4 bytes)
    )
    ihdr_crc = np.uint32(0xFFFFFFFF & (0xEDB88320 ^ np.frombuffer(ihdr_type, dtype=np.uint32)[0] ^ np.frombuffer(ihdr_data, dtype=np.uint32).sum()))
    ihdr_crc = ihdr_crc.to_bytes(4, byteorder='big')

    # Combine all chunks to form the PNG file
    header = png_signature + ihdr_length + ihdr_type + ihdr_data + ihdr_crc

    return header

# Open the image
def open_image(path):
    with open(path, 'r') as f:
        # Read the entire file
        content = f.read()

        match = re.search(r'{(.*)}', content, re.DOTALL)
        if match:
            array_data = match.group(1)
        else:
            raise ValueError('Could not find array data in file')
        
        hex_values = re.findall(r'0x..', array_data)

        # Convert the hexadecimal values to integers and load them into a NumPy array
        array = np.array([int(value, 16) for value in hex_values])

        # image = Image.fromarray(array, 'L')
        # image.show()

        png_header = create_png_header(256, 256)
        with open("output.png", "wb") as f:
            f.write(png_header)
            f.write(array.tobytes())







adafruitIconPath = Path(__file__).parent.parent / "tinyuf2" / "src" / "favicon" / "favicon_adafruit_256.h"
open_image(adafruitIconPath)

