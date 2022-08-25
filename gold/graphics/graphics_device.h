#pragma once

#include "gold/util/vector3.h"
#include "gold/util/string.h"

#define WIN32_LEAN_AND_MEAN
#include <GL/glew.h>
#include <windows.h>

#include <string_view>

enum class error_code;

/*
* Graphics device used for drawing
*/
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
	size_t width      = 0.f;
	size_t height     = 0.f;

	bool is_in_render = false;

  public:
	/*
	* Destructor
	*/
	~gold_graphicsdevice();

	/*
	* Create a window and initialize contexts for rendering
	* <param name="instance">Instance passed to program in WinMain</param>
	* <param name="width">Width of window to create (set to native width for fullscreen)</param>
	* <param name="height">Height of window to create (set to native height for fullscreen)</param>
	* <param name="title_name">Title of window to create (defaults to "Gold Game")</param>
	*/
	void init(HINSTANCE instance, size_t width, size_t height, std::wstring_view title_name = L"Gold Game");
	/*
	* Begin render function, call this at the start of your render function
	* <returns>False if the window was closed, true otherwise</returns>
	*/
	bool begin_render();
	/*
	 * End render function, call this at the end of your render function
	 */
	void end_render();

	/*
	 * <returns>Context used for drawing</returns>
	 */
	HGLRC get_context() const;
	/*
	 * <returns>Window handle used for drawing</returns>
	 */
	HWND get_window() const;
	/*
	 * <returns>Time since last frame</returns>
	 */
	float get_last_frame_time() const;
	/*
	 * <returns>Distance of mouse cursor moved since last frame</returns>
	 */
	gold_vector3 get_last_cursor_distance() const;

	/*
	 * Set title of render window
	 * <param name="title">Text to use for title</param>
	 */
	void set_window_title(const gold_string &title);

  private:
	static LRESULT CALLBACK wnd_proc(HWND wnd, UINT msg, WPARAM param1, LPARAM param2);
};