#include "game.h"
#include "gold/error_code.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <iostream>

int WINAPI WinMain(HINSTANCE inst, HINSTANCE inst_prev, LPSTR cmdline, int cmdshow)
{
	gold_game gol_game;
	error_code err_code = error_code::success;

	if ((err_code = gol_game.init(inst)) != error_code::success)
		return static_cast<int>(err_code);

	while ((err_code = gol_game.run()) == error_code::success)
	{
	}

	if (err_code != error_code::stopped)
		std::cout << static_cast<int>(err_code) << "\n";

	return static_cast<int>(err_code);
}