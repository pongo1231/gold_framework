#include "game.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int WINAPI WinMain(HINSTANCE inst, HINSTANCE inst_prev, LPSTR cmdline, int cmdshow)
{
	gold_game gold_game;

	gold_game.init(inst);

	while (gold_game.run())
	{
	}

	return 0;
}