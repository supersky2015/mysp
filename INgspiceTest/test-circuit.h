#ifndef TEST_CIRCUIT_H
#define TEST_CIRCUIT_H

// 测试是否重新开妈仿真
void test_restart();

// 简单rc充电电路
void test_circuit_rc();

// 测试LED亮灭动画
void test_flash_led();

// 多例并行仿真
void test_circuit_parallel();

// 使用电阻作为开关切换电路（修改了ngspice dll一处问题，原来不支持在仿真中修改电阻）
void test_switch_led();

// 封装电流控制开关到子电路
// 测试通过修改子电路中电流来控制开关开闭
// by interactive mode
void test_switch_via_subckt_inter();

// by shared-lib
void test_switch_via_subckt();

#endif