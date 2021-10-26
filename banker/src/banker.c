#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> //bool类型数据头函数
#define processNum 5 //五个进程
#define resourceNum 3 //三类资源

//https://blog.csdn.net/qq_43402639/article/details/102668226
//初始状态
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
    printf("系统剩余资源数:");
    for (j = 0; j < resourceNum; j++)
        printf("%d ", Available[j]);
    printf("\n");
    printf("============当前资源分配状态=========\n");
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
//进程不安全，回收预分配的资源
void recycle()
{
    int j;
    for (j = 0; j < resourceNum; j++) {
        Need[p][j] += Request[j];
        Available[j] += Request[j];
        Allocation[p][j] -= Request[j];
    }
    printf("系统进程资源状态不改变！\n");
}
void Test_safety()
{
    int i, j;
    int finish = 0, Done = 0; //Done一轮遍历下完成的，finish总共完成的
    int safeseries[processNum] = { -1,-1,-1,-1,-1 };
    //初始化
    for (i = 0; i < processNum; i++)
        Finish[i] = false;
    for (j = 0; j < resourceNum; j++)
        work[j] = Available[j]; //初始值等于Available;
    // 查找未完成进程，且当前进程尚需资源不大于系统剩余资源;
    i = 0;
    while (finish != processNum) {
        j = 0;
        if (Finish[i] == false)
            for (j = 0; j < resourceNum; j++)
                if (Need[i][j] > work[j]) break;

        if (j == resourceNum) {
            Finish[i] = true;
            print2(i);
            safeseries[finish++] = i; //记录下安全序列
            for (j = 0; j < resourceNum; j++)
                work[j] += Allocation[i][j];
        }
        i++; //下一个进程
        //一轮遍历后，判断是否还有可分配进程
        if (i >= processNum) {
            i = i % processNum;
            if (Done == finish) //判断本轮完成进程是否等于上一轮，是则代表没有可执行进程
                break;
            else Done = finish; //否则将本轮完成进程数赋值给Done
        }
    }
    if (finish == processNum) {
        printf("进程P%d请求通过,此时安全序列为：", p);
        for (i = 0; i < processNum; i++)
            printf("p%d ", safeseries[i]);
        printf("\n");
        print1(); //打印出此刻系统资源分配状态
    }
    else {
        recycle();
        printf("进程死锁，进程P%d请求无法通过!\n", p);
        print1();
    }
}
void judge_assign()
{
    int j;
    for (j = 0; j < resourceNum; j++) {
        //当前请求资源加上已分配资源不能大于最大需求资源；
        if (Request[j] + Allocation[p][j] > Max[p][j]) {
            printf("当前请求资源+已分配资源>最大需求资源:无法满足!错误!\n");
            break;
        }
        //当前请求资源不能大于系统现有资源；
        if (Request[j] > Available[j]) {
            printf("当前请求资源>系统现有资源:无法满足!错误!\n");
            break;
        }
    }
    if (j == resourceNum) {
        //尝试分配资源
        for (j = 0; j < resourceNum; j++) {
            Need[p][j] -= Request[j];
            Available[j] -= Request[j];
            Allocation[p][j] += Request[j];
        }
        //检查此时系统的安全性
        printf("===========安全序列===========\n");
        printf("process=== Work===== Need =====Allocation=====work+allocation==finish\n");
        Test_safety();

    }
}
int main()
{
	setvbuf(stdout,NULL,_IONBF,0);
    int i;
    print1();
    printf("===========此时安全序列===========\n");
    printf("process=== Work===== Need =====Allocation=====work+allocation==finish\n");
    Test_safety();
    while (1) {
        printf("存在进程0,1,2,3,4,资源类别0,1,2\n请依次输入请求资源的进程和进程请求的A,B,C类资源数\n例如：1 0 0 1 \n");
        scanf_s("%d", &p);
        for (i = 0; i < resourceNum; i++)
            scanf_s("%d", &Request[i]);
        //尝试分配资源给进程
        judge_assign();
    }
    return 0;
}
