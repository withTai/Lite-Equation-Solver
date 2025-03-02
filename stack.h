//���ļ��������������ڶ�ջ�����ж������Ĵ洢�����㣬ʵ���ı����ʽ�ļ���
#pragma once
#include"analysis.h"
using namespace std;

//�м������ݴ��������ַ���
struct m_data
{
	string str = "0"; //����or����or��ĸ������1��- 1
	Num number = Zero; //���֣�2��- 2
	char op = 0; //�������3��- 3 4 5 6 7 8 9 10
	int type_number = 0; //���г�Ա����
	m_data* next = 0;
};
m_data* addtail(m_data* head, int s)
{
	m_data* p = new m_data;
	if (s == 1)
	{
		p->str = token;
		p->type_number = 1;
	}
	else if (s == 2)
	{
		p->number = num;
		p->type_number = 2;
	}
	else
	{
		p->op = ch;
		p->type_number = 3;
	}
	p->next = 0;
	if (!head)
	{
		head = p;
	}
	else
	{
		m_data* w = head;
		while (w->next)
			w = w->next;
		w->next = p;
	}
	return head;
}
m_data* changedata(m_data* head)
{
	m_data* p = head;
	m_data* c1 = new m_data;
	m_data* c2 = new m_data;
	m_data* c3 = new m_data;

	c1->op = '(';
	c1->type_number = 3;
	c1->next = 0;

	c2->op = ')';
	c2->type_number = 3;
	c2->next = c3;

	c3->op = '=';
	c3->type_number = 3;
	c3->next = 0;

	while (p && p->op != '=')
		p = p->next;
	if (p)
	{
		p->op = '-';
		c1->next = p->next;
		p->next = c1;

		while (p->next)
			p = p->next;
		p->next = c2;
	}
	return head;
}
void dellist(m_data* head)
{
	while (head)
	{
		m_data* p = head;
		head = head->next;
		delete p;
	}
}

/*---------------����������ʽ�洢-------------------*/
typedef struct BiTNode
{
	char data = 0;	//��������򣨷��ţ�
	Num number = 0;	//�����������ֵ��
	string combine_data = "0";	//�����������ĸ��
	BiTNode* lchild = 0, * rchild = 0;
}*BiTree;
/*-----------------��ʽ��ջ----------------------*/
typedef struct StackNode
{
	BiTree data_tree;	//�洢���Ƕ�����
	char data_op;	//�洢���Ƿ���
	StackNode* next;
}*LinkStack;
//��������ջ
void Push_tree(LinkStack& S, BiTree e)
{
	LinkStack p = new StackNode;
	p->data_tree = e;
	p->next = S;
	S = p;
}
//�ַ���������ţ���ջ
void Push_op(LinkStack& S, char e)
{
	LinkStack p = new StackNode;
	p->data_op = e;
	p->next = S;
	S = p;
}
//��������ջ T1
void Pop_tree(LinkStack& S, BiTree& T1)
{
	if (S == NULL)	return;
	LinkStack p = S;
	T1 = p->data_tree;
	S = S->next;
	delete p;
}
//�ַ���������ţ���ջ ch
void Pop_op(LinkStack& S, char& ch)
{
	if (S == NULL)	return;
	LinkStack p = S;
	ch = p->data_op;
	S = S->next;
	delete p;
}
//ȡջ��Ԫ��
char GetTop_op(LinkStack S)
{
	if (S != NULL)
		return S->data_op;
	else return ' ';
}
BiTree GetTop_tree(LinkStack S)
{
	if (S != NULL)
		return S->data_tree;
	else return NULL;
}

