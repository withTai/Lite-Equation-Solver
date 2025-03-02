//此文件定义了方程求解的数学模型用于方程计算，同时创建了部分解决工具函数，增强了与其他文件的适配性
#pragma once
#include "stack.h"
void clearChar()
{
	memset(a, '\0', sizeof(a));
}

void ReplaceXx(Num value, string& tmp, string& str)
{
	for (int i = 0; i < tmp.size(); ++i)
	{
		if (tmp[i] != 'x' && tmp[i] != 'X')
		{
			str += tmp[i];
		}
		else
		{
			str += string(value);
		}
	}
}

void writeChar(string& str)
{
	for (int i = 0; i < str.size(); ++i)
	{
		a[i] = str[i];
	}
}

Num derive(string& tmp, Num t)
{
	string str = "";
	m_data* head = 0;
	Num ans[2], x[2], accuracy;
	accuracy = 1;
	accuracy.exp = -16;
	x[0] = t;
	x[1] = t + accuracy;
	LinkStack expt = NULL, optr = NULL;
	for (int i = 0; i < 2; ++i)
	{
		str.clear();
		clearChar();
		ReplaceXx(x[i], tmp, str);
		writeChar(str);
		Exp = a;
		while (getsym())
		{
			head = addtail(head, symbol);
		}
		head = changedata(head);
		InitExpTree(head, expt, optr);
		BiTree T = NULL;
		Pop_tree(expt, T);
		ans[i] = EvaluateExpTree(T);
		head = NULL;
		expt = NULL;
		optr = NULL;
	}
	return (ans[1] - ans[0]) / accuracy;
}

Num NewtonIteration(string& tmp, Num t)
{
	string str = "";
	m_data* head = 0;
	Num y, accuracy;
	accuracy = 1;
	accuracy.exp = -8;
	Num value1 = t, value2 = t;
	LinkStack expt = NULL, optr = NULL;
	do
	{
		str.clear();
		clearChar();
		value1 = value2;
		ReplaceXx(value1, tmp, str);
		writeChar(str);
		Exp = a;
		while (getsym())
		{
			head = addtail(head, symbol);
		}
		head = changedata(head);
		InitExpTree(head, expt, optr);
		BiTree T = NULL;
		Pop_tree(expt, T);
		y = EvaluateExpTree(T);
		value2 = value1 - y / derive(tmp, value1);
		value2.Rounding(16);
		head = NULL;
		expt = NULL;
		optr = NULL;
	} while (abs(value1 - value2) > accuracy);
	value1.Rounding(8);
	return value1;
}

double XLS(int n, double p[][100])         //矩阵行列式
{
	double xls = 0;
	int N = n;
	if (n == 2)
		xls = (p[0][0] * p[1][1] - p[0][1] * p[1][0]);
	else if (n == 1)
		xls = p[0][0];
	else if (n > 2)
	{
		for (int i = 0; i < n; i++)
		{
			int t = 1, m = 0;
			double ai[100][100]{};
			for (int j = 0; j < (n - 1); j++)
			{
				for (int k = 0; k < (n - 1); k++)
				{

					if (m != i)
					{
						ai[j][k] = p[t][m];
						m++;
					}
					else
					{
						m += 1;
						ai[j][k] = p[t][m];
						m++;
					}
				}
				t += 1;
				m = 0;
			}
			if ((i + 2) % 2 == 0)
				xls += p[0][i] * XLS(n - 1, ai);
			else
				xls -= p[0][i] * XLS(n - 1, ai);

		}
		n -= 1;
	}
	n = N;
	return xls;
}

