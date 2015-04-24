// INgspice.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <stdio.h>

__declspec(dllexport) void test_if_global_var_changed();

__declspec(dllexport) void set_global_var(int i);