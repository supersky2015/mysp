#ifndef COMMON_PATH_H
#define COMMON_PATH_H

#include <vector>
#include <string>
#include <windows.h>

using namespace std;

namespace Common{
/*
 * get directory where executable is in.
 */
//string GetExeDir(int parentLevel = 0);

/*
 * get current working directory.
 */
//string GetCurrentDir();

//string GetPathFolderName(const string& path);

//string GetPathFilename(const string& path);

//string GetPathExtension(const string& path);


wstring w = GetPathFullname<wchar_t>(L"c:\abc.txt");
string c = GetPathFullname<char>("c:\abc.txt");


template<typename T>
basic_string<T, char_traits<T>, allocator<T>> GetPathFullname(const T* path)
{
	T drive[_MAX_DRIVE];
	T dir[_MAX_DIR];
	T fname[_MAX_FNAME];
	T ext[_MAX_EXT];
	
	if (typeid(T).name() == "char")
		_splitpath(reinterpret_cast<const char*>(path), drive, dir, fname, ext);
	else
		_wsplitpath(reinterpret_cast<const wchar_t*>(path), drive, dir, fname, ext);

	return basic_string<T>(fname) + basic_string<T>(ext);
}

}

#endif
