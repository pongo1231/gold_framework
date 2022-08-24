#pragma once

#include "gold/util/vector3.h"

#define WIN32_LEAN_AND_MEAN
#include <GL/glew.h>
#include <windows.h>

#include <string_view>

enum class error_code;

class gold_graphicsdevice
{
  private:
	HWND wnd           = NULL;
	HDC device_context = NULL;
	HGLRC gl_context   = NULL;
	MSG msg {};
	LARGE_INTEGER last_frame_timestamp {};
	float last_frame_time = 0.f;
	gold_vector3 last_cursor_distance;

	bool is_in_render = false;

  public:
	~gold_graphicsdevice();

	error_code init(HINSTANCE instance, size_t width, size_t height, std::wstring_view title_name = L"Gold Game");
	error_code begin_render();
	error_code end_render();

	HGLRC get_context() const;
	HWND get_wnd() const;
	float get_last_frame_time() const;
	gold_vector3 get_last_cursor_distance() const;

  private:
	static LRESULT CALLBACK wnd_proc(HWND wnd, UINT msg, WPARAM param1, LPARAM param2);
};