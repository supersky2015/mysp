#include "stdafx.h"
#include "string.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#include <wtypes.h>
#include <stdlib.h>
#include <locale.h>
#include <direct.h>

namespace Common{
	string WS2S(const wstring& ws)
	{
		string curLocale = setlocale(LC_ALL, "chs");        // curLocale = "C";
		//setlocale(LC_ALL, "chs");
		const wchar_t* _Source = ws.c_str();
		size_t _Dsize = 2 * ws.size() + 1;
		char *_Dest = new char[_Dsize];
		memset(_Dest,0,_Dsize);
		wcstombs(_Dest,_Source,_Dsize);
		string result = _Dest;
		delete []_Dest;
		setlocale(LC_ALL, curLocale.c_str());
		return result;
	}

	wstring S2WS(const string& s)
	{
		setlocale(LC_ALL, "chs"); 
		const char* _Source = s.c_str();
		size_t _Dsize = s.size() + 1;
		wchar_t *_Dest = new wchar_t[_Dsize];
		wmemset(_Dest, 0, _Dsize);
		mbstowcs(_Dest,_Source,_Dsize);
		wstring result = _Dest;
		delete []_Dest;
		setlocale(LC_ALL, "C");
		return result;
	}

	wchar_t* _char_to_wchar(const char* src, int src_len, int charset, int& des_len)
	{
		if (src == NULL || 0 == src_len)
		{
			return NULL;
		}

		des_len = MultiByteToWideChar(charset, 0, src, src_len, NULL, NULL );

		if (des_len == 0)
			return NULL;

		wchar_t* des = new wchar_t[des_len + 1];

		if (des == NULL)
			return NULL;

		if (0 == MultiByteToWideChar(charset, 0, src, src_len, des, des_len))
		{
			delete [] des;
			return NULL;
		}

		des[des_len] = 0;

		return des;
	}

	char* _wchar_to_char(const wchar_t* src, int src_len, int charset, int& des_len)
	{
		if (src == NULL || 0 == src_len)
		{
			return NULL;
		}

		if (src_len==-1)
		{
			src_len = (DWORD)wcslen(src);
		}

		des_len = WideCharToMultiByte(charset, NULL, src, src_len, NULL, NULL, NULL, NULL);

		if(des_len==0)
		{
			return NULL;
		}

		char * des = new char[des_len + 1];
		memset(des, 0, des_len+1);

		if (des == NULL)
		{
			return NULL;
		}

		if(0 == WideCharToMultiByte(charset, NULL, src, src_len, des, des_len,NULL,NULL))
		{
			delete [] des;
			return NULL;
		}

		des[des_len] = 0;
		return	des;
	}

	wchar_t* _char_to_wchar(const char* src, int src_len, int charset)
	{
		if (src == NULL)
		{
			return NULL;
		}

		int des_len = MultiByteToWideChar(charset, 0, src, src_len, NULL, NULL );

		if (des_len == 0)
			return NULL;

		wchar_t* des = new wchar_t[des_len + 1];

		if (des == NULL)
			return NULL;

		if (0 == MultiByteToWideChar(charset, 0, src, src_len, des, des_len))
		{
			delete [] des;
			return NULL;
		}

		des[des_len] = 0;

		return des;
	}

	char* _wchar_to_char(const wchar_t* src, int src_len, int charset)
	{
		if (src == NULL)
		{
			return NULL;
		}

		if (src_len==-1)
		{
			src_len = (DWORD)wcslen(src);
		}

		int des_len = WideCharToMultiByte(charset, NULL, src, src_len, NULL, NULL, NULL, NULL);

		if(des_len==0)
		{
			return NULL;
		}

		char * des = new char[des_len + 1];
		memset(des, 0, des_len+1);

		if (des == NULL)
		{
			return NULL;
		}

		if(0 == WideCharToMultiByte(charset, NULL, src, src_len, des, des_len,NULL,NULL))
		{
			delete [] des;
			return NULL;
		}

		des[des_len] = 0;
		return	des;
	}

	wstring StringToWstring(const string& src, int charset)
	{
		int des_len = 0;
		wchar_t* des = _char_to_wchar(src.c_str(), src.length(), charset, des_len);
		wstring des_string(des, des_len);
		delete []des;
		return des_string;
	}

	string WstringToString(const wstring& src, int charset)
	{
		int des_len = 0;
		char* des = _wchar_to_char(src.c_str(), src.length(), charset, des_len);
		string des_string(des, des_len);
		delete []des;
		return des_string;
	}

	string ConvertString( const string& src, int old_charset, int new_charset)
	{
		wstring des = StringToWstring(src, old_charset);
		return WstringToString(des, new_charset);
	}

