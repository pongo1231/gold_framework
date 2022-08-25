#include "graphics_device.h"

#include "gold/util/assert.h"
#include "gold/util/macros.h"
#include "gold/util/matrix.h"
#include "gold/util/time.h"
#include "gold/util/vertex.h"

#include <GL/glew.h>
#include <GL/wglew.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

gold_graphicsdevice::~gold_graphicsdevice()
{
	if (gl_context)
		wglDeleteContext(gl_context);
}

void gold_graphicsdevice::init(HINSTANCE inst, size_t width, size_t height, std::wstring_view title_name)
{
	if (gl_context)
		gold_assert("gold_graphicsdevice::init context already exists!");

	this->width = width;
	this->height = height;

	WNDCLASSEX wnd_class {};
	wnd_class.lpszClassName = GOLD_CLASSNAME;
	wnd_class.hInstance     = inst;
	wnd_class.lpfnWndProc   = wnd_proc;
	wnd_class.hCursor       = LoadCursor(0, IDC_ARROW);
	wnd_class.style         = CS_HREDRAW | CS_VREDRAW;
	wnd_class.cbSize        = sizeof(WNDCLASSEX);
	RegisterClassEx(&wnd_class);

	auto screen_width  = GetSystemMetrics(SM_CXSCREEN);
	auto screen_height = GetSystemMetrics(SM_CYSCREEN);

	if (width > screen_width)
		gold_assert("gold_graphicsdevice::init width > screen width !");
	if (height > screen_height)
		gold_assert("gold_graphicsdevice::init height > screen height !");

	DWORD flags = WS_VISIBLE | WS_POPUP;
	if (width != screen_width || height != screen_height)
		flags |= WS_CAPTION | WS_SYSMENU;

	if (FAILED(wnd = CreateWindowEx(NULL, GOLD_CLASSNAME, title_name.data(), flags, CW_USEDEFAULT, CW_USEDEFAULT, width,
	                                height, NULL, NULL, inst, NULL)))
		gold_assert("gold_graphicsdevice::init failed to create window!");

	// Taken from https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)
	PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR),
		                          1,
		                          PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // Flags
		                          PFD_TYPE_RGBA, // The kind of framebuffer. RGBA or palette.
		                          32,            // Colordepth of the framebuffer.
		                          0,
		                          0,
		                          0,
		                          0,
		                          0,
		                          0,
		                          0,
		                          0,
		                          0,
		                          0,
		                          0,
		                          0,
		                          0,
		                          24, // Number of bits for the depthbuffer
		                          8,  // Number of bits for the stencilbuffer
		                          0,  // Number of Aux buffers in the framebuffer.
		                          PFD_MAIN_PLANE,
		                          0,
		                          0,
		                          0,
		                          0 };

	device_context            = GetDC(wnd);
	auto pixel_fmt            = ChoosePixelFormat(device_context, &pfd);
	SetPixelFormat(device_context, pixel_fmt, &pfd);

	gl_context = wglCreateContext(device_context);
	if (!gl_context)
		gold_assert("gold_graphicsdevice::init failed to create context!");

	if (!wglMakeCurrent(device_context, gl_context))
		gold_assert("gold_graphicsdevice::init wglMakeCurrent failed!");

	glewExperimental = true;
	auto err_id      = glewInit();
	if (err_id != GLEW_OK)
	{
		LOG_ERROR("glewInit failed (" << glewGetErrorString(err_id) << ")!");
		gold_assert("gold_graphicsdevice::init glewInit failed!");
	}
}

bool gold_graphicsdevice::begin_render()
{
	if (is_in_render)
		gold_assert("gold_graphicsdevice::begin_render already called before, call end_render first!");

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
			return false;
	}

	auto time            = GetTickCount64();
	gold_delta_time      = (time - gold_last_frame_time) / 1000.f;
	gold_last_frame_time = time;

	last_cursor_distance = {};

	if (GetActiveWindow() == wnd)
	{
		ShowCursor(false);
		RECT wnd_rect;
		GetWindowRect(wnd, &wnd_rect);
		POINT cursor_pos;
		GetCursorPos(&cursor_pos);
		SetCursorPos(wnd_rect.left + width * .5f, wnd_rect.top + height * .5f);
		last_cursor_distance = { static_cast<float>(cursor_pos.x - (wnd_rect.left + width * .5f)),
			                     static_cast<float>(cursor_pos.y - (wnd_rect.top + height * .5f)), 0.f };
	}

	glClearColor(.3f, .3f, .3f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	is_in_render = true;

	return true;
}

void gold_graphicsdevice::end_render()
{
	if (!is_in_render)
		gold_assert("gold_graphicsdevice::end_render call begin_render first!");

	glUseProgram(0);

	SwapBuffers(device_context);

	LARGE_INTEGER timestamp {};
	QueryPerformanceCounter(&timestamp);

	last_frame_time      = (timestamp.QuadPart - last_frame_timestamp.QuadPart) / 1000000.f;
	last_frame_timestamp = timestamp;

	is_in_render         = false;
}

HGLRC gold_graphicsdevice::get_context() const
{
	return gl_context;
}

HWND gold_graphicsdevice::get_wnd() const
{
	return wnd;
}

float gold_graphicsdevice::get_last_frame_time() const
{
	return last_frame_time;
}

LRESULT CALLBACK gold_graphicsdevice::wnd_proc(HWND wnd, UINT msg, WPARAM param1, LPARAM param2)
{
	if (msg == WM_DESTROY)
		PostQuitMessage(0);

	return DefWindowProc(wnd, msg, param1, param2);
}

gold_vector3 gold_graphicsdevice::get_last_cursor_distance() const
{
	return last_cursor_distance;
}

void gold_graphicsdevice::set_window_title(const gold_string &title)
{
	SetWindowTextA(wnd, title.c_string());
}