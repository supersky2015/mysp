#ifndef PLOT_H
#define PLOT_H

#include <string>
#include <vector>
#include <ngspice/sharedspice.h>
using namespace std;

/*
* 向量-vec	：在ngspice里面代表一个计算点，包括等势点，电流分支
* 图-plot	：代表一次仿真指令的结果集合，可以通过 .tran .dc .ac 等仿真指令
*/
struct plot
{
	//基本属性
	string title;
	string name;
	string date;
	string type;

	/*
	 * 向量的个数
	 * 下面成员具有相同的veccount值.
	 *		pvecs,currentValues,names,pvalues,values 
	 */
	int veccount;		

	/*
	 * 各个向量的计算值的个数， 对应于VecPArray, VecArray的size 
	 * 注：一个plot里面所有向量都有一样size的value
	 */
	int vecsize;		

	//初始化ngspice分配的结构指针
	pvecinfo* pvecs;

	//存储所有向量当前值，方便取用
	vector<double> currentValues;		
	
	//存储所有向量名
	vector<string> names;	
	
	/*
	 * 一个向量历次计算值数组,长度为vecsize, 使用指针可以共享ngspice计算值存储空间 - 定义为 VecArray类型
	 * pvalues 存储所有向量VecArray
	 */
	typedef double* VecPArray;			
	vector<VecPArray> pvalues;			
	
	/*
	 * values 等同于 pvalues，不同之处在于 pvalues指向ngspice分配的内存，values自己分配管理
	 * 一个向量历次计算值数组，计算量大的时候内存开销会很大.
	 * 暂不使用
	 */
	typedef vector<double> VecArray;	
	vector<VecArray> values; 

	/*
	 * 根据向量名获取当前向量值
	 */
	double plot::GetVecCurrValue( const string& name );

};


#endif