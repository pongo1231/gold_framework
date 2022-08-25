#pragma once

#include "gold/gold.h"

struct HINSTANCE__;
typedef HINSTANCE__ *HINSTANCE;

enum class error_code;

class gold_game
{
  private:
	gold_unique_ptr<gold_graphicsdevice> graphics_device;
	gold_unique_ptr<gold_camera> camera;

	bool did_init = false;

	gold_entity* start_platform;
	gold_entity* cube;
	gold_entity *player;
	gold_entity *platform1;
	gold_entity *platform2;
	gold_entity *platform3;
	gold_unique_ptr<gold_skybox> skybox;
	gold_unique_ptr<gold_model> model;
	gold_unique_ptr<gold_model> model2;

  public:
	error_code init(HINSTANCE inst);
	error_code run();

	gold_camera *get_camera() const;
};