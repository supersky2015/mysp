#ifndef TIMER_H
#define TIMER_H

#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")//timeGetTime

namespace Common{

class timer
{
public:
	timer()
		:highPrecision(true)
	{	
		highPrecision=true;
		QueryPerformanceFrequency(&frequency);	//获得计时器的时钟频率			
		QueryPerformanceCounter(&start);		//1us
		if (0 == start.QuadPart) {				
			OutputDebugStringA("INFO: timer does not support high-precision!");
			highPrecision = false;
			start.LowPart = timeGetTime();		//1ms
		}
	}

	~timer()
	{
	}

	//单位为s
	double Escape()
	{
		if (highPrecision) {
			LARGE_INTEGER end;
			QueryPerformanceCounter(&end);
			return (end.QuadPart - start.QuadPart)/(double)frequency.QuadPart;
		}
		else {	
			return (timeGetTime() - start.LowPart)/(double)1000;
		}
	}

	//重新记录开始时间
	void Reset()
	{
		if (highPrecision) {
			QueryPerformanceCounter(&start);
		}
		else {
			start.LowPart = timeGetTime();
		}
	}

private:
	LARGE_INTEGER start;
	LARGE_INTEGER frequency;

	bool highPrecision;
};

}

#endif
