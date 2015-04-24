// ingspice.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <stdio.h>

int global_var = 0;

extern "C"
{
__declspec(dllexport)
void test_if_global_var_changed()
{
	printf("get %08X : %d\n", GetModuleHandle(NULL), global_var);
}

__declspec(dllexport)
void set_global_var(int i)
{
	printf("set %08X \n", GetModuleHandle(NULL));
	global_var = i;
}
};
