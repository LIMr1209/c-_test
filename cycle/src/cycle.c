/*
 ============================================================================
 Name        : cycle.c
 Author      : limr
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "ProcessScheduling.h"
//https://blog.csdn.net/weixin_42643216/article/details/102897159

int main(int argc, const char * argv[]) {

	setvbuf(stdout,NULL,_IONBF,0);
    //运行(就绪)队列(头结点不储存信息)
    PCB *running_list = (PCB *)malloc(sizeof(PCB));
    running_list->next = NULL;

    int p_number;
    printf("请输入要创建的进程数目:\n");
    scanf("%d",&p_number);


    printf("请输入进程名字和所需时间:\n");
    for (int i = 0; i < p_number; i++) {
        //create(running_list);
        char *name = (char *)malloc(sizeof(char));
        int time;
        scanf("%s %d",name,&time);
        create_process(running_list, name, time);
    }

    //轮转法
    set_readyR(running_list);
    printf("调度前:\n");
    show(running_list);
    printf("调度后:\n");
    round_robin(running_list);

    return 0;
}
