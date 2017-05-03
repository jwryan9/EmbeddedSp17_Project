import string
from PIL import Image


def rgb2hex(r, g, b):
    hex = "#{:02x}{:02x}{:02x}".format(r, g, b)
    return hex


# function takes in a hex color and outputs it inverted
def invert(color_to_convert):
    table = string.maketrans('0123456789abcdef', 'fedcba9876543210')
    return '0x' + color_to_convert[1:].lower().translate(table).upper()

img = Image.open('../Resources/TeamImages/Cubs.png')
width, height = img.size
rgb_img = img.convert('RGB')

hex_img = [[0 for x in range(width)] for y in range(height)]

for h in range(height):
    for w in range(width):
        pixel = rgb_img.getpixel((w, h))
        hex_pixel = rgb2hex(pixel[0], pixel[1], pixel[2])
        inverted_hex_pixel = invert(hex_pixel)
        # hex_img[w][h] = inverted_hex_pixel
        hex_img[w][h] = hex_pixel

out_str = 'arr[' + str(height) + '][' + str(width) + '] = {{'

for h in range(height):
    for w in range(width):
        out_str += str(hex_img[w][h]) + ','
    out_str += '} , {'

out_str = out_str.replace(',}', '}')
out_str = out_str.replace('#', '0x')
print(out_str)



