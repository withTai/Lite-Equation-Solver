//此文件为对文本方程表达式的词法语法分析，定义了多个判断文本词法语法类型的函数
#pragma once
#include "hpf.h"
int type = 1;//判断方程类型
char ch;   //存放当前读取字符
char a[128];
char* Exp = a; //字符串输入的表达式
char token[64]; //暂存字符数组
Num num; //存放当前读入的数值
enum SYMBOL {
	ERROR, IDSY, INTSY,
	PLUSSY, MINUSSY, STARSY, DIVISY, LPARSY,
	RPARSY, POWERSY, EQUSY, Xx
}symbol;
//枚举类型全局变量，代表所识别内容的类型
Num ce[10][10];

void getChar() //读取字符存入ch中，将指针指向下一字符
{
	ch = *Exp;
	Exp++;
}
void clearToken() //清除token中暂存的内容
{
	memset(token, '\0', sizeof(token));
}
bool isSpace() //判断是否空格
{
	if ((int)*Exp == 32)
		return true;
	else
		return false;
}
bool isNewline() //判断是否回车
{
	if ((int)*Exp == 10)
		return true;
	else
		return false;
}
bool isTab() //制表符
{
	if ((int)*Exp == 9)
		return true;
	else
		return false;
}
bool isLetter() //字母
{
	if ((ch >= 'A' && ch < 'X') || (ch > 'X' && ch <= 'Z') || (ch >= 'a' && ch < 'x') || (ch > 'x' && ch <= 'z'))
		return true;
	else
		return false;
}
bool isDigit() //数字
{
	if (ch >= '0' && ch <= '9')
		return true;
	else
		return false;
}
bool isEqu() //等号
{
	if ((int)ch == 61)
		return true;
	else
		return false;
}
bool isPlus() //+
{
	if ((int)ch == 43)
		return true;
	else
		return false;
}
bool isMinus() //-
{
	if ((int)ch == 45)
		return true;
	else
		return false;
}
bool isDivi() //除
{
	if ((int)ch == 47)
		return true;
	else
		return false;
}
bool isStar() //*
{
	if ((int)ch == 42)
		return true;
	else
		return false;
}
bool isLpar() //（
{
	if ((int)ch == 40)
		return true;
	else
		return false;
}
bool isRpar() //）
{
	if ((int)ch == 41)
		return true;
	else
		return false;
}
bool isPower() //^
{
	if ((int)ch == 94)
		return true;
	else
		return false;
}
bool isDecimal() //小数点，判断是否为小数
{
	if ((int)ch == 46)
		return true;
	else
		return false;
}
bool isEqual() //=
{
	if ((int)ch == 61)
		return true;
	else
		return false;
}
bool isX() //未知数
{
	if (ch == 'X' || ch == 'x')
		return true;
	else
		return false;
}
void catToken() //将当前字符ch与token中字符串连接
{
	token[strlen(token)] = ch;
}
void retract() //字符指针后退一位
{
	Exp--;
	ch = *Exp;
}

void error() //出错处理过程 
{
	symbol = ERROR;
}
/*------------------------------词法分析------------------------------*/
int getsym()
{
	clearToken();
	if (isSpace() || isNewline() || isTab()) //跳过space、enter、tab
		Exp++;
	getChar();
	if (isLetter()) //判断是否为字母
	{
		while (isLetter()) //连续字符进行拼接
		{
			catToken();
			getChar();
		}
		retract();
		symbol = IDSY;//1
	}
	else if (isX()) //判断是否未知数
	{
		catToken();
		getChar();
		if (isDigit()) //判断是否数字，即多元方程
		{
			catToken();
		}
		else
			retract();
		symbol = Xx;//11
	}
	else if (isDigit()) //判断是否数字
	{
		while (isDigit() || isDecimal()) //连续数字拼接、包括小数点
		{
			catToken();
			getChar();
		}
		retract();
		string str;
		str = token;
		num = str;
		//num = transNum(token);
		symbol = INTSY;//2
	}
	else if (isPlus()) //+
	{
		symbol = PLUSSY; //3
	}
	else if (isMinus()) //-
	{
		symbol = MINUSSY; //4
	}
	else if (isStar()) //*
	{
		symbol = STARSY;//5
	}
	else if (isDivi()) //判断是否/
	{
		symbol = DIVISY; //6
	}
	else if (isLpar()) //（
	{
		symbol = LPARSY; //7
	}
	else if (isRpar()) // )
	{
		symbol = RPARSY; //8
	}
	else if (isPower()) //^
	{
		symbol = POWERSY; //9
	}
	else if (isEqual()) //=
	{
		symbol = EQUSY; //10
	}
	else
		error();
	return symbol;
}