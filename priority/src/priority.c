/*
 ============================================================================
 Name        : priority.c
 Author      : limr
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "ProcessScheduling.h"

int main(int argc, const char * argv[]) {
	setvbuf(stdout,NULL,_IONBF,0);

    //����(����)����(ͷ��㲻������Ϣ)
    PCB *running_list = (PCB *)malloc(sizeof(PCB));
    running_list->next = NULL;

    int p_number;
    printf("������Ҫ�����Ľ�����Ŀ:\n");
    scanf("%d",&p_number);

//    ������������ȼ�
//    printf("������������ֺ�����ʱ��:\n");
//    for (int i = 0; i < p_number; i++) {
//        //create(running_list);
//        char *name = (char *)malloc(sizeof(char));
//        int time;
//        scanf("%s %d",name,&time);
//        create_process(running_list, name, time);
//    }

    printf("������������ֺ�����ʱ�䡢���ȼ�:\n");
    for (int i = 0; i < p_number; i++) {
        //create(running_list);
        char *name = (char *)malloc(sizeof(char));
        int time,priority;
        scanf("%s %d %d",name,&time,&priority);
        create_process1(running_list, name, time,priority);
    }

    //���ȼ����ȷ�
    set_ready(running_list);
    printf("����ǰ:\n");
    show(running_list);
    printf("���Ⱥ�:\n");
    priority_scheduling(running_list);

    return 0;
}
