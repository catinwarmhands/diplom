from math import *

CL = [(1 + sqrt(3)) / (4 * sqrt(2)),
      (3 + sqrt(3)) / (4 * sqrt(2)),
      (3 - sqrt(3)) / (4 * sqrt(2)),
      (1 - sqrt(3)) / (4 * sqrt(2))]

CL = [1 / sqrt(2), 1 / sqrt(2)]


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
    return iCL, iCH


def dwt2(image, CL, delta=0):
    CH = hpf_coeffs(CL)  # Вычисляем недостающие коэффициенты
    w, h = image.shape  # Размеры изображения
    imageT = image.copy()  # Копируем исходное изображение для преобразования
    for i in range(h):  # Обрабатываем строки
        imageT[i, :] = pconv(imageT[i, :], CL, CH, delta)
    for i in range(w):  # Обрабатываем столбцы
        imageT[:, i] = pconv(imageT[:, i], CL, CH, delta)

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
    for i in range(2):
    # while w >= len(CL) and h >= len(CL):
        data[0:w, 0:h] = dwt2(data[0:w, 0:h], CL)
        w //= 2
        h //= 2
    return data


def idwt2(data, CL):
    w, h = data.shape  # Размеры изображения

    # Переупорядочиваем столбцы и строки обратно
    imageT = data.copy()
    imageT[0:h:2, 0:w:2] = data[0:h // 2, 0:w // 2]
    imageT[1:h:2, 0:w:2] = data[h // 2:h, 0:w // 2]
    imageT[0:h:2, 1:w:2] = data[0:h // 2, w // 2:w]
    imageT[1:h:2, 1:w:2] = data[h // 2:h, w // 2:w]

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


# ####################### MY ########################## #

def fht_1d(a):
    result = [None] * len(a)
    n = len(a) // 2

    for i in range(n):
        summ = a[2*i] + a[2*i+1]
        diff = a[2*i] - a[2*i+1]
        result[i]   = summ * 1/2
        result[n+i] = diff * 1/2

    return result


def inv_fht_1d(a):
    # http://vestnik.mstu.edu.ru/v12_2_n35/articles/03_zhar.pdf
    result = [None] * len(a)
    n = len(a) // 2

    for i in range(n):
        summ = a[i] + a[i+n]
        diff = a[i] - a[i+n]
        result[2*i] = summ
        result[2*i+1] = diff

    return result


def map_for_each_row_and_column(f, a):
    h, w = a.shape
    for i in range(h):
        a[i, :] = f(a[i, :])
    for j in range(w):
        a[:, j] = f(a[:, j])
    return a


def fht_2d(a, steps=None):
    if steps == 0:
        return a

    h, w = a.shape
    step = 0
    while w >= 2 and h >= 2:
        a[:h, :w] = map_for_each_row_and_column(fht_1d, a[:h, :w])
        w //= 2
        h //= 2
        step += 1
        if steps is not None and step >= steps:
            break
    return a


def inv_fht_2d(a, steps=None):
    if steps == 0:
        return a

    # fix for non-square images
    def getwh(a, steps):
        h, w = a.shape
        step = 0
        while w >= 2 and h >= 2:
            w //= 2
            h //= 2
            step += 1
            if steps is not None and step >= steps:
                break
        w *= 2
        h *= 2
        return w, h

    w, h = getwh(a, steps)

    hh, ww = a.shape

    step = 0
    while w <= ww and h <= hh:
        a[:h, :w] = map_for_each_row_and_column(inv_fht_1d, a[:h, :w])
        w *= 2
        h *= 2
        step += 1
        if steps is not None and step >= steps:
            break
    return a
