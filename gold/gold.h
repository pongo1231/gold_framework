#include "gold/camera.h"
#include "gold/entity.h"
#include "gold/error_code.h"
#include "gold/graphics/graphics_device.h"
#include "gold/graphics/mesh.h"
#include "gold/graphics/model/factory.h"
#include "gold/graphics/model/primitives/cube.h"
#include "gold/graphics/model/primitives/skybox.h"
#include "gold/graphics/shader.h"
#include "gold/graphics/shader_program.h"
#include "gold/graphics/texture.h"
#include "gold/input.h"
#include "gold/memory.h"
#include "gold/scriptmanager.h"
#include "gold/util/macros.h"
#include "gold/util/string.h"
#include "gold/util/time.h"
#include "gold/util/vertex.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

inline void gold_init()
{
	#ifndef _NDEBUG
	AllocConsole();

	SetConsoleTitleA("Gold Debug");
	DeleteMenu(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_BYCOMMAND);

	static auto gold_consoleout    = std::ofstream("CONOUT$");
	std::cout.rdbuf(gold_consoleout.rdbuf());
	std::cout.clear();
	#endif
}