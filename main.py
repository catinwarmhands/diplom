from math import *
from PIL import Image
from numpy import *
import matplotlib.pyplot as plt
import cv2
from output_images import *
from haar import *


def printstr(str):
    print(str, end='', flush=True)

recursive = False
threshold = 0.45

# filename = 'img/hello_small.jpg'
# filename = 'img/lena.png'
filename = 'img/pearl.png'

image = Image.open(filename)#.convert('L')

print(filename, ":", image.size, "*", len(image.getbands()))

w, h = image.size
colormode = "".join(image.getbands())
depth = len(colormode)
total_size = w * h * depth


def fht_2d_image(image, steps=None):
    colormode = "".join(image.getbands())
    depth = len(colormode)
    printstr("Processing ")
    channels = []
    for i in range(depth):
        printstr(colormode[i])

        def append(x, a):
            x.append(Image.fromarray(a).convert('L'))

        a = array(image.getchannel(i)) * float(1.0)

        a = fht_2d(a, steps)
        append(channels, a)
    print()
    return Image.merge(colormode, channels)


# plt_output_images([fht_2d_image(image, steps) for steps in range(3)], 5)

printstr("Processing ")
channels_spectre = []
channels_restored = []
for i in range(depth):
    printstr(colormode[i])

    def append(x, a):
        x.append(Image.fromarray(a).convert('L'))

    a = array(image.getchannel(i)) * float(1.0)

    a = fht_2d(a)
    append(channels_spectre, a)

    a = inv_fht_2d(a)
    append(channels_restored, a)
print()
spectre = Image.merge(colormode, channels_spectre)
restored = Image.merge(colormode, channels_restored)

plt_output_images([spectre, restored])
