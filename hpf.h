//此文件为高精度算法头文件，定义了高精度浮点数的类和对象，并重载了一系列运算符，以达到高精度运算的目的
#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <map>
#include <cmath>

using namespace std;

struct Node
{
	signed char data;//每一位存储的数字
	Node* pre, * next;//链表的前后数位
	Node(signed char x = 0, Node* p = NULL, Node* n = NULL)
	{
		data = x;
		pre = p;
		next = n;
	}
};

class Num
{
public:
	bool sign;//符号，0为正，1为负
	int exp;//数量级
	int len;//数位长度
	Node* head, * tail;//存储数位的链表

public:
	static bool OutType;//输出方式，0为小数输出，1为科学计数法输出
	static bool AngleUnit;//角度计算单位，0为弧度制，1为角度制
	static int Precision;//运算精度，默认为16位，可设置
	static int RealPrecision;//内部运算精度，规定100位
	static const Num Zero;//定义0，方便比较
	static const Num NumPi;//圆周率，保留100位
	static const Num NumE;//自然对数，保留100位
	Num(bool S, int E, int L, Node* H, Node* T);//构造函数
	Num(const Num& x);//拷贝构造函数
	Num(const string& x);//字符串构造函数
	Num& Neg() const;//相反数运算
	void PushFront(signed char x);//在数位最高位添加一个数
	void PushBack(signed char x);//在数位最低位添加一个数
	void Rounding(int x);//四舍五入保留有效数字
	void SimplifyFront();//化简去掉链表头部的0
	void SimplifyBack();//化简去掉链表尾部的0
	signed char PopFront();//在数位最高位删除一个数，并返回这个数
	signed char PopBack();//在数位最低位删除一个数，并返回这个数
	friend void Shift(Num& x, Num& y);//添加头尾的0，使这两个数的数量级对齐
	Num& operator=(const Num& x);
	Num& operator=(const int& x);
	Num& operator=(const double& x);
	Num& operator=(const string& x);
	operator double() const;
	operator int() const;
	operator string() const;
	friend bool operator==(const Num& x, const Num& y);
	friend bool operator!=(const Num& x, const Num& y);
	friend bool operator>(const Num& x, const Num& y);
	friend bool operator>=(const Num& x, const Num& y);
	friend bool operator<(const Num& x, const Num& y);
	friend bool operator<=(const Num& x, const Num& y);
	friend Num abs(const Num x);
	friend Num operator-(const Num& x);
	friend Num operator+(const Num& x, const Num& y);
	friend Num operator-(const Num& x, const Num& y);
	friend Num operator*(const Num& x, const Num& y);
	friend Num operator/(const Num& x, const Num& y);
	friend Num operator+=(Num& x, const Num& y);
	friend Num operator-=(Num& x, const Num& y);
	friend Num operator*=(Num& x, const Num& y);
	friend Num operator/=(Num& x, const Num& y);
	friend Num extract(const Num& x, const int& y);
	friend Num involution(const Num& x, const int& y);
	friend Num powf(const Num& x, const Num& y);
	friend istream& operator>>(istream& in, Num& x);
	friend ostream& operator<<(ostream& out, const Num& x);
};

bool Num::OutType = 0;
bool Num::AngleUnit = 0;
int Precision = 16;
int RealPrecision = 100;
const Num Zero(0, 0, 0, NULL, NULL);
string strPi = "3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679";
Num NumPi(strPi);
string strE = "2.7182818284590452353602874713526624977572470936999595749669676277240766303535475945713821785251664274";
Num NumE(strE);

Num::Num(bool S = false, int E = 0, int L = 0, Node* H = NULL, Node* T = NULL)
{
	sign = S;
	exp = E;
	len = L;
	head = H;
	tail = T;
}

Num::Num(const Num& x)
{
	sign = x.sign;
	exp = x.exp;
	len = x.len;
	head = x.head;
	tail = x.tail;
}

Num::Num(const string& x)
{
	Num t = new Num;
	t = x;
	sign = t.sign;
	exp = t.exp;
	len = t.len;
	head = t.head;
	tail = t.tail;
}

