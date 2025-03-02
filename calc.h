//此文件定义了方程求解器运行时的主函数
#pragma once
#include "formula.h"
void Equlation()
{
	//system("color F0");
	string tmp, str;
	Num aa, bb, cc, dd, xx, yy, zz;
	cout << "欢迎使用方程求解器！" << endl << "若为一元方程，变量请用 x 表示；若为多元方程，变量请用 x+数字(如x1) 表示." << endl << "请输入方程：" << endl;
	cin >> tmp;
	JudgeType(tmp);
	if (type == 2)
	{
		Matrix(tmp);
	}
	if (type == 1) //一元多次方程
	{
		int t = isQuardratic(tmp, xx, yy, zz);
		if (isCubic(tmp, aa, bb, cc, dd))
		{
			cout << endl;
			Cubic(aa, bb, cc, dd);
		}
		else if (t == 1)
		{
			cout << endl;
			Quardratic(xx, yy, zz);
		}
		else if (t == -1)
		{
			cout << endl;
			cout << "x=" << -zz / yy << endl;
		}
		else
		{
			Num init;
			init = 2.33;
			cout << endl;
			cout << "x=" << NewtonIteration(tmp, init) << endl;
		}
	}
}