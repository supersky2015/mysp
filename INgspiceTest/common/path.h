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
string GetExeDir(int parentLevel = 0);

/*
 * get current working directory.
 */
string GetCurrentDir();

string GetPathFolderName(const string& path);

string GetPathFilename(const string& path);

string GetPathExtension(const string& path);

string GetPathFullname(const string& path);

}

#endif