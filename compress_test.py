def compress_test():
    recursive = True
    threshold = 0.45

    filename = 'img/hello_small.jpg'
    # filename = 'img/lena.png'
    # filename = 'img/grad3.png'

    image = Image.open(filename)  # .convert('L')

    print(filename, ":", image.size, "*", len(image.getbands()))

    w, h = image.size
    colormode = "".join(image.getbands())
    depth = len(colormode)
    total_size = w * h * depth

    channels_spectre = []
    channels_restored = []

    s = 0

    printstr("Processing ")
    for i in range(depth):
        printstr(colormode[i])

        def append(x, a):
            x.append(Image.fromarray(a * 255).convert('L'))
            # x.append(a)

        a = array(image.getchannel(i)) / 255

        a = rec_dwt2(a, CL) if recursive else dwt2(a, CL)
        append(channels_spectre, a)

        a[abs(a) < threshold] = 0
        s += sum(a == 0)

        a = rec_idwt2(a, CL) if recursive else idwt2(a, CL)

        append(channels_restored, a)

    print()

    print("removed {} of {} ({}%)".format(s, total_size, round(s / total_size * 100, 3)))

    image_spectre = Image.merge(colormode, channels_spectre)
    image_restored = Image.merge(colormode, channels_restored)

    cv2_output_images([image, image_spectre, image_restored], colums=3, width=1000)
    # plt_output_images([image, image_spectre, image_restored])