Num& Num::Neg() const
{
	if (head == NULL && tail == NULL)
	{
		Num* t = new Num(sign, exp, len, head, tail);
		return *t;
	}
	else
	{
		Num* t = new Num(!sign, exp, len, head, tail);
		return *t;
	}
}

void Num::PushFront(signed char x)
{
	++len;
	Node* tp = new Node(x, NULL, head);
	if (head)
	{
		head->pre = tp;
		head = head->pre;;
	}
	else
	{
		head = tail = tp;
	}
}

void Num::PushBack(signed char x)
{
	++len;
	Node* tp = new Node(x, tail);
	if (tail)
	{
		tail->next = tp;
		tail = tail->next;
	}
	else
	{
		tail = head = tp;
	}
}

void Num::Rounding(int x)
{
	if (x >= len)
	{
		return;
	}
	else
	{
		Node* now = tail;
		for (int i = 0; i < len - x - 1; ++i)
		{
			now = now->pre;
		}
		signed char t = now->data;
		now = now->pre;
		tail = now;
		tail->next = NULL;
		len = x;
		if (t > 4)
		{
			++now->data;
		}
		while (now->data > 9)
		{
			now->data = 0;
			now = now->pre;
			if (!now)
			{
				PushFront(1);
			}
			else
			{
				++now->data;
			}
		}
		SimplifyBack();
		return;
	}
}

void Num::SimplifyFront()
{
	while (head->data == 0)
	{
		if (head == tail)
		{
			break;
		}
		--exp;
		--len;
		head = head->next;
		head->pre = NULL;
	}
}

void Num::SimplifyBack()
{
	while (tail->data == 0)
	{
		if (head == tail)
		{
			break;
		}
		--len;
		tail = tail->pre;
		tail->next = NULL;
	}
}

signed char Num::PopFront()
{
	if (head == NULL && tail == NULL)
	{
		return 0;
	}
	--len;
	signed char t = head->data;
	if (head == tail)
	{
		head = tail = NULL;
	}
	else
	{
		head = head->next;
		head->pre = NULL;
	}
	return t;
}

signed char Num::PopBack()
{
	if (head == NULL && tail == NULL)
	{
		return 0;
	}
	--len;
	signed char t = tail->data;
	if (head == tail)
	{
		head = tail = NULL;
	}
	else
	{
		tail = tail->pre;
		tail->next = NULL;
	}
	return t;
}

void Shift(Num& x, Num& y)
{
	if (x.exp != y.exp)
	{
		Num& z((y.exp < x.exp) ? y : x);
		int t = (x.exp - z.exp) + (y.exp - z.exp);
		z.exp += t;
		while (t--)
		{
			z.PushFront(0);
		}
	}
	if (x.len != y.len)
	{
		Num& w((y.len < x.len) ? y : x);
		int s = (x.len - w.len) + (y.len - w.len);
		while (s--)
		{
			w.PushBack(0);
		}
	}
}

Num& Num::operator=(const Num& x)
{
	Num* t = new Num;
	Node* nowx = x.tail;
	while (nowx)
	{
		t->PushFront(nowx->data);
		nowx = nowx->pre;
	}
	t->sign = x.sign;
	t->exp = x.exp;
	this->sign = t->sign;
	this->exp = t->exp;
	this->len = t->len;
	this->head = t->head;
	this->tail = t->tail;
	return *this;
}

Num& Num::operator=(const int& x)
{
	Num* t = new Num;
	t->sign = (x < 0);
	int tp = 1;
	while (x / tp)
	{
		t->PushFront(abs(x / tp % 10));
		tp *= 10;
		++t->exp;
	}
	if (x)
	{
		--t->exp;
	}
	this->sign = t->sign;
	this->exp = t->exp;
	this->len = t->len;
	this->head = t->head;
	this->tail = t->tail;
	return *this;
}

Num& Num::operator=(const double& x)
{
	char t[16] = { 0 };
	sprintf_s(t, "%f", x);
	string str = t;
	*this = str;
	this->SimplifyBack();
	return *this;
}