void Matrix(string& tmp)
{
	writeChar(tmp);
	m_data* head = 0;
	while (getsym())
	{
		head = addtail(head, symbol);
	}
	int i = 0, count = 0;
	do
	{
		addce(head, i, count);

		if (i >= count - 1)
			break;
		cin >> Exp;
		head = 0;
		while (getsym())
			head = addtail(head, symbol);
	} while (1);
	cout << endl;
	//ce组成为前n-1列为系数矩阵，第n列为等号右边的数值。即ce是增广矩阵
	int n = i;
	double XLS(int n, double p[][100]);
	double a[100][100]{};    //方程系数
	double ab[100][100]{};   //a逆矩阵
	double c[100][1]{};      //方程常数
	double x[100][1]{};      //方程的解
	for (int I = 0; I < n; I++)
	{
		for (int j = 0; j < n; j++)
		{
			a[I][j] = ce[I][j];
		}
	}
	for (int I = 0; I < n; I++)
	{
		c[I][0] = ce[I][count - 1];
	}

	double Xls = XLS(n, a);


	if (Xls != 0)
	{



		double b[100][100]{};
		if (n > 2)
		{

			for (int I = 0; I < n; I++)
			{
				for (int j = 0; j < n; j++)
				{
					double T[100][100]{};
					int w = 0, v = 0;
					for (int t = 0; t < n - 1; t++)
					{
						if (t == I)
							w += 1;
						for (int m = 0; m < n - 1; m++)
						{
							if (m == j)
								v += 1;
							T[t][m] = a[w][v];
							v++;
						}
						w++;
						v = 0;
					}
					b[j][I] = XLS(n - 1, T);

				}
			}

			for (int I = 0; I < n; I++)
			{
				for (int j = 0; j < n; j++)
				{
					if ((I + j + 2) % 2 == 0)
						ab[I][j] = b[I][j] / Xls;
					else
						ab[I][j] = -b[I][j] / Xls;
				}
			}
		}
		else if (n == 2)
		{
			ab[0][0] = a[1][1] / Xls;
			ab[0][1] = -a[1][0] / Xls;
			ab[1][0] = -a[0][1] / Xls;
			ab[1][1] = a[0][0] / Xls;
		}
		else
		{
			ab[0][0] = a[0][0];

		}
		for (int i = 0; i < n; i++)
		{
			double X = 0;
			for (int j = 0; j < n; j++)
			{
				X += ab[i][j] * c[j][0];

			}
			x[i][0] = X;

			cout << "X" << i + 1 << "=" << x[i][0] << '\n';
		}
	}
	else
	{
		double m[100][100]{};
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n + 1; j++)
			{
				m[i][j] = ce[i][j];
			}
		}
		for (int i = 0; i < n; i++)
		{
			while (m[i][i] == 0)
			{
				for (int j = i; j < n; j++)
				{
					double p3[100][100]{};
					for (int t = 0; t < n + 1; t++)
					{
						p3[j][t] = m[j][t];
						m[j][t] = m[j + 1][t];
						m[j + 1][t] = p3[j][t];
					}
				}
			}
			if (m[i][i] == 0)
			{
				cout << "方程无解" << endl;
			}
			else
			{
				double a = m[i][i];
				for (int t = 0; t < n + 1; t++)
				{
					m[i][t] /= a;
				}
			}
			for (int k = i + 1; k < n; k++)
			{
				for (int t = 0; t < n + 1; t++)
				{
					m[k][t] -= m[i][t] * m[k][i];
				}
			}
		}
		cout << "方程组有多个解，特解为：" << '\n';
		for (int i = 0; i < n; i++)
		{
			cout << "x" << i + 1 << "=" << m[i][n] << '\n';
		}
	}

	dellist(head);
}

int isQuardratic(string& tmp, Num& aa, Num& bb, Num& cc)
{
	//如果是一元二次方程，则返回真，并将一元二次方程系数提取至各个参数中，若不是，则返回假，参数赋值为0
	string w;
	string& str = w;
	m_data* head = 0;
	Num ans[3], x, accuracy;
	accuracy = 1;
	accuracy.exp = -12;
	Num Number[10];
	LinkStack expt = NULL, optr = NULL;
	for (int i = 0; i < 10; ++i)
	{
		Number[i] = i;
	}
	for (int i = 1; i < 4; ++i)
	{
		str.clear();
		clearChar();
		ReplaceXx(Number[i], tmp, str);
		writeChar(str);
		Exp = a;
		while (getsym())
		{
			head = addtail(head, symbol);
		}
		head = changedata(head);
		InitExpTree(head, expt, optr);
		BiTree T = NULL;
		Pop_tree(expt, T);
		ans[i - 1] = EvaluateExpTree(T);
		ans[i - 1].Rounding(16);
		head = NULL;
		expt = NULL;
		optr = NULL;
	}
	aa = (ans[0] - Number[2] * ans[1] + ans[2]) / Number[2];
	bb = (-Number[5] * ans[0] + Number[8] * ans[1] - Number[3] * ans[2]) / Number[2];
	cc = ans[0] - aa - bb;
	if ((double)aa == 0)
	{
		aa = Zero;
		return -1;
	}
	for (int i = 4; i < 7; ++i)
	{
		str.clear();
		clearChar();
		ReplaceXx(Number[i], tmp, str);
		writeChar(str);
		Exp = a;
		while (getsym())
		{
			head = addtail(head, symbol);
		}
		head = changedata(head);
		InitExpTree(head, expt, optr);
		BiTree T = NULL;
		Pop_tree(expt, T);
		x = EvaluateExpTree(T);
		if (!(abs(aa * Number[i] * Number[i] + bb * Number[i] + cc - x) < accuracy))
		{
			head = NULL;
			aa = Zero;
			bb = Zero;
			cc = Zero;
			return 0;
		}
		head = NULL;
		expt = NULL;
		optr = NULL;
	}
	return 1;
}

