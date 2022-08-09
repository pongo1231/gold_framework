#pragma once

#include "gold/camera.h"
#include "gold/graphics/graphics_device.h"
#include "gold/graphics/model/primitives/cube.h"
#include "gold/graphics/model/primitives/skybox.h"
#include "gold/input.h"
#include "gold/memory.h"

#include <memory>

struct HINSTANCE__;
typedef HINSTANCE__ *HINSTANCE;

enum class error_code;

class gold_game
{
  private:
	std::unique_ptr<gold_graphicsdevice> graphics_device;
	std::unique_ptr<gold_camera> camera;

	bool did_init = false;

	gold_cube *plane;
	gold_cube *cube;
	std::unique_ptr<gold_skybox> skybox;
	std::unique_ptr<gold_model> model;
	std::unique_ptr<gold_model> model2;

  public:
	error_code init(HINSTANCE inst);
	error_code run();

	gold_camera *get_camera() const;
};