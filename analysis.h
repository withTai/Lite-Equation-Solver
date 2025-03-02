//���ļ�Ϊ���ı����̱��ʽ�Ĵʷ��﷨�����������˶���ж��ı��ʷ��﷨���͵ĺ���
#pragma once
#include "hpf.h"
int type = 1;//�жϷ�������
char ch;   //��ŵ�ǰ��ȡ�ַ�
char a[128];
char* Exp = a; //�ַ�������ı��ʽ
char token[64]; //�ݴ��ַ�����
Num num; //��ŵ�ǰ�������ֵ
enum SYMBOL {
	ERROR, IDSY, INTSY,
	PLUSSY, MINUSSY, STARSY, DIVISY, LPARSY,
	RPARSY, POWERSY, EQUSY, Xx
}symbol;
//ö������ȫ�ֱ�����������ʶ�����ݵ�����
Num ce[10][10];

void getChar() //��ȡ�ַ�����ch�У���ָ��ָ����һ�ַ�
{
	ch = *Exp;
	Exp++;
}
void clearToken() //���token���ݴ������
{
	memset(token, '\0', sizeof(token));
}
bool isSpace() //�ж��Ƿ�ո�
{
	if ((int)*Exp == 32)
		return true;
	else
		return false;
}
bool isNewline() //�ж��Ƿ�س�
{
	if ((int)*Exp == 10)
		return true;
	else
		return false;
}
bool isTab() //�Ʊ��
{
	if ((int)*Exp == 9)
		return true;
	else
		return false;
}
bool isLetter() //��ĸ
{
	if ((ch >= 'A' && ch < 'X') || (ch > 'X' && ch <= 'Z') || (ch >= 'a' && ch < 'x') || (ch > 'x' && ch <= 'z'))
		return true;
	else
		return false;
}
bool isDigit() //����
{
	if (ch >= '0' && ch <= '9')
		return true;
	else
		return false;
}
bool isEqu() //�Ⱥ�
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
bool isDivi() //��
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
bool isLpar() //��
{
	if ((int)ch == 40)
		return true;
	else
		return false;
}
bool isRpar() //��
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
bool isDecimal() //С���㣬�ж��Ƿ�ΪС��
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
bool isX() //δ֪��
{
	if (ch == 'X' || ch == 'x')
		return true;
	else
		return false;
}
void catToken() //����ǰ�ַ�ch��token���ַ�������
{
	token[strlen(token)] = ch;
}
void retract() //�ַ�ָ�����һλ
{
	Exp--;
	ch = *Exp;
}

void error() //��������� 
{
	symbol = ERROR;
}
/*------------------------------�ʷ�����------------------------------*/
int getsym()
{
	clearToken();
	if (isSpace() || isNewline() || isTab()) //����space��enter��tab
		Exp++;
	getChar();
	if (isLetter()) //�ж��Ƿ�Ϊ��ĸ
	{
		while (isLetter()) //�����ַ�����ƴ��
		{
			catToken();
			getChar();
		}
		retract();
		symbol = IDSY;//1
	}
	else if (isX()) //�ж��Ƿ�δ֪��
	{
		catToken();
		getChar();
		if (isDigit()) //�ж��Ƿ����֣�����Ԫ����
		{
			catToken();
		}
		else
			retract();
		symbol = Xx;//11
	}
	else if (isDigit()) //�ж��Ƿ�����
	{
		while (isDigit() || isDecimal()) //��������ƴ�ӡ�����С����
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
	else if (isDivi()) //�ж��Ƿ�/
	{
		symbol = DIVISY; //6
	}
	else if (isLpar()) //��
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