bool isCubic(string& tmp, Num& aa, Num& bb, Num& cc, Num& dd)
{
	string w;
	string& str = w;
	m_data* head = 0;
	Num ans[4], x, accuracy;
	accuracy = 1;
	accuracy.exp = -8;
	Num Number[20];
	LinkStack expt = NULL, optr = NULL;
	for (int i = 0; i < 20; ++i)
	{
		Number[i] = i;
	}
	for (int i = 0; i < 4; ++i)
	{
		str.clear();
		clearChar();
		ReplaceXx(Number[i], tmp, str);
		writeChar(str);
		Exp = a;
		while (getsym())
		{
			head = addtail(head, symbol);
		}
		head = changedata(head);
		InitExpTree(head, expt, optr);
		BiTree T = NULL;
		Pop_tree(expt, T);
		ans[i] = EvaluateExpTree(T);
		ans[i].Rounding(16);
		head = NULL;
		expt = NULL;
		optr = NULL;
	}
	aa = (-Number[1] * ans[0] + Number[3] * ans[1] - Number[3] * ans[2] + Number[1] * ans[3]) / Number[6];
	bb = (Number[2] * ans[0] - Number[5] * ans[1] + Number[4] * ans[2] - Number[1] * ans[3]) / Number[2];
	cc = (-Number[11] * ans[0] + Number[18] * ans[1] - Number[9] * ans[2] + Number[2] * ans[3]) / Number[6];
	dd = Number[1] * ans[0];
	if ((double)(aa) == 0)
	{
		aa = Zero;
		bb = Zero;
		cc = Zero;
		dd = Zero;
		return false;
	}
	for (int i = 4; i < 8; ++i)
	{
		str.clear();
		clearChar();
		ReplaceXx(Number[i], tmp, str);
		writeChar(str);
		Exp = a;
		while (getsym())
		{
			head = addtail(head, symbol);
		}
		head = changedata(head);
		InitExpTree(head, expt, optr);
		BiTree T = NULL;
		Pop_tree(expt, T);
		x = EvaluateExpTree(T);
		if (!(abs(aa * Number[i] * Number[i] * Number[i] + bb * Number[i] * Number[i] + cc * Number[i] + dd - x) < accuracy))
		{
			head = NULL;
			aa = Zero;
			bb = Zero;
			cc = Zero;
			dd = Zero;
			return false;
		}
		head = NULL;
		expt = NULL;
		optr = NULL;
	}
	return true;
}

void Quardratic(Num a, Num b, Num c)//一元二次方程
{
	double delta;
	double x1, x2;
	double aa, bb, cc;
	aa = (double)a;
	bb = (double)b;
	cc = (double)c;
	delta = bb * bb - 4 * aa * cc;
	if (delta > 0)//两实数根
	{

		x1 = (-bb + sqrt(delta)) / (2 * aa);
		x2 = (-bb - sqrt(delta)) / (2 * aa);
		cout << "有两个不同的实数根" << endl;
		cout << "x1 = " << x1 << '\t' << "x2 = " << x2 << endl;
	}
	else if (delta == 0)//一实数根
	{
		x1 = -bb / (2 * aa);
		cout << "有两个相同的实数根" << endl;
		cout << "x1 = x2 = " << x1 << endl;
	}
	else if (delta < 0)//两复数根
	{
		double r, i;
		r = -bb / (2 * aa);
		i = sqrt(-delta) / (2 * aa);
		if (r == 0)
		{
			cout << "有两个相同的虚数根" << endl;
			cout << "x1 = " << i << "i" << '\t' << "x2 = " << -i << "i" << endl;
		}
		else
		{
			cout << "有两个相同的复数根" << endl;
			cout << "x1 = " << r << " + " << i << "i" << '\t' << "x2 = " << r << " - " << i << "i" << endl;
		}
	}
}