Num& Num::operator=(const string& x)
{
	Num* t = new Num;
	short d = 0;
	if (x[0] == '0' && x.size() == 1)
	{
		this->sign = t->sign;
		this->exp = t->exp;
		this->len = t->len;
		this->head = t->head;
		this->tail = t->tail;
		return *this;
	}
	while (x[d] != 'e' && x[d] != 'E' && d <= (x.size() - 1))
	{
		d++;
	}
	if (d < x.size() - 1)
	{
		t->exp = stoll(x.substr(d + 1, x.size() - 1));
	}
	string str = x.substr(0, d);
	{
		if (str[0] == '-')
		{
			t->sign = true;
			str.erase(0, 1);
		}
		long long p = 0;
		while (str[p] == '0')
		{
			p++;
		}
		if (p != 0)
		{
			str.erase(0, p);
		}
		reverse(str.begin(), str.end());
		if (str[0] == '\0')
		{
			str.erase(0, 1);
		}
		bool IfDot = 0;
		long long DotPosition = 0;
		for (unsigned long long i = 0; i < str.size(); i++)
		{
			if (str[i] == '.')
			{
				IfDot = 1;
				DotPosition = i;
				continue;
			}
			if (str[i] < 48 || str[i] > 57)
			{
				throw("DataException");
				return *this;
			}
			t->PushFront(str[i] - 48);
		}
		if (str[str.size() - 1] == '.')
		{
			str.erase(str.size() - 1, 1);
			long long p = 0;
			while (str[str.size() - 1 - p] == '0')
			{
				p++;
			}
			t->exp += -p - 1;
			for (int i = 0; i < p; ++i)
			{
				t->PopFront();
			}
		}
		else
		{
			t->exp += str.size() - 1 - DotPosition - IfDot;
		}
		this->sign = t->sign;
		this->exp = t->exp;
		this->len = t->len;
		this->head = t->head;
		this->tail = t->tail;
		return *this;
	}
}

Num::operator double() const
{
	double x = 0;
	double m = 0;
	Node* now = head;
	for (long long i = 0; i < len; i++)
	{
		m = now->data;
		m *= pow(10.0, exp - i);
		x += m;
		now = now->next;
	}
	if (sign)
	{
		return -x;
	}
	else
	{
		return x;
	}
}

Num::operator int() const
{
	Num t(sign, exp, len, head, tail);
	double s = t;
	return s;
}

Num::operator string() const
{
	Num t(sign, exp, len, head, tail);
	string res;
	if (t.sign == 1)
	{
		res += '-';
	}
	if (t.exp < 0)
	{
		res += "0.";
		for (long long i = -1; i > exp; i--)
		{
			res += '0';
		}
	}
	Node* now = t.head;
	for (long long i = len; i > 0; i--)
	{
		res.push_back(now->data + 48);
		now = now->next;
		if (len - i == exp && i != 1)
		{
			res += '.';
		}
	}
	for (long long i = exp - len + 1; i > 0; i--)
	{
		res += '0';
	}
	return res;
}

bool operator==(const Num& x, const Num& y)
{
	if (x.len == 0 && !(y.len == 0))
	{
		return false;
	}
	if (!(x.len == 0) && y.len == 0)
	{
		return false;
	}
	if (x.len == 0 && y.len == 0)
	{
		return true;
	}
	Num xx = x;
	Num yy = y;
	Shift(xx, yy);
	Node* nowx = xx.head, * nowy = yy.head;
	int lenx = xx.len, leny = yy.len;
	while (nowx->next && !nowx->data)
	{
		nowx = nowx->next, lenx--;
	}
	while (nowy->next && !nowy->data)
	{
		nowy = nowy->next, leny--;
	}
	if (xx.sign != yy.sign)
	{
		return false;
	}
	if (lenx != leny)
	{
		return false;
	}
	while (nowx && nowy)
	{
		if (nowx->data != nowy->data)
		{
			return false;
		}
		nowx = nowx->next;
		nowy = nowy->next;
	}
	return true;
}

