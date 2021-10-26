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
int PN;//��������
int WN;//��ҵ����
int work[max];
typedef struct Partition
{
	int ID;//������
	int SA;//Starting Address ��ʼ��ַ
	int SZ;//size��С
}Part;
Part PT[max];//����ԭʼ����
void Menu()
{
	printf("------------------���������㷨-------------------\n");
	printf("*              1.�״���Ӧ�㷨FF(First Fit)     *\n");
	printf("*              2.�����Ӧ�㷨BF(Best Fit)      *\n");
	printf("*              3.���Ӧ�㷨WF(Worst Fit)     *\n");
	printf("*              4.�ڴ��ͷ�                      *\n");
	printf("*              0.�˳�                          *\n");
	printf("-------------------------------------------------\n");
}
void print(Part p[])//��ӡ���
{
	int i,j;
	for (i = 0; i < PN; i++)
	{
		if (p[i].SZ == 0)//���ڴ�СΪ��ķ�����ɾ��
		{
			printf("ɾ��%d�ŷ���\n", i + 1);
          for (j = i; j < PN - 1; j++)
				 p[j] = p[j + 1];
		  PN--;
		}
	}
	printf("������ ʼַ ��С\n");
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
void Sort(Part p[])//WF��BF�㷨ʹ�������ң������������
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
void Judge(Part p[])//�ж���ҵ�Ƿ�����
{
	int i, j;
	bool flag, free[max];
	for (i = 0; i < PN; i++)
		free[i] = true;
	for (i = 0; i < WN; i++)
	{
		flag = true;
		for (j = 0; j < PN&&flag; j++)
			if (work[i] <= p[j].SZ&&free[j])//��j�����з��������i����ҵ����
			{
				p[j].SA += work[i];
				p[j].SZ -= work[i];
				flag = false;
				free[j] = false;
			}
		if (flag)
		{
			printf("�ڴ治��!��ҵ%d��ȴ�.\n", i + 1); break;
		}
	}
}
bool Check(Part p[])//�������ڴ��Ƿ�Խ�����
{
	int i,j;
	bool flag = true;
	for (i = 1; flag&&i < PN; i++)
		if (p[i].SA <( p[i - 1].SA + p[i-1].SZ))
			flag = false;
		else if (p[i].SA ==(p[i - 1].SA + p[i-1].SZ))
		{
			printf("�ϲ�����\n");
			p[i - 1].SZ += p[i].SZ;
			for (j = i; j < PN-1; j++)
				p[j] = p[j + 1];
			PN--;
			print(p);
		}
	return flag;
}
void Free(Part p[])//�ͷ��ڴ棬ʵ������������з���
{
	int i, choice, size;
	bool flag;
	printf("�������ͷ��ڴ�ķ����źʹ�С  ");
	scanf("%d%d", &choice, &size);
	Sort(p);
	p[choice - 1].SZ += size;
	flag=Check(p);
	if (flag) {
		printf("�ͷųɹ�!\n"); print(p);
	}
	else
	{
		printf("�ڴ����������������С!");
		p[choice - 1].SZ -= size;
	}
}

void FF()//������Ӧ�㷨
{
	printf("----------------------FF�㷨-------------------------\n");
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
	printf("��ַ�����:\n");
	print(p);

	Judge(p);
	Sort(p); Copy(p, PT);
	print(PT);
}
void BF()//�����Ӧ�㷨
{
	printf("----------------------BF�㷨-------------------------\n");
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
	printf("��С�����:\n");
	print(p);
	Judge(p);
	Sort(p); Copy(p, PT);
	print(PT);
}
void init()
{
	int i;
	bool flag;
	printf("��ʼ��:�������������: ");
	scanf("%d", &PN);
	printf("��ֱ���������š���ʼ��ַ��������С\n������ ʼַ ��С\n");
	for (i = 0; i < PN; i++)
	{
		printf("   ", i + 1);
		scanf("%d%d%d", &(PT[i].ID), &(PT[i].SA), &(PT[i].SZ));
	}
	flag = Check(PT);
	if(!flag)
	{
		printf("��ַ��ͻ! �����˳�ʼ��!\n");
		init();
	}
}
void Apply()//��ҵ����
{
	int i;
	int choice;
   printf("������ҵ����:   "); scanf("%d", &WN);
		printf("������ҵ��С:\n��ҵ�� ��С\n");
		for (i = 0; i < WN; i++)
		{
			printf("%3d     ", i + 1);
			scanf("%d", &work[i]);
		}
		printf("��ѡ��:(1)FF (2)BF   ");
		scanf("%d", &choice);
		if (choice == 1)FF();
		if (choice == 2)BF();
}
void WF()
{
	printf("----------------------WF�㷨-------------------------\n");
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
	printf("������С�����:\n");
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
	CHOICE: printf("��ѡ�� (1)��ҵ���� (2)�ڴ��ͷ�  ");
	scanf("%d", &choice);
	if(choice==1) Apply();
	if(choice== 2)Free(PT);
	int n;
	printf("�Ƿ����? 1����0�˳�:");
	scanf("%d", &n);
	if (n == 1) goto CHOICE;
	else return 0;
}
