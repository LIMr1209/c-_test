/*
 ============================================================================
 Name        : partition.c
 Author      : limr
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//https://blog.csdn.net/weixin_43917485/article/details/103157567
#define max 10
int PN;//分区数量
int WN;//作业数量
int work[max];
typedef struct Partition
{
	int ID;//分区号
	int SA;//Starting Address 起始地址
	int SZ;//size大小
}Part;
Part PT[max];//保留原始数据
void Menu()
{
	printf("------------------分区分配算法-------------------\n");
	printf("*              1.首次适应算法FF(First Fit)     *\n");
	printf("*              2.最佳适应算法BF(Best Fit)      *\n");
	printf("*              3.最坏适应算法WF(Worst Fit)     *\n");
	printf("*              4.内存释放                      *\n");
	printf("*              0.退出                          *\n");
	printf("-------------------------------------------------\n");
}
void print(Part p[])//打印输出
{
	int i,j;
	for (i = 0; i < PN; i++)
	{
		if (p[i].SZ == 0)//存在大小为零的分区则删除
		{
			printf("删除%d号分区\n", i + 1);
          for (j = i; j < PN - 1; j++)
				 p[j] = p[j + 1];
		  PN--;
		}
	}
	printf("分区号 始址 大小\n");
	for (i = 0; i < PN; i++)
		printf("%4d%6d%5d\n", p[i].ID, p[i].SA, p[i].SZ);
}
void Copy(Part p[], Part s[])
{
	int i;
	for (i = 0; i < PN; i++)
	{
		s[i].ID = p[i].ID;
		s[i].SA = p[i].SA;
		s[i].SZ = p[i].SZ;
	}
}
void Sort(Part p[])//WF、BF算法使分区打乱，因此重新排序
{
	int i, n = PN;
	bool flag = true;
	while (n > 1 && flag)
	{
		flag = false;
		for (i = 0; i < n - 1; i++)
			if (p[i].ID>p[i + 1].ID)
			{
				Part temp = p[i];
				p[i] = p[i + 1];
				p[i + 1] = temp;
				flag = true;
			}
		n--;
	}
}
void Judge(Part p[])//判断作业是否满足
{
	int i, j;
	bool flag, free[max];
	for (i = 0; i < PN; i++)
		free[i] = true;
	for (i = 0; i < WN; i++)
	{
		flag = true;
		for (j = 0; j < PN&&flag; j++)
			if (work[i] <= p[j].SZ&&free[j])//第j个空闲分区满足第i个作业需求
			{
				p[j].SA += work[i];
				p[j].SZ -= work[i];
				flag = false;
				free[j] = false;
			}
		if (flag)
		{
			printf("内存不足!作业%d需等待.\n", i + 1); break;
		}
	}
}
bool Check(Part p[])//检查分区内存是否越界溢出
{
	int i,j;
	bool flag = true;
	for (i = 1; flag&&i < PN; i++)
		if (p[i].SA <( p[i - 1].SA + p[i-1].SZ))
			flag = false;
		else if (p[i].SA ==(p[i - 1].SA + p[i-1].SZ))
		{
			printf("合并分区\n");
			p[i - 1].SZ += p[i].SZ;
			for (j = i; j < PN-1; j++)
				p[j] = p[j + 1];
			PN--;
			print(p);
		}
	return flag;
}
void Free(Part p[])//释放内存，实际上是扩充空闲分区
{
	int i, choice, size;
	bool flag;
	printf("请输入释放内存的分区号和大小  ");
	scanf("%d%d", &choice, &size);
	Sort(p);
	p[choice - 1].SZ += size;
	flag=Check(p);
	if (flag) {
		printf("释放成功!\n"); print(p);
	}
	else
	{
		printf("内存溢出，超出分区大小!");
		p[choice - 1].SZ -= size;
	}
}

void FF()//首先适应算法
{
	printf("----------------------FF算法-------------------------\n");
	int i, j, n = PN;
	bool flag = true;
	Part p[max];
	Copy(PT, p);
	while (n > 1 && flag)
	{
		flag = false;
		for (i = 0; i < n - 1; i++)
			if (p[i].SA>p[i + 1].SA)
			{
				Part temp = p[i];
				p[i] = p[i + 1];
				p[i + 1] = temp;
				flag = true;
			}
		n--;
	}
	printf("地址排序后:\n");
	print(p);

	Judge(p);
	Sort(p); Copy(p, PT);
	print(PT);
}
void BF()//最佳适应算法
{
	printf("----------------------BF算法-------------------------\n");
	int n = PN, i, j;
	bool flag = true;
	Part p[max];
	Copy(PT, p);
	while (n > 1 && flag)
	{
		flag = false;
		for (i = 0; i < n - 1; i++)
			if (p[i].SZ>p[i + 1].SZ)
			{
				Part temp = p[i];
				p[i] = p[i + 1];
				p[i + 1] = temp;
				flag = true;
			}
		n--;
	}
	printf("大小排序后:\n");
	print(p);
	Judge(p);
	Sort(p); Copy(p, PT);
	print(PT);
}
void init()
{
	int i;
	bool flag;
	printf("初始化:请输入分区数量: ");
	scanf("%d", &PN);
	printf("请分别输入分区号、起始地址、分区大小\n分区号 始址 大小\n");
	for (i = 0; i < PN; i++)
	{
		printf("   ", i + 1);
		scanf("%d%d%d", &(PT[i].ID), &(PT[i].SA), &(PT[i].SZ));
	}
	flag = Check(PT);
	if(!flag)
	{
		printf("地址冲突! 请重兴初始化!\n");
		init();
	}
}
void Apply()//作业申请
{
	int i;
	int choice;
   printf("输入作业数量:   "); scanf("%d", &WN);
		printf("输入作业大小:\n作业号 大小\n");
		for (i = 0; i < WN; i++)
		{
			printf("%3d     ", i + 1);
			scanf("%d", &work[i]);
		}
		printf("请选择:(1)FF (2)BF   ");
		scanf("%d", &choice);
		if (choice == 1)FF();
		if (choice == 2)BF();
}
void WF()
{
	printf("----------------------WF算法-------------------------\n");
	int n = PN, i, j;
	bool flag = true;
	Part p[max];
	Copy(PT, p);
	while (n > 1 && flag)
	{
		flag = false;
		for (i = 0; i < n - 1; i++)
			if (p[i].SZ<p[i + 1].SZ)
			{
				Part temp = p[i];
				p[i] = p[i + 1];
				p[i + 1] = temp;
				flag = true;
			}
		n--;
	}
	printf("分区大小排序后:\n");
	print(p);
	Judge(p);
	Sort(p); Copy(p, PT);
	print(p);
}
int  main()
{
	setvbuf(stdout,NULL,_IONBF,0);
	bool flag = true;
	int choice;
	Menu();
	init();
	print(PT);
	CHOICE: printf("请选择 (1)作业申请 (2)内存释放  ");
	scanf("%d", &choice);
	if(choice==1) Apply();
	if(choice== 2)Free(PT);
	int n;
	printf("是否继续? 1继续0退出:");
	scanf("%d", &n);
	if (n == 1) goto CHOICE;
	else return 0;
}