bool operator!=(const Num& x, const Num& y)
{
	return !(x == y);
}

bool operator>(const Num& x, const Num& y)
{
	if (x == Zero && !(y == Zero))
	{
		return y.sign;
	}
	if (!(x == Zero) && y == Zero)
	{
		return !x.sign;
	}
	if (x == Zero && y == Zero)
	{
		return false;
	}
	if (x.sign && !y.sign)
	{
		return false;
	}
	else if (!x.sign && y.sign)
	{
		return true;
	}
	else if (x.sign && y.sign)
	{
		return x.Neg() < y.Neg();
	}
	else
	{
		Num xx = x;
		Num yy = y;
		Shift(xx, yy);
		Node* nowx = xx.head, * nowy = yy.head;
		while (nowx && nowy)
		{
			if (nowx->data > nowy->data)
			{
				return true;
			}
			else if (nowx->data < nowy->data)
			{
				return false;
			}
			nowx = nowx->next;
			nowy = nowy->next;
		}
		return false;
	}
}

bool operator>=(const Num& x, const Num& y)
{
	return !(x < y);
}

bool operator<(const Num& x, const Num& y)
{
	if (x == Zero && !(y == Zero))
	{
		return !y.sign;
	}
	if (!(x == Zero) && y == Zero)
	{
		return x.sign;
	}
	if (x == Zero && y == Zero)
	{
		return false;
	}
	if (x.sign && !y.sign)
	{
		return true;
	}
	else if (!x.sign && y.sign)
	{
		return false;
	}
	else if (x.sign && y.sign)
	{
		return x.Neg() > y.Neg();
	}
	else
	{
		Num xx = x;
		Num yy = y;
		Shift(xx, yy);
		Node* nowx = xx.head, * nowy = yy.head;
		while (nowx && nowy)
		{
			if (nowx->data < nowy->data)
			{
				return true;
			}
			else if (nowx->data > nowy->data)
			{
				return false;
			}
			nowx = nowx->next;
			nowy = nowy->next;
		}
		return false;
	}
}

bool operator<=(const Num& x, const Num& y)
{
	return !(x > y);
}

Num abs(const Num x)
{
	if (x.head == NULL && x.tail == NULL)
	{
		return x;
	}
	else
	{
		Num t = x;
		t.sign = false;
		return t;
	}
}

Num operator-(const Num& x)
{
	Num t = x;
	t.sign = !t.sign;
	return t;
}

Num operator+(const Num& x, const Num& y)
{
	if (x == Zero && y == Zero)
	{
		return Zero;
	}
	if (x == Zero)
	{
		return y;
	}
	if (y == Zero)
	{
		return x;
	}
	if (x.sign && !y.sign)
	{
		return y - x.Neg();
	}
	else if (!x.sign && y.sign)
	{
		return x - y.Neg();
	}
	else if (x.sign && y.sign)
	{
		return (x + y).Neg();
	}
	else
	{
		Num xx = x;
		Num yy = y;
		Shift(xx, yy);
		Num* ans = new Num(false, xx.exp);
		Node* nowx = xx.tail, * nowy = yy.tail;
		signed char buf = 0;
		while (true)
		{
			signed char sum = buf;
			if (nowx)
			{
				sum += nowx->data;
			}
			if (nowy)
			{
				sum += nowy->data;
			}
			ans->PushFront(sum % 10);
			buf = sum / 10;
			if (nowx)
			{
				nowx = nowx->pre;
			}
			if (nowy)
			{
				nowy = nowy->pre;
			}
			if (!nowx && !nowy && buf)
			{
				++ans->exp;
			}
			if (!nowx && !nowy && !buf)
			{
				break;
			}
		}
		ans->SimplifyBack();
		ans->Rounding(RealPrecision);
		ans->SimplifyBack();
		return *ans;
	}
}

