#foreign_library "stb_image.dll"
#foreign_library "stb_image_write.dll"

 bool :: Bool

stbi_load :: #no_context (filename: *u8, width: *s32, height: *s32, channels_in_file: *s32, desired_channels: s32) -> *u8 #foreign;

stbi_write_png :: #no_context (filename: *u8, width: s32, height: s32, comp: s32, data: *Void, stride_in_bytes: s32) -> s32 #foreign;
stbi_write_bmp :: #no_context (filename: *u8, width: s32, height: s32, comp: s32, data: *Void) -> s32 #foreign;
stbi_write_tga :: #no_context (filename: *u8, width: s32, height: s32, comp: s32, data: *Void) -> s32 #foreign;
stbi_write_hdr :: #no_context (filename: *u8, width: s32, height: s32, comp: s32, data: *float) -> s32 #foreign;

Image :: struct {
	data: *Void;
	width, height, depth: s32;
}

load_image :: (filename: String) -> Image {
	image: Image = ---;
	using image;

	data = stbi_load(to_c_string(filename), *width, *height, *depth, 0);
	// image.data = array_create_from_memory(byte)(data, image.width * image.height * image.depth);

	if !image.data {
		print("image_load(%) fail\n", filename);
		// abort();
	}
	return image
}

strncmp :: __cdecl  #no_context (str1: *u8, str2: *u8, size: s32) -> s32 #foreign "strncmp"

image_write :: (using image: Image, filename: String) -> bool {
	// using image;
	ends_with :: (str: String, suffix: String) -> bool {
		if suffix.count > str.count {
			return 0;
		}
		return strncmp(str.data + str.count - suffix.count , suffix.data, cast(s32)suffix.count) == 0;
	}

	if ends_with(filename, ".png") {
		stbi_write_png(to_c_string(filename), width, height, depth, data, width*depth);
	} else if ends_with(filename, ".bmp") {
		stbi_write_bmp(to_c_string(filename), width, height, depth, data);
	} else if ends_with(filename, ".tga") {
		stbi_write_tga(to_c_string(filename), width, height, depth, data);
	} else {
		print("image_write(%) fail: wrong extension\n", filename);
		return false;
	}
	return true;
}