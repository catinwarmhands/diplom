import matplotlib.pyplot as plt
import cv2
from math import *
from numpy import *


def cv2_output_images(images, colums=3, width=700):
    cols = len(images)
    rows = ceil(cols/colums)
    if cols < colums:
        rows = 1
    if cols > colums:
        cols = colums

    w, h = images[0].size

    if rows > 1:
        while len(images) % colums != 0:
            images.append(Image.fromarray(zeros([w, h], dtype=uint8)).convert('L'))

    for i, image in enumerate(images):
        if len(image.getbands()) == 1:
            images[i] = cv2.cvtColor(array(image), cv2.COLOR_GRAY2BGR)
        else:
            images[i] = array(image)[..., ::-1]

    result_rows = []
    temp = None
    for i, image in enumerate(images):
        if temp is None:
            temp = image
        else:
            temp = concatenate((temp, image), axis=1)

        if ((i + 1) % colums == 0 and temp is not None) or i == len(images) - 1:
            result_rows.append(temp)
            temp = None

    result = result_rows[0]
    for i in range(1, len(result_rows)):
        result = concatenate((result, result_rows[i]), axis=0)

    name = 'Result'
    mult = width / (w * cols)

    cv2.namedWindow(name, cv2.WINDOW_NORMAL)
    cv2.resizeWindow(name, width, int(h * rows * mult))

    while True:
        cv2.imshow(name, result)
        if cv2.waitKey() or cv2.getWindowProperty(name, 1) == -1:
            break


def plt_output_images(images, colums = 3):
    cols = len(images)
    rows = ceil(cols/colums)
    if cols < colums:
        rows = 1
    if cols > colums:
        cols = colums

    for i, image in enumerate(images):
        plt.subplot(rows, cols, i + 1)
        plt.imshow(image)

    plt.show()
