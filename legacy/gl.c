#pragma once
#include "utils.c"
#include "windows.h"

//dont forget to link opengl_library.dll

s32 _GL_ZERO =                 0x0000;
s32 _GL_ONE =                  0x0001;
s32 _GL_TRIANGLES =           0x0004;
s32 _GL_QUADS = 0x0007;
s32 _GL_BLEND =               0x0BE2;
s32 _GL_SRC_ALPHA =           0x0302;
s32 _GL_ONE_MINUS_SRC_ALPHA = 0x0303;
s32 _GL_TEXTURE_2D =          0x0DE1;
s32 _GL_RGBA8 =               0x8058;  
s32 _GL_UNSIGNED_BYTE =       0x1401;
s32 _GL_BGRA_EXT =            0x80E1;
s32 _GL_TEXTURE_MAX_LEVEL =   0x813D;
s32 _GL_RGBA =                0x1908;
s32 _GL_ALPHA =               0x1906;
s32 _GL_TEXTURE_MAX_ANISOTROPY_EXT = 0x84FE;
s32 _GL_TEXTURE_MAG_FILTER =             0x2800;
s32 _GL_TEXTURE_MIN_FILTER =             0x2801;
s32 _GL_TEXTURE_WRAP_S =             0x2802;
s32 _GL_TEXTURE_WRAP_T =             0x2803;
s32 _GL_NEAREST =                     0x2600;
s32 _GL_LINEAR =                     0x2601;

// LONG WINAPI
// WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
// { 
//     static PAINTSTRUCT ps;

//     switch(uMsg) {
//     case WM_PAINT:
// 	display();
// 	BeginPaint(hWnd, &ps);
// 	EndPaint(hWnd, &ps);
// 	return 0;

//     case WM_SIZE:
// 	glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
// 	PostMessage(hWnd, WM_PAINT, 0, 0);
// 	return 0;

//     case WM_CHAR:
// 	switch (wParam) {
// 	case 27:			/* ESC key */
// 	    PostQuitMessage(0);
// 	    break;
// 	}
// 	return 0;

//     case WM_CLOSE:
// 	PostQuitMessage(0);
// 	return 0;
//     }

//     return DefWindowProc(hWnd, uMsg, wParam, lParam); 
// } 


HWND create_window(s64 x, s64 y, s64 width, s64 height, char *title, BYTE type, DWORD flags) {
   int         pf;
    HDC         hDC;
    HWND        hWnd;
    WNDCLASS    wc;
    PIXELFORMATDESCRIPTOR pfd;
    static HINSTANCE hInstance = 0;

    /* only register the window class once - use hInstance as a flag. */
    if (!hInstance) {
	hInstance = GetModuleHandle(NULL);
	wc.style         = CS_OWNDC;
	// wc.lpfnWndProc   = (WNDPROC)WindowProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = "OpenGL";

	if (!RegisterClass(&wc)) {
	    MessageBox(NULL, "RegisterClass() failed:  "
		       "Cannot register window class.", "Error", MB_OK);
	    return NULL;
	}
    }

    hWnd = CreateWindow("OpenGL", title, WS_OVERLAPPEDWINDOW |
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			x, y, width, height, NULL, NULL, hInstance, NULL);

    if (hWnd == NULL) {
	MessageBox(NULL, "CreateWindow() failed:  Cannot create a window.",
		   "Error", MB_OK);
	return NULL;
    }

    hDC = GetDC(hWnd);

    /* there is no guarantee that the contents of the stack that become
       the pfd are zeroed, therefore _make sure_ to clear these bits. */
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | flags;
    pfd.iPixelType   = type;
    pfd.cColorBits   = 32;

    pf = ChoosePixelFormat(hDC, &pfd);
    if (pf == 0) {
	MessageBox(NULL, "ChoosePixelFormat() failed:  "
		   "Cannot find a suitable pixel format.", "Error", MB_OK); 
	return 0;
    } 
 
    if (SetPixelFormat(hDC, pf, &pfd) == FALSE) {
	MessageBox(NULL, "SetPixelFormat() failed:  "
		   "Cannot set format specified.", "Error", MB_OK);
	return 0;
    } 

    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    ReleaseDC(hDC, hWnd);

    return hWnd;
}
__declspec(dllimport)  void glEnable(s32 cap);
// update_window_events :: #no_context () -> Void #foreign;
// window_clear :: #no_context (red: F32, green: F32, blue: F32, alpha: F32) -> Void #foreign;
// swap_buffers :: #no_context () -> Void #foreign;
// glColor4f       :: __stdcall #no_context (r: F32, g: F32, b: F32, a: F32) -> Void #foreign;
// glColor3f       :: __stdcall #no_context (r: F32, g: F32, b: F32) -> Void #foreign;
__declspec(dllimport)  void glBlendFunc(s32 sfactor, s32 dfactor);
// glEnable        :: __stdcall #no_context (cap : S32) #foreign;
// glDisable       :: __stdcall #no_context (cap : S32) #foreign;
// glGenTextures   :: __stdcall #no_context (count: S32, result: * U32) #foreign;
// glTexParameteri :: __stdcall #no_context (target: S32, pname: S32, param: S32) #foreign;
// glTexParameterf :: __stdcall #no_context (target: S32, pname: S32, param: F32) #foreign;
// glBindTexture   :: __stdcall #no_context (target: S32, texture: U32) #foreign;
// glTexImage2D    :: __stdcall #no_context (target: S32, level: S32, international_format: S32, width: S32, height: S32, border: S32, format: S32, type: S32, pixels: * Void) #foreign;
// glBegin :: __stdcall #no_context (mode: S32) #foreign;
// glEnd :: __stdcall #no_context () #foreign;
// glVertex3f :: __stdcall #no_context (x: F32, y: F32, z: F32) #foreign;
// glTexCoord2f :: __stdcall #no_context (u: F32, v: F32) #foreign;
// glLoadIdentity :: __stdcall #no_context () -> Void #foreign;
void glOrtho(f64 left, f64 right, f64 bottom, f64 top, f64 near, f64 far);

void* game_create_window(int width, int height) {
    HWND handle = create_window(100, 100, width, height, "Cupcake Demo: Invaders!", PFD_TYPE_RGBA, 0);

    // glEnable(_GL_BLEND);
    // glBlendFunc(_GL_SRC_ALPHA, _GL_ONE_MINUS_SRC_ALPHA);
    // glOrtho(0, width, height, 0, -1, 1);

    // glEnable(_GL_TEXTURE_2D);

    return handle;
}