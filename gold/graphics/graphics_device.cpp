#include "graphics_device.h"

#include "gold/error_code.h"
#include "gold/util/macros.h"
#include "gold/util/matrix.h"
#include "gold/util/time.h"
#include "gold/util/vector.h"
#include "gold/util/vertex.h"

#include <GL/glew.h>
#include <GL/wglew.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

gold_graphicsdevice::~gold_graphicsdevice()
{
	if (gl_ctx)
		wglDeleteContext(gl_ctx);
}

error_code gold_graphicsdevice::init(HINSTANCE inst)
{
	if (gl_ctx)
		return error_code::already_exists;

	WNDCLASSEX wnd_class {};
	wnd_class.lpszClassName = DEMO_CLASSNAME;
	wnd_class.hInstance     = inst;
	wnd_class.lpfnWndProc   = wnd_proc;
	wnd_class.hCursor       = LoadCursor(0, IDC_ARROW);
	wnd_class.style         = CS_HREDRAW | CS_VREDRAW;
	wnd_class.cbSize        = sizeof(WNDCLASSEX);
	RegisterClassEx(&wnd_class);

	if (FAILED(wnd = CreateWindowEx(NULL, DEMO_CLASSNAME, DEMO_TITLENAME,
	                                WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
	                                1280, 720, NULL, NULL, inst, NULL)))
		return error_code::graphics_init_windowcreate_failed;

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

	device_ctx                = GetDC(wnd);
	auto pixel_fmt            = ChoosePixelFormat(device_ctx, &pfd);
	SetPixelFormat(device_ctx, pixel_fmt, &pfd);

	gl_ctx = wglCreateContext(device_ctx);
	if (!gl_ctx)
		return error_code::graphics_init_graphics_failed;

	if (!wglMakeCurrent(device_ctx, gl_ctx))
	{
		LOG_ERROR("wglMakeCurrent failed!");

		return error_code::graphics_init_graphics_failed;
	}

	glewExperimental = true;
	auto err_id      = glewInit();
	if (err_id != GLEW_OK)
	{
		LOG_ERROR("glewInit failed (" << glewGetErrorString(err_id) << ")!");

		return error_code::graphics_init_graphics_failed;
	}

	return error_code::success;
}

error_code gold_graphicsdevice::begin_render()
{
	if (is_in_render)
		return error_code::already_run;

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
			return error_code::stopped;
	}

	auto time            = GetTickCount64();
	gold_delta_time      = (time - gold_last_frame_time) / 1000.f;
	gold_last_frame_time = time;

	glClearColor(.3f, .3f, .3f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	is_in_render = true;

	return error_code::success;
}

error_code gold_graphicsdevice::end_render()
{
	if (!is_in_render)
		return error_code::run_begin_first;

	glUseProgram(0);

	SwapBuffers(device_ctx);

	LARGE_INTEGER timestamp {};
	QueryPerformanceCounter(&timestamp);

	last_frame_time      = (timestamp.QuadPart - last_frame_timestamp.QuadPart) / 1000000.f;
	last_frame_timestamp = timestamp;

	is_in_render         = false;

	return error_code::success;
}

_NODISCARD HGLRC gold_graphicsdevice::get_ctx() const
{
	return gl_ctx;
}

_NODISCARD HWND gold_graphicsdevice::get_wnd() const
{
	return wnd;
}

_NODISCARD float gold_graphicsdevice::get_last_frame_time() const
{
	return last_frame_time;
}

LRESULT CALLBACK gold_graphicsdevice::wnd_proc(HWND wnd, UINT msg, WPARAM param1, LPARAM param2)
{
	if (msg == WM_DESTROY)
		PostQuitMessage(0);

	return DefWindowProc(wnd, msg, param1, param2);
}