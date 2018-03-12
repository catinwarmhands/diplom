from math import *
from PIL import Image
from numpy import *
import matplotlib.pyplot as plt

CL = [(1 + sqrt(3)) / (4 * sqrt(2)),
      (3 + sqrt(3)) / (4 * sqrt(2)),
      (3 - sqrt(3)) / (4 * sqrt(2)),
      (1 - sqrt(3)) / (4 * sqrt(2))]

CL= [1/sqrt(2), 1/sqrt(2)]


def hpf_coeffs(CL):
    N = len(CL)  # Количество коэффициентов
    CH = [(-1) ** k * CL[N - k - 1]  # Коэффициенты в обратном порядке с чередованием знака
          for k in range(N)]
    return CH


def pconv(data, CL, CH, delta=0):
    assert (len(CL) == len(CH))  # Размеры списков коэффициентов должны быть равны
    N = len(CL)
    M = len(data)
    out = []  # Список с результатом, пока пустой
    for k in range(0, M, 2):  # Перебираем числа 0, 2, 4…
        sL = 0  # Низкочастотный коэффициент
        sH = 0  # Высокочастотный коэффициент
        for i in range(N):  # Находим сами взвешенные суммы
            sL += data[(k + i - delta) % M] * CL[i]
            sH += data[(k + i - delta) % M] * CH[i]
        out.append(sL)  # Добавляем коэффициенты в список
        out.append(sH)
    return out


def icoeffs(CL, CH):
    assert (len(CL) == len(CH))  # Размеры списков коэффициентов должны быть равны
    iCL = []  # Коэффициенты первой строки
    iCH = []  # Коэффициенты второй строки
    for k in range(0, len(CL), 2):
        iCL.extend([CL[k - 2], CH[k - 2]])
        iCH.extend([CL[k - 1], CH[k - 1]])
    return (iCL, iCH)


X = [0, 1, 2, 3]
CH = hpf_coeffs(CL)
iCL, iCH = icoeffs(CL, CH)
Y = pconv(X, CL, CH)
X2 = pconv(Y, iCL, iCH, len(CL) - 2)


def dwt2(image, CL):
    CH = hpf_coeffs(CL)  # Вычисляем недостающие коэффициенты
    w, h = image.shape  # Размеры изображения
    imageT = image.copy()  # Копируем исходное изображение для преобразования
    for i in range(h):  # Обрабатываем строки
        imageT[i, :] = pconv(imageT[i, :], CL, CH)
    for i in range(w):  # Обрабатываем столбцы
        imageT[:, i] = pconv(imageT[:, i], CL, CH)

    # Переупорядочиваем столбцы и строки
    data = imageT.copy()
    data[0:h // 2, 0:w // 2] = imageT[0:h:2, 0:w:2]
    data[h // 2:h, 0:w // 2] = imageT[1:h:2, 0:w:2]
    data[0:h // 2, w // 2:w] = imageT[0:h:2, 1:w:2]
    data[h // 2:h, w // 2:w] = imageT[1:h:2, 1:w:2]
    return data


def rec_dwt2(image, CL):
    data = image.copy()
    w, h = data.shape
    while w >= len(CL) and h >= len(CL):
        data[0:w, 0:h] = dwt2(data[0:w, 0:h], CL)
        w //= 2
        h //= 2
    return data


def idwt2(data, CL):
    w, h = data.shape  # Размеры изображения

    # Переупорядочиваем столбцы и строки обратно
    imageT = data.copy()
    imageT[0:h:2, 0:w:2] = data[0:h//2, 0:w//2]
    imageT[1:h:2, 0:w:2] = data[h//2:h, 0:w//2]
    imageT[0:h:2, 1:w:2] = data[0:h//2, w//2:w]
    imageT[1:h:2, 1:w:2] = data[h//2:h, w//2:w]

    CH = hpf_coeffs(CL)
    iCL, iCH = icoeffs(CL, CH)
    image = imageT.copy()  # Копируем исходное изображение для преобразования
    for i in range(w):  # Обрабатывем столбцы
        image[:, i] = pconv(image[:, i], iCL, iCH, delta=len(iCL) - 2)
    for i in range(h):  # Обрабатывем строки
        image[i, :] = pconv(image[i, :], iCL, iCH, delta=len(iCL) - 2)

    return image

def rec_idwt2(image, CL):
    data = image.copy()
    w, h = len(CL), len(CL)

    ww, hh = data.shape
    while w <= ww and h <= hh:
        data[0:w, 0:h] = idwt2(data[0:w, 0:h], CL)
        w *= 2
        h *= 2
    return data


image = Image.open('img/lena.png').convert("L")

# for i in range(3):
#     a = array(image.getchannel(i)) / 255
#     data2 = dwt2(a, CL)
#     plt.subplot(1, 3, i+1)
#     plt.imshow(a, cmap="Greys_r")

plt.subplot(1, 3, 1)
a = array(image) / 255
plt.imshow(a, cmap="Greys_r")

plt.subplot(1, 3, 2)
a = rec_dwt2(a, CL)
plt.imshow(a, cmap="Greys_r", clim=(0.0, 0.6))

threshold = 0.35
a[abs(a)<threshold] = 0
s = sum(a==0)

plt.subplot(1, 3, 3)
b = rec_idwt2(a, CL)
plt.imshow(b, cmap="Greys_r")

print("removed = {} ({}%)".format(s, s/(len(a)**2)*100))

plt.show()