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
    //����(����)����(ͷ��㲻������Ϣ)
    PCB *running_list = (PCB *)malloc(sizeof(PCB));
    running_list->next = NULL;

    int p_number;
    printf("������Ҫ�����Ľ�����Ŀ:\n");
    scanf("%d",&p_number);


    printf("������������ֺ�����ʱ��:\n");
    for (int i = 0; i < p_number; i++) {
        //create(running_list);
        char *name = (char *)malloc(sizeof(char));
        int time;
        scanf("%s %d",name,&time);
        create_process(running_list, name, time);
    }

    //��ת��
    set_readyR(running_list);
    printf("����ǰ:\n");
    show(running_list);
    printf("���Ⱥ�:\n");
    round_robin(running_list);

    return 0;
}
