// INgspice.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include <stdio.h>

__declspec(dllexport) void test_if_global_var_changed();

__declspec(dllexport) void set_global_var(int i);