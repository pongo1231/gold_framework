#pragma once

#include <iostream>
#include <fstream>

#define GOLD_CLASSNAME TEXT("gold_framework")

#ifdef _NDEBUG
#define LOG(x) std::cout << x << "\n";
#else
#define LOG(x) std::cout << x << std::endl;
#endif
#define LOG_ERROR(x) LOG("Error: " << x)