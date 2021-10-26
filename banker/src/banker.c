#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> //bool��������ͷ����
#define processNum 5 //�������
#define resourceNum 3 //������Դ

//https://blog.csdn.net/qq_43402639/article/details/102668226
//��ʼ״̬
int p;
int work[resourceNum];
int Request[resourceNum];
bool Finish[processNum];
int Available[resourceNum] = { 3,3,2 };
int Max[processNum][resourceNum] = { 7,5,3,3,2,2,9,0,2,2,2,2,4,3,3 };
int Need[processNum][resourceNum] = { 7,4,3,1,2,2,6,0,0,0,1,1,4,3,1 };
int Allocation[processNum][resourceNum] = { 0,1,0,2,0,0,3,0,2,2,1,1,0,0,2 };
void print1()
{
    int i, j;
    printf("ϵͳʣ����Դ��:");
    for (j = 0; j < resourceNum; j++)
        printf("%d ", Available[j]);
    printf("\n");
    printf("============��ǰ��Դ����״̬=========\n");
    printf("process=== Max =====Allocation=====  Need\n");
    for (i = 0; i < processNum; i++) {
        printf("p%d        ", i);
        for (j = 0; j < resourceNum; j++)
            printf("%d ", Max[i][j]);
        printf("       ");
        for (j = 0; j < resourceNum; j++)
            printf("%d ", Allocation[i][j]);
        printf("       ");
        for (j = 0; j < resourceNum; j++)
            printf("%d ", Need[i][j]);
        printf("       \n");
    }
}
void print2(int i)
{
    int j;
    printf("p%d        ", i);
    for (j = 0; j < resourceNum; j++)
        printf("%d ", work[j]);
    printf("       ");
    for (j = 0; j < resourceNum; j++)
        printf("%d ", Need[i][j]);
    printf("       ");
    for (j = 0; j < resourceNum; j++)
        printf("%d ", Allocation[i][j]);
    printf("       ");
    for (j = 0; j < resourceNum; j++)
        printf("%d ", Allocation[i][j] + work[j]);
    printf("       ");
    for (j = 0; j < resourceNum; j++)
        if (!Finish[i])
            break;
    if (j == resourceNum)
        printf("true");
    else
        printf("false");
    printf("       \n");
}
//���̲���ȫ������Ԥ�������Դ
void recycle()
{
    int j;
    for (j = 0; j < resourceNum; j++) {
        Need[p][j] += Request[j];
        Available[j] += Request[j];
        Allocation[p][j] -= Request[j];
    }
    printf("ϵͳ������Դ״̬���ı䣡\n");
}
void Test_safety()
{
    int i, j;
    int finish = 0, Done = 0; //Doneһ�ֱ�������ɵģ�finish�ܹ���ɵ�
    int safeseries[processNum] = { -1,-1,-1,-1,-1 };
    //��ʼ��
    for (i = 0; i < processNum; i++)
        Finish[i] = false;
    for (j = 0; j < resourceNum; j++)
        work[j] = Available[j]; //��ʼֵ����Available;
    // ����δ��ɽ��̣��ҵ�ǰ����������Դ������ϵͳʣ����Դ;
    i = 0;
    while (finish != processNum) {
        j = 0;
        if (Finish[i] == false)
            for (j = 0; j < resourceNum; j++)
                if (Need[i][j] > work[j]) break;

        if (j == resourceNum) {
            Finish[i] = true;
            print2(i);
            safeseries[finish++] = i; //��¼�°�ȫ����
            for (j = 0; j < resourceNum; j++)
                work[j] += Allocation[i][j];
        }
        i++; //��һ������
        //һ�ֱ������ж��Ƿ��пɷ������
        if (i >= processNum) {
            i = i % processNum;
            if (Done == finish) //�жϱ�����ɽ����Ƿ������һ�֣��������û�п�ִ�н���
                break;
            else Done = finish; //���򽫱�����ɽ�������ֵ��Done
        }
    }
    if (finish == processNum) {
        printf("����P%d����ͨ��,��ʱ��ȫ����Ϊ��", p);
        for (i = 0; i < processNum; i++)
            printf("p%d ", safeseries[i]);
        printf("\n");
        print1(); //��ӡ���˿�ϵͳ��Դ����״̬
    }
    else {
        recycle();
        printf("��������������P%d�����޷�ͨ��!\n", p);
        print1();
    }
}
void judge_assign()
{
    int j;
    for (j = 0; j < resourceNum; j++) {
        //��ǰ������Դ�����ѷ�����Դ���ܴ������������Դ��
        if (Request[j] + Allocation[p][j] > Max[p][j]) {
            printf("��ǰ������Դ+�ѷ�����Դ>���������Դ:�޷�����!����!\n");
            break;
        }
        //��ǰ������Դ���ܴ���ϵͳ������Դ��
        if (Request[j] > Available[j]) {
            printf("��ǰ������Դ>ϵͳ������Դ:�޷�����!����!\n");
            break;
        }
    }
    if (j == resourceNum) {
        //���Է�����Դ
        for (j = 0; j < resourceNum; j++) {
            Need[p][j] -= Request[j];
            Available[j] -= Request[j];
            Allocation[p][j] += Request[j];
        }
        //����ʱϵͳ�İ�ȫ��
        printf("===========��ȫ����===========\n");
        printf("process=== Work===== Need =====Allocation=====work+allocation==finish\n");
        Test_safety();

    }
}
int main()
{
	setvbuf(stdout,NULL,_IONBF,0);
    int i;
    print1();
    printf("===========��ʱ��ȫ����===========\n");
    printf("process=== Work===== Need =====Allocation=====work+allocation==finish\n");
    Test_safety();
    while (1) {
        printf("���ڽ���0,1,2,3,4,��Դ���0,1,2\n����������������Դ�Ľ��̺ͽ��������A,B,C����Դ��\n���磺1 0 0 1 \n");
        scanf_s("%d", &p);
        for (i = 0; i < resourceNum; i++)
            scanf_s("%d", &Request[i]);
        //���Է�����Դ������
        judge_assign();
    }
    return 0;
}
