#pragma once

#define WIN32_LEAN_AND_MEAN
#include <GL/glew.h>
#include <windows.h>

#define _NODISCARD [[nodiscard]]

enum class error_code;

class gold_graphicsdevice
{
  private:
	HWND wnd       = NULL;
	HDC device_ctx = NULL;
	HGLRC gl_ctx   = NULL;
	MSG msg {};
	LARGE_INTEGER last_frame_timestamp {};
	float last_frame_time = 0.f;

	bool is_in_render     = false;

  public:
	~gold_graphicsdevice();

	error_code init(HINSTANCE instance);
	error_code begin_render();
	error_code end_render();

	_NODISCARD HGLRC get_ctx() const;
	_NODISCARD HWND get_wnd() const;
	_NODISCARD float get_last_frame_time() const;

  private:
	static LRESULT CALLBACK wnd_proc(HWND wnd, UINT msg, WPARAM param1, LPARAM param2);
};