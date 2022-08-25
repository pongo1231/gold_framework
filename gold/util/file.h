#pragma once

#include <sys/stat.h>

#include <cstdio>
#include <string_view>

/*
* RAII wrapper for file handle
*/
class gold_filehandle
{
  public:
	/*
	* File handle
	*/
	FILE *const handle;

	/*
	* Constructor
	* <param name="handle">Handle of file</param>
	*/
	gold_filehandle(FILE *handle) : handle(handle)
	{
	}

	/*
	* Destructor
	*/
	~gold_filehandle()
	{
		fclose(handle);
	}

	// We don't need a copy constructor or operator
	gold_filehandle(const gold_filehandle &)            = delete;
	gold_filehandle &operator=(const gold_filehandle &) = delete;
};

/*
* File helper functions
*/
namespace gold_file
{
	/*
	* <param name="filename">File path</param>
	* <returns>Whether file exists</returns>
	*/
	inline bool does_file_exist(std::string_view filename)
	{
		struct stat temp;
		return stat(filename.data(), &temp) == 0;
	}

	/*
	* <param name="filename">File path</param>
	* <param name="mode">Mode to open file in, default is "rw" (read+write)</param>
	* <returns>File handle</returns>
	*/
	inline gold_filehandle open_file(std::string_view filename, std::string_view mode = "rw")
	{
		auto file = fopen(filename.data(), mode.data());
		return file;
	}
}