import cv2


def show_webcam(mirror=False):
    name = "Hello, OpenCV"
    w, h = 512, 512
    cv2.namedWindow(name, 0)  # 0 makes it work a bit better
    cv2.resizeWindow(name, w, h)  # this doesn't keep
    cam = cv2.VideoCapture(0)
    while True:
        ret_val, img = cam.read()
        if mirror:
            img = cv2.flip(img, 1)
        cv2.imshow(name, img)
        if cv2.waitKey(1) == 27 or cv2.getWindowProperty(name, 1) == -1:
            break  # esc to quit
    cv2.destroyAllWindows()



show_webcam(mirror=True)