Num operator-(const Num& x, const Num& y)
{
	if (x == Zero && y == Zero)
	{
		return Zero;
	}
	if (x == y)
	{
		return Zero;
	}
	if (x == Zero)
	{
		return y.Neg();
	}
	if (y == Zero)
	{
		return x;
	}
	if (x.sign && !y.sign)
	{
		return (x.Neg() + y).Neg();
	}
	else if (!x.sign && y.sign)
	{
		return x + y.Neg();
	}
	else if (x.sign && y.sign)
	{
		return y.Neg() - x.Neg();
	}
	else if (x < y)
	{
		return (y - x).Neg();
	}
	else
	{
		Num xx = x;
		Num yy = y;
		Shift(xx, yy);
		xx.head->pre = NULL;//
		Num* ans = new Num(false, xx.exp);
		Node* nowx = xx.tail, * nowy = yy.tail;
		signed char buf = 0;
		while (true)
		{
			signed char res = buf + nowx->data;
			buf = 0;
			if (nowy)
			{
				res -= nowy->data;
			}
			if (res < 0)
			{
				buf--, res += 10;
			}
			ans->PushFront(res);
			nowx = nowx->pre;
			if (nowy)
			{
				nowy = nowy->pre;
			}
			if (!nowx)
			{
				break;
			}
		}
		ans->SimplifyFront();
		ans->SimplifyBack();
		ans->Rounding(RealPrecision);
		ans->SimplifyBack();
		return *ans;
	}
}

Num operator*(const Num& x, const Num& y)
{
	if (x == Zero || y == Zero)
	{
		return Zero;
	}
	if (x.sign && !y.sign)
	{
		return (x.Neg() * y).Neg();
	}
	else if (!x.sign && y.sign)
	{
		return (x * y.Neg()).Neg();
	}
	else if (x.sign && y.sign)
	{
		return x.Neg() * y.Neg();
	}
	else
	{
		Num xx = x;
		Num yy = y;
		Num* ans = new Num(false, xx.exp + yy.exp);
		Node* nowy = yy.tail;
		Node* nowans = ans->tail;
		while (true)
		{
			signed char buf = 0;
			Node* nowx = xx.tail, * pos = nowans;
			while (true)
			{
				signed char sum = buf;
				if (nowx && nowy)
				{
					sum += (nowx->data) * (nowy->data);
				}
				if (!pos)
				{
					ans->PushFront(sum % 10);
					buf = sum / 10;
					pos = ans->head;
					if (!nowans)
					{
						nowans = pos;
					}
				}
				else
				{
					buf = (pos->data + sum) / 10;
					pos->data = (pos->data + sum) % 10;
				}
				if (nowx)
				{
					nowx = nowx->pre;
				}
				pos = pos->pre;
				if (!nowx && !buf)
				{
					break;
				}
			}
			if (nowy)
			{
				nowy = nowy->pre;
			}
			nowans = nowans->pre;
			if (!nowy)
			{
				break;
			}
		}
		ans->SimplifyFront();
		if (xx.len + yy.len == ans->len + 1)
		{
			ans->exp = xx.exp + yy.exp;
		}
		else if (xx.len + yy.len == ans->len)
		{
			ans->exp = xx.exp + yy.exp + 1;
		}
		ans->SimplifyBack();
		ans->Rounding(RealPrecision);
		ans->SimplifyBack();
		return *ans;
	}
}

Num operator/(const Num& x, const Num& y)
{
	if (x == Zero)
	{
		return Zero;
	}
	if (y == Zero)
	{
		throw("MathError");
		exit(3);
	}
	if (x.sign && !y.sign)
	{
		return (x.Neg() / y).Neg();
	}
	else if (!x.sign && y.sign)
	{
		return (x / y.Neg()).Neg();
	}
	else if (x.sign && y.sign)
	{
		return x.Neg() / y.Neg();
	}
	else
	{
		Num xx = x;
		Num yy = y;
		Num* ans = new Num;
		ans->exp = xx.exp - yy.exp;
		xx.exp = 0;
		yy.exp = 0;
		for (int i = 0; i < RealPrecision + 1; ++i)
		{
			ans->PushBack(0);
			while (xx >= yy)
			{
				++ans->tail->data;
				xx -= yy;
			}
			if (xx <= Zero || yy == Zero)
			{
				break;
			}
			--yy.exp;
		}
		ans->Rounding(RealPrecision);
		ans->SimplifyFront();
		ans->SimplifyBack();
		return *ans;
	}
}

