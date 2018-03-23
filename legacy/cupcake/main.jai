#import "modules/Print.jai"

#import "Image.jai"


main :: () {
	image := load_image("D:/code/diplom/img/test1.png");
	print("image = %", image);
	image_write(image, "test2.bmp");
}

#run main()