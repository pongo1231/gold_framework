#pragma once

#include <sys/stat.h>

#include <cstdio>
#include <string_view>

class gold_filehandle
{
  public:
	FILE *const handle;

	gold_filehandle(FILE *handle) : handle(handle)
	{
	}

	~gold_filehandle()
	{
		fclose(handle);
	}

	gold_filehandle(const gold_filehandle &)            = delete;

	gold_filehandle &operator=(const gold_filehandle &) = delete;
};

namespace gold_file
{
	inline bool does_file_exist(std::string_view filename)
	{
		struct stat temp;
		return stat(filename.data(), &temp) == 0;
	}

	inline gold_filehandle open_file(std::string_view filename, std::string_view mode = "rw")
	{
		auto file = fopen(filename.data(), mode.data());
		return file;
	}
}