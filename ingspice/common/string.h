#ifndef COMMON_STRING_H
#define COMMON_STRING_H

#include <vector>
#include <string>
#include <windows.h>

using namespace std;

namespace Common{

string WS2S(const wstring& ws);

wstring S2WS(const string& s);

wstring StringToWstring(const string& src, int charset = CP_ACP);

string WstringToString(const wstring& src, int charset = CP_ACP);

string ConvertString(const string& src, int old_charset, int new_charset);

string& ReplaceAll(string& str, const string& old_value, const string& new_value);

string& ReplaceAllDistinct(string& str, const string& old_value, const string& new_value);

vector<wstring> Split(wstring str, wstring pattern);

vector<string> Split(string str, string pattern);

string Joint(vector<string>& strings, string pattern);

wstring Joint(vector<wstring>& strings, wstring pattern);

string FormatString(int nSize, const char* szFormat, ...);

wstring FormatString(int nSize, const wchar_t* szFormat, ...);

string AppendFormat(string& pre, int size, const char* format, ...);

wstring AppendFormat(wstring& pre, int size, const wchar_t* format, ...);

void OutputDebugMessage(const wchar_t* fmt, ...);

void OutputDebugMessage(const char* fmt, ...);

void OutputDebugMessage(int size, const wchar_t* fmt, ...);

void OutputDebugMessage(int size, const char* fmt, ...);

}

#endif