void Cubic(Num a, Num b, Num c, Num d)
{
	double A, B, C, delta, x1, x2, x3;
	double aa, bb, cc, dd;
	aa = (double)a;
	bb = (double)b;
	cc = (double)c;
	dd = (double)d;
	A = bb * bb - 3 * aa * cc;
	B = bb * cc - 9 * aa * dd;
	C = cc * cc - 3 * bb * dd;
	delta = B * B - 4 * A * C;
	if (A == 0 && B == 0 && (bb != 0 || cc != 0))
	{
		x1 = -bb / (3 * aa);
		cout << "有三个相同的实数根" << endl;
		cout << "x1 = x2 = x3 = " << x1 << endl;
	}
	else if (bb == 0 && cc == 0)
	{
		cout << "有三个相同的实数根" << endl;
		if (dd / aa < 0)
			cout << "x1=x2=x3=" << pow(-dd / aa, 1.0 / 3) << endl;
		else
			cout << "x1=x2=x3=" << -pow(dd / aa, 1.0 / 3) << endl;
	}
	else if (delta > 0)
	{
		double Y1, Y2;
		Y1 = (A * bb + 3 * aa * (-B + sqrt(B * B - 4 * A * C)) / 2);
		Y2 = (A * bb + 3 * aa * (-B - sqrt(B * B - 4 * A * C)) / 2);
		double B, C;
		if (Y1 >= 0)
			B = pow(Y1, 1.0 / 3);
		else
			B = -pow(-Y1, 1.0 / 3);
		if (Y2 >= 0)
			C = pow(Y2, 1.0 / 3);
		else
			C = -pow(-Y2, 1.0 / 3);
		x1 = (-bb - (B + C)) / (3 * aa);
		double r = (-bb + (B + C) / 2) / (3 * aa);
		double i = (sqrt(3) / 2 * (B - C)) / (3 * aa);
		cout << "有一个实根两个虚根" << endl;
		cout << "x1=" << x1 << endl;
		cout << "x2=" << r << " + " << i << "i" << endl;
		cout << "x3=" << r << " - " << i << "i" << endl;
	}
	else if (delta == 0 && B != 0)
	{
		double K = B / A;
		x1 = -bb / aa + K;
		x2 = -K / 2;
		cout << "有三个实根，其中两个相等" << endl;
		cout << "x1=" << x1 << endl;
		cout << "x2=x3=" << x2 << endl;
	}
	else if (delta < 0)
	{
		double T = (2 * A * bb - 3 * aa * B) / (2 * sqrt(A * A * A));
		double angle = acos(T);
		x1 = (-bb - 2 * sqrt(A) * cos(angle / 3)) / (3 * aa);
		x2 = (-bb + sqrt(A) * (cos(angle / 3) + sqrt(3) * sin(angle / 3))) / (3 * aa);
		x3 = (-bb + sqrt(A) * (cos(angle / 3) - sqrt(3) * sin(angle / 3))) / (3 * aa);
		cout << "有三个实数根" << endl;
		cout << "x1=" << x1 << endl;
		cout << "x2=" << x2 << endl;
		cout << "x3=" << x3 << endl;
	}
}


void JudgeType(string& tmp)
{
	for (int i = 0; i < tmp.size(); ++i)
	{
		if (tmp[i] == 'x' || tmp[i] == 'X')
		{
			if (i + 1 < tmp.size())
			{
				if (i + 1 < tmp.size() && tmp[i + 1] >= '0' && tmp[i + 1] <= '9')
				{
					type = 2;
					return;
				}
				type = 1;
				return;
			}
			type = 1;
			return;
		}
	}
	type = 1;
	return;
}