Num operator+=(Num& x, const Num& y)
{
	Num t = x;
	t = x + y;
	x = t;
	return t;
}

Num operator-=(Num& x, const Num& y)
{
	Num t = x;
	t = x - y;
	x = t;
	return t;
}

Num operator*=(Num& x, const Num& y)
{
	Num t = x;
	t = x * y;
	x = t;
	return t;
}

Num operator/=(Num& x, const Num& y)
{
	Num t = x;
	t = x / y;
	x = t;
	return t;
}

Num extract(const Num& x, const int& y)
{
	Num low, high, mid, tmp;
	Num one, two, ten;
	one = 1;
	two = 2;
	ten = 10;
	if (x > one)
	{
		low = one;
		high = x;
	}
	else if (x < Zero)
	{
		throw"MathError";
		exit(3);
	}
	else
	{
		low = x;
		high = one;
	}
	while (low <= high)
	{
		mid = (low + high) / two;
		tmp = involution(mid, y);
		if (abs(tmp - x) <= involution(ten, -Precision + (tmp - x).exp + 1))
		{
			return mid;
		}
		else if (tmp > x)
		{
			high = mid;
		}
		else
		{
			low = mid;
		}
	}
}

Num involution(const Num& x, const int& y)
{
	Num ans;
	ans = 1;
	if (y > 0)
	{
		for (int i = 0; i < y; ++i)
		{
			ans *= x;
		}
	}
	else if (y < 0)
	{
		for (int i = 0; i > y; --i)
		{
			ans /= x;
		}
	}
	return ans;
}

Num powf(const Num& x, const Num& y)
{
	int i, k = 0;
	Num ans, t, tmpm, one, two, ten, numi;
	one = 1;
	two = 2;
	ten = 10;
	tmpm = one;
	ans = one;
	t = x - one;
	if (x == one)
	{
		return one;
	}
	if (y == Zero)
	{
		return one;
	}
	if (x == Zero)
	{
		return Zero;
	}
	if (x < Zero)
	{
		throw"MathError";
		return 0;
	}
	if (y.exp >= y.len - 1)
	{
		Node* now = y.head;
		while (now)
		{
			k *= 10;
			k += now->data;
			now = now->next;
		}
		k *= pow(10, y.exp - y.len + 1);
		return involution(x, k);
	}
	if (x >= two)
	{
		return powf(one / x, -y);
	}
	else
	{
		for (i = 1; abs(tmpm) > involution(ten, -Precision - 1); ++i)
		{
			numi = i;
			tmpm *= ((y - numi + one) * t / numi);
			ans += tmpm;
		}
		ans.Rounding(Precision);
		ans.SimplifyFront();
		ans.SimplifyBack();
		return ans;
	}
}

istream& operator>>(istream& in, Num& x)
{
	string t;
	in >> t;
	x = t;
	return in;
}

ostream& operator<<(ostream& out, const Num& x)
{
	Num t = x;
	if (!x.OutType)
	{
		if (t.sign == 1)
		{
			out << '-';
		}
		if (t.exp < 0)
		{
			out << "0.";
			for (long long i = -1; i > x.exp; i--)
			{
				out << '0';
			}
		}
		Node* now = t.head;
		for (long long i = x.len; i > 0; i--)
		{
			out << (int)(now->data);
			now = now->next;
			if (x.len - i == x.exp && i != 1)
			{
				out << '.';
			}
		}
		for (long long i = x.exp - x.len + 1; i > 0; i--)
		{
			out << '0';
		}
	}
	else
	{
		if (x.sign == 1)
		{
			out << '-';
		}
		out << (int)(t.head->data) << '.';
		Node* now = t.head->next;
		for (long long i = x.len - 1; i > 0; i--)
		{
			out << (int)(now->data);
			now = now->next;
		}
		out << 'e' << x.exp;
	}
	return out;
}