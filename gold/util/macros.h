#pragma once

#include <iostream>

#define DEMO_CLASSNAME TEXT("framework")
#define DEMO_TITLENAME TEXT("Framework")

#define LOG(x) std::cout << x << "\n"
#define LOG_ERROR(x) LOG("Error: " << x)

#define _NODISCARD [[nodiscard]]