/*------------------------------���ʽ���Ĵ����㷨------------------------------*/
//������TΪ�����Ķ��������洢���ţ�
void CreateExpTree(BiTree& T, BiTree a, BiTree b, char theta)
{
	BiTree L = new BiTNode;
	L->data = theta;
	L->lchild = a;
	L->rchild = b;
	T = L;
}
//������TΪ�����Ķ��������洢��ĸ��
void CreateExpTree_str(BiTree& T, BiTree a, BiTree b, string theta)
{
	BiTree L = new BiTNode;
	L->combine_data = theta;
	L->lchild = a;
	L->rchild = b;
	T = L;
}
//������TΪ�����Ķ��������洢��ֵ��
void CreateExpTree_num(BiTree& T, BiTree a, BiTree b, Num theta)
{
	BiTree L = new BiTNode;
	L->number = theta;
	L->lchild = a;
	L->rchild = b;
	T = L;
}
//�Ƚ�����������ȼ�
int isp(char ch)	//ջ�����ȼ�
{
	switch (ch)
	{
	case'=': return 0;
	case'(': return 1;
	case'^': return 7;
	case'*': case'/': return 5;
	case'+': case'-': return 3;
	case')': return 8;
	}
}
int icp(char ch)	//ջ�����ȼ�
{
	switch (ch)
	{
	case'=': return 0;
	case'(': return 8;
	case'^': return 6;
	case'*': case'/': return 4;
	case'+': case'-': return 2;
	case')': return 1;
	}
}
//top��ջ��Ԫ�أ�ch����Ҫ�Ƚϵ�Ԫ��
char Precede(int isp, int icp)
{
	if (isp < icp)
		return '<';
	else if (isp > icp)
		return '>';
	else
		return '=';
}

/*--------------------�����ﴴ�����ʽ��������--------------------------*/
//exptջ(�����)��optrջ(�����)
void InitExpTree(m_data* p, LinkStack& expt, LinkStack& optr)
{
	BiTree T = NULL;	//����
	BiTree T1 = NULL;	//������
	BiTree T2 = NULL;	//������
	char ch = ' ';	//�����ķ���
	while (p)
	{
		if (p->type_number == 2)
		{
			CreateExpTree_num(T, NULL, NULL, p->number);
			Push_tree(expt, T);
			p = p->next;
		}
		else if (p->type_number == 1)
		{
			CreateExpTree_str(T, NULL, NULL, p->str);
			Push_tree(expt, T);
			p = p->next;
		}
		else
		{
			switch (Precede(isp(GetTop_op(optr)), icp(p->op)))	//�Ƚ����ȼ�
			{
			case '<':
				Push_op(optr, p->op);
				p = p->next;
				break;
			case '>':
				Pop_op(optr, ch);	//������һ���ַ�
				Pop_tree(expt, T1);
				Pop_tree(expt, T2);	//������������ֵ����������
				CreateExpTree(T, T2, T1, ch);	//��data_treeΪ��������T1��T2��������
				Push_tree(expt, T);		//����T�Ž�exptջ��
				break;
			case '=':
				Pop_op(optr, ch);
				p = p->next;
				break;
			default:
				break;
			}
		}
	}
}
/*-----------------------------���ʽ������ֵ-------------------------------*/
//���ݵ�ǰ���������������Ǹ�������Ӧ����
Num GetValue(char data, Num lvalue, Num rvalue)
{
	Num t;
	switch (data)
	{
	case '+':
		//cout << lvalue + rvalue << "+" << endl;
		return lvalue + rvalue;
	case '-':
		//cout << lvalue - rvalue << "-" << endl;
		return lvalue - rvalue;
	case '*':
		//cout << lvalue * rvalue << "*" << endl;
		return lvalue * rvalue;
	case '/':
		return lvalue / rvalue;
	case '^':
		t = powf(lvalue, rvalue);
		//cout << t << "pow" << endl;
		return t;
		/*case '=':
			if (lvalue == rvalue)
				return 1;
			else
				return 0;*/
	default:
		break;
	}
}
//�������ʽ������ʽ��ֵ
Num EvaluateExpTree(BiTree T)
{
	Num lvalue = Zero, rvalue = Zero;	//���Ҷ�ӽ����������ʼΪ0
	if (T->lchild == NULL && T->rchild == NULL)
		return T->number;
	else {
		lvalue = EvaluateExpTree(T->lchild);	//�൱�ں������������
		rvalue = EvaluateExpTree(T->rchild);
		return GetValue(T->data, lvalue, rvalue);
	}
}

void addce(m_data* head, int& i, int& count)
{
	count = 0;
	m_data* p = head;
	while (p)
	{
		if (p->type_number == 2)
		{
			ce[i][count] = p->number;
			count++;
		}
		p = p->next;
	}
	i++;
	dellist(head);
}