
// NOTE(Wase): Foreign function from opengl_library
stbi_png_load :: #no_context (filename: *U8, x: *S32, y: *S32, comp: *S32, req_comp: S32) -> *U8 #foreign

Texture_Format :: enum {
    ARGB8888: 0
}

Texture_Map :: struct {
    width: S32;
    height: S32;
    
    id: U32; 
    data: *Bitmap;  
}

Bitmap :: struct {
    width: S32;
    height: S32;

    data: * U8;

    format :Texture_Format = Texture_Format.ARGB8888;
    num_mipmap_levels :S64 = 1;
    length_in_bytes : S64;
}

create_texture :: (data: *Bitmap) -> *Texture_Map {
    map := new Texture_Map;
    map.width = data.width;
    map.height = data.height;
    map.data = data;

    update_texture(map);

    return map;
}

update_texture :: (map: *Texture_Map) {
    if (map.id == 0)
        glGenTextures(1, *map.id);

    if !map.data return;

    glBindTexture(_GL_TEXTURE_2D, map.id)

    glTexImage2D(_GL_TEXTURE_2D, 0, _GL_RGBA8, 
                 map.data.width, map.data.height,
                 0, _GL_RGBA, _GL_UNSIGNED_BYTE, map.data.data)
 
    glTexParameteri(_GL_TEXTURE_2D, _GL_TEXTURE_MAX_LEVEL, 0)
    glTexParameteri(_GL_TEXTURE_2D, _GL_TEXTURE_MAG_FILTER, _GL_NEAREST)
}


texture_load :: (filename: String) -> *Texture_Map {
	width:  S32 = 0;
    height: S32 = 0;

    data := stbi_png_load(to_c_string(filename), *width, *height, null, 0);

    if !data {
        print("FAILED to load bitmap '%'\n", filename)
        return null
    }

    result := new Bitmap;
    result.width = width;
    result.height = height;
    result.data = data;
    result.length_in_bytes = width*height*4;

    texture := create_texture(result);
    return texture;
}

// FONT
// STB TRUETYPE

stbtt_bakedchar :: struct {
    x0, y0, x1, y1: U16      
    xoff, yoff, xadvance: F32
} 

stbtt_aligned_quad :: struct {
    x0, y0, s0, t0: F32  
    x1, y1, s1, t1: F32  
} 

Font_Map :: struct {
    size: F32
    texture: *Texture_Map // opengl texture id
    characters: [] stbtt_bakedchar
}

stbtt_BakeFontBitmap :: (data: *U8, offset: S32, pixel_height: F32, pixels: *U8, pw: S32, ph: S32, first_char: S32, num_chars: S32, chardata: *stbtt_bakedchar) -> S32 #foreign       
stbtt_GetBakedQuad :: (chardata: *stbtt_bakedchar, pw: S32, ph: S32, char_index: S32, xpos: *F32, ypos: *F32, quad: *stbtt_aligned_quad, opengl_fillrule: S32) #foreign

font_load :: (filename: String, font_size: F32) -> *Font_Map {
    stream := fopen(to_c_string(filename), to_c_string("rb"))
    if !stream then return null

    fseek(stream, 0, SEEK_END)
    size := ftell(stream)
    fseek(stream, 0, SEEK_SET)

    if size <= 0 then return null

	file_buffer := new [size] U8 ---
	texture_buffer := new [512 * 512] U8 ---
    defer delete texture_buffer
    defer delete file_buffer

	font := new Font_Map
	font.characters = new [96] stbtt_bakedchar
	font.size = font_size
	font.texture = new Texture_Map
	font.texture.width = 512 
	font.texture.height = 512 

	read_size := fread(file_buffer.data, 1, cast(S32) file_buffer.count, stream)
	if read_size <= 0 then return null
	fclose(stream);

	baked_return := stbtt_BakeFontBitmap(file_buffer.data, 0, font_size, texture_buffer.data, 512, 512, 32, 96, font.characters.data)
	if baked_return <= 0 then return null

    glGenTextures(1, *font.texture.id);
    glBindTexture(_GL_TEXTURE_2D, font.texture.id)

    glTexImage2D(_GL_TEXTURE_2D, 0, _GL_ALPHA, 
                 512, 512,
                 0, _GL_ALPHA, _GL_UNSIGNED_BYTE, texture_buffer.data)
 
    glTexParameteri(_GL_TEXTURE_2D, _GL_TEXTURE_MAX_LEVEL, 0)
    glTexParameteri(_GL_TEXTURE_2D, _GL_TEXTURE_MAG_FILTER, _GL_LINEAR)
    return font
}

