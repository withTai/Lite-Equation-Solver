//���ļ������˷������������ʱ��������
#pragma once
#include "formula.h"
void Equlation()
{
	//system("color F0");
	string tmp, str;
	Num aa, bb, cc, dd, xx, yy, zz;
	cout << "��ӭʹ�÷����������" << endl << "��ΪһԪ���̣��������� x ��ʾ����Ϊ��Ԫ���̣��������� x+����(��x1) ��ʾ." << endl << "�����뷽�̣�" << endl;
	cin >> tmp;
	JudgeType(tmp);
	if (type == 2)
	{
		Matrix(tmp);
	}
	if (type == 1) //һԪ��η���
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