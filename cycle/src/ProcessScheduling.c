/*
 * ProcessScheduling.c
 *
 *  Created on: 2021年10月26日
 *      Author: LIMr
 */


#include "ProcessScheduling.h"

//创建新的进程
void create_process(PCB *running_list,char *name,int need_time){
    //申请一个内存控制块的空间
    PCB *p = (PCB *)malloc(sizeof(PCB));
    assert(p != NULL);

    //设置该控制块的值
    p->name = name;
    p->need_time = need_time;

    //状态
    p->state = process_type_waitting;
    //时间片
    p->time_slice = 0;
    //cpu时间
    p->cpu_time = 0;
    //计数器
    p->time_count = 0;
    //总需用时
    p->total_time = need_time;

    //默认优先级一致
    p->priority = 1;

    //下个进程
    p->next = NULL;

    //放入运行队列中
    PCB *s = running_list;
    while (s->next != NULL) {
        s = s->next;
    }
    s->next = p;

}

//展示当前就绪队列状态
void show(PCB *running_list){
    PCB *p = running_list->next;
    if (p == NULL) {
        printf("当前队列中无进程\n");
        return;
    }

    printf("进程名  优先级  时间片  cpu时间  需要时间  进程状态  计数器\n");
    while (p != NULL) {
        printf("%s    %4d   %4d   %4d     %4d       %c    %4d\n",p->name,p->priority,p->time_slice,p->cpu_time,p->need_time,p->state,p->time_count);

        p = p->next;
    }
    printf("\n");
}

//轮转法
void round_robin(PCB *running_list){
    /*
     每次运行完进程后，会将该进程从队首调到队尾
     即下一次运行的仍是链表第一个结点的进程
     最先完成的进程处于最末尾
     */
    //记录第一个结点的位置
    PCB *index = running_list->next;
    if (index == NULL) {
        printf("当前队列已空\n");
        return;
    }


    while (index != NULL && index->state != process_type_finish) {
        //按时间片运行该进程，即修改进程的cpu时间和需要时间、计数器
        PCB *s = index;
        s->time_slice = TIME_SLICE;

        //cpu时间（即已运行时间) = 总需时间 -（当前cpu时间+时间片)
        //若已完成则直接显示总需时间
        s->cpu_time = (s->cpu_time + TIME_SLICE)<s->total_time?s->cpu_time + TIME_SLICE:s->total_time;
        //若当前仍需时间减时间片小于等于零，则说明进程已完成
        s->need_time = (s->need_time - TIME_SLICE)>0?s->need_time - TIME_SLICE:0;
        //计数器+1
        s->time_count += 1;

        //判断该进程是否结束
        if (s->need_time == 0) {
            //修改进程状态
            s->state = process_type_finish;
        }else{
            s->state = process_type_waitting;
        }

        //将该进程调到队尾
        //1.头指针指向首元结点的下一个结点
        running_list->next = s->next;

        //2.遍历整个链表，将其插入到未完成队列的最尾端(其后是已完成的队列)
        PCB *p = running_list;
        //2.1寻找插入位置
        while (p->next != NULL && p->next->state != process_type_finish) {
            p = p->next;
        }
        //2.2判断插入位置
        if (p->next == NULL) {
            //最后一个
            p->next = s;
            p->next->next = NULL;
        }else{
            //不是最后一个，其后仍有结点
            s->next = p->next;
            p->next = s;
        }

        //重新指向首元结点
        index = running_list->next;
        set_readyR(running_list);
        //展示当前队列状况
        show(running_list);
    }
}
//找到当前队列中第一个进程，将它状态变为就绪
void set_readyR(PCB *running_list){
    PCB *s = running_list->next;
    if (s == NULL) {
        printf("当前队列已空\n");
        return;
    }

    if (s->state != process_type_finish) {
        s->state = process_type_ready;
        return;
    }
}