	string& ReplaceAll(string& str, const string& old_value, const string& new_value)
	{   
		while(true)
		{
			string::size_type pos(0);
			if ((pos = str.find(old_value)) != string::npos)
				str.replace(pos, old_value.length(), new_value);
			else
				break;
		}
		return str;
	}

	string& ReplaceAllDistinct(string& str, const string& old_value, const string& new_value)
	{
		for(string::size_type pos(0); pos != string::npos; pos += new_value.length() )
		{
			if ((pos = str.find(old_value,pos)) != string::npos)
				str.replace(pos, old_value.length(), new_value);
			else
				break;
		}
		return str;
	}

	vector<string> Split(string str, string pattern)
	{
		string::size_type pos;
		vector<string> result;
		str += pattern;
		size_t size = str.size();

		for(size_t i = 0; i < size; i++)
		{
			pos = str.find(pattern, i);
			if (pos < size)
			{
				string s=str.substr(i, pos - i);
				result.push_back(s);
				i= pos + pattern.size() - 1;
			}
		}
		return result;
	}

	vector<wstring> Split( wstring str, wstring pattern )
	{
		wstring::size_type pos;
		vector<wstring> result;
		str += pattern;
		size_t size = str.size();

		for(size_t i = 0; i < size; i++)
		{
			pos = str.find(pattern, i);
			if (pos < size)
			{
				wstring s=str.substr(i, pos - i);
				if (s.size())
					result.push_back(s);
				i= pos + pattern.size() - 1;
			}
		}
		return result;
	}

	string FormatString(int nSize, const char* szFormat, ...)
	{
		va_list ap;
		char* pBuf = reinterpret_cast<char*>(_malloca(nSize));
		va_start(ap, szFormat);
		vsprintf(pBuf, szFormat, ap);
		va_end(ap);
		string ret(pBuf);
		_freea(pBuf);
		return ret;
	}

	wstring FormatString(int nSize, const wchar_t* szFormat, ...)
	{
		va_list ap;
		wchar_t* pBuf = new wchar_t[nSize];
		va_start(ap, szFormat);
		vswprintf_s(pBuf, nSize, szFormat, ap);
		va_end(ap);
		wstring ret(pBuf);
		delete []pBuf;
		return ret;
	}

	string AppendFormat( string& pre, int size, const char* format, ... )
	{
		char* buf = new char[size];
		memset(buf, 0, size);
		va_list ap;
		va_start(ap, format);
		vsprintf_s(buf, size, format, ap);
		va_end(ap);
		string append(buf);
		delete []buf;
		pre += append;
		return pre;
	}

	std::wstring AppendFormat( wstring& pre, int size, const wchar_t* format, ... )
	{
		wchar_t* buf = new wchar_t[size];
		wmemset(buf, 0, size);
		va_list ap;
		va_start(ap, format);
		vswprintf_s(buf, size, format, ap);
		va_end(ap);
		wstring append(buf);
		delete []buf;
		pre += append;
		return pre;
	}

	void OutputDebugMessage(const wchar_t* fmt, ...)
	{
		va_list ap;
		wchar_t buf[1024] = {0};
		va_start(ap, fmt);
		vswprintf_s(buf, 1024, fmt, ap);
		wcscat_s(buf, 1024, L"\n");
		va_end(ap);

		OutputDebugStringW(buf);
	}

	void OutputDebugMessage(const char* fmt, ...)
	{
		va_list ap;
		char buf[512] = {0};
		va_start(ap, fmt);
		vsprintf_s(buf, 512, fmt, ap);
		strcat_s(buf, 512, "\n");
		va_end(ap);

		OutputDebugStringA(buf);
	}

	void OutputDebugMessage(int size, const wchar_t* fmt, ...)
	{
		va_list ap;
		wchar_t* buf = new wchar_t[size];
		va_start(ap, fmt);
		vswprintf_s(buf, size, fmt, ap);
		wcscat_s(buf, size, L"\n");
		va_end(ap);

		OutputDebugStringW(buf);
		delete []buf;
		buf = NULL;
	}

	void OutputDebugMessage(int size, const char* fmt, ...)
	{
		va_list ap;
		char* buf = new char[size];
		va_start(ap, fmt);
		vsprintf_s(buf, size, fmt, ap);
		strcat_s(buf, size, "\n");
		va_end(ap);

		OutputDebugStringA(buf);
		delete []buf;
		buf = NULL;
	}

	string Joint(vector<string>& strings, string pattern)
	{
		string ret;
		size_t size = strings.size();
		for (size_t i = 0; i < size; i++)
		{
			ret += strings[i];
			if (i != size - 1)
				ret += pattern;
		}
		return ret;
	}

	wstring Joint(vector<wstring>& strings, wstring pattern){
		wstring ret;
		size_t size = strings.size();
		for (size_t i = 0; i < size; i++){
			ret += strings[i];
			if (i != size - 1)
				ret += pattern;
		}
		return ret;
	}

}

