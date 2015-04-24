#include "stdafx.h"
#include "path.h"
#include <direct.h>

namespace Common{
	string GetExeDir(int parentLevel /*= 0*/)
	{
		char path[MAX_PATH] = {0};
		if (!::GetModuleFileNameA(NULL, path, sizeof(path)))
			return "";

		char* tmp = strrchr(path, '\\');
		if (tmp)
			*(tmp + 1) = '\0';

		while (parentLevel--) {
			*tmp = '\0';
			tmp = strrchr(path, '\\');
			if (!tmp) {
				path[strlen(path)] = '\\';
				break;
			}else
				*(tmp + 1) = '\0';
		}

		return string(path);
	}

	/*get current working directory*/
	string GetCurrentDir()
	{
		char path[FILENAME_MAX] = {0};
		//if (!_getcwd(path, sizeof(path)))
		if (!GetCurrentDirectoryA(sizeof(path), path))
			return "";
		path[strlen(path)] = '\\';
		return string(path);
	}

	string GetPathFolderName(const string& path)
	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		//_splitpath(path.c_str(), drive, dir, fname, ext);
		_splitpath_s(path.c_str(), drive, dir, fname, ext);

		string folderName;
		string directory(dir);
		string::size_type end = directory.find_last_of('\\');
		if (end != string::npos) {
			string tmp = directory.substr(0, end);
			string::size_type beg = tmp.find_last_of('\\');
			if (beg != string::npos && beg < end)
				folderName = directory.substr(beg + 1, end - beg - 1);
		}
		if (folderName.empty() && strlen(drive) != 0)
			folderName.append(1, drive[0]);

		return folderName;
	}

	string GetPathFilename(const string& path)
	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath(path.c_str(), drive, dir, fname, ext);
		return string(fname);
	}

	string GetPathExtension(const string& path)
	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath(path.c_str(), drive, dir, fname, ext);
		return string(ext);
	}

	string GetPathFullname(const string& path)
	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath(path.c_str(), drive, dir, fname, ext);
		return string(fname) + string(ext);
	}

}

