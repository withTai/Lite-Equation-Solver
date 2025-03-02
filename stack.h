//此文件定义了链表用于堆栈，进行二叉树的存储与运算，实现文本表达式的计算
#pragma once
#include"analysis.h"
using namespace std;

//中间链表暂存分析后的字符串
struct m_data
{
	string str = "0"; //参数or函数or字母常数（1）- 1
	Num number = Zero; //数字（2）- 2
	char op = 0; //运算符（3）- 3 4 5 6 7 8 9 10
	int type_number = 0; //类中成员代码
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

/*---------------二叉树的链式存储-------------------*/
typedef struct BiTNode
{
	char data = 0;	//结点数据域（符号）
	Num number = 0;	//结点数据域（数值）
	string combine_data = "0";	//结点数据域（字母）
	BiTNode* lchild = 0, * rchild = 0;
}*BiTree;
/*-----------------链式堆栈----------------------*/
typedef struct StackNode
{
	BiTree data_tree;	//存储的是二叉树
	char data_op;	//存储的是符号
	StackNode* next;
}*LinkStack;
//二叉树入栈
void Push_tree(LinkStack& S, BiTree e)
{
	LinkStack p = new StackNode;
	p->data_tree = e;
	p->next = S;
	S = p;
}
//字符（运算符号）入栈
void Push_op(LinkStack& S, char e)
{
	LinkStack p = new StackNode;
	p->data_op = e;
	p->next = S;
	S = p;
}
//二叉树出栈 T1
void Pop_tree(LinkStack& S, BiTree& T1)
{
	if (S == NULL)	return;
	LinkStack p = S;
	T1 = p->data_tree;
	S = S->next;
	delete p;
}
//字符（运算符号）出栈 ch
void Pop_op(LinkStack& S, char& ch)
{
	if (S == NULL)	return;
	LinkStack p = S;
	ch = p->data_op;
	S = S->next;
	delete p;
}
//取栈顶元素
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

/*------------------------------表达式树的创建算法------------------------------*/
//创建以T为根结点的二叉树（存储符号）
void CreateExpTree(BiTree& T, BiTree a, BiTree b, char theta)
{
	BiTree L = new BiTNode;
	L->data = theta;
	L->lchild = a;
	L->rchild = b;
	T = L;
}
//创建以T为根结点的二叉树（存储字母）
void CreateExpTree_str(BiTree& T, BiTree a, BiTree b, string theta)
{
	BiTree L = new BiTNode;
	L->combine_data = theta;
	L->lchild = a;
	L->rchild = b;
	T = L;
}
//创建以T为根结点的二叉树（存储数值）
void CreateExpTree_num(BiTree& T, BiTree a, BiTree b, Num theta)
{
	BiTree L = new BiTNode;
	L->number = theta;
	L->lchild = a;
	L->rchild = b;
	T = L;
}
//比较运算符的优先级
int isp(char ch)	//栈内优先级
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
int icp(char ch)	//栈外优先级
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
//top是栈顶元素，ch是需要比较的元素
char Precede(int isp, int icp)
{
	if (isp < icp)
		return '<';
	else if (isp > icp)
		return '>';
	else
		return '=';
}

/*--------------------★★★★★创建表达式树★★★★★--------------------------*/
//expt栈(根结点)，optr栈(运算符)
void InitExpTree(m_data* p, LinkStack& expt, LinkStack& optr)
{
	BiTree T = NULL;	//树根
	BiTree T1 = NULL;	//左子树
	BiTree T2 = NULL;	//右子树
	char ch = ' ';	//弹出的符号
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
			switch (Precede(isp(GetTop_op(optr)), icp(p->op)))	//比较优先级
			{
			case '<':
				Push_op(optr, p->op);
				p = p->next;
				break;
			case '>':
				Pop_op(optr, ch);	//弹出上一个字符
				Pop_tree(expt, T1);
				Pop_tree(expt, T2);	//弹出上两个数值（二叉树）
				CreateExpTree(T, T2, T1, ch);	//以data_tree为根，连接T1和T2两颗子树
				Push_tree(expt, T);		//最后把T放进expt栈中
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
/*-----------------------------表达式树的求值-------------------------------*/
//根据当前结点运算符的例子那个进行相应运算
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
//遍历表达式树求表达式的值
Num EvaluateExpTree(BiTree T)
{
	Num lvalue = Zero, rvalue = Zero;	//存放叶子结点的数据域初始为0
	if (T->lchild == NULL && T->rchild == NULL)
		return T->number;
	else {
		lvalue = EvaluateExpTree(T->lchild);	//相当于后序遍历二叉树
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