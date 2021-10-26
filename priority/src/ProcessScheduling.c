/*
 * ProcessScheduling.c
 *
 *  Created on: 2021年10月26日
 *      Author: LIMr
 */


#include "ProcessScheduling.h"

//创建新的进程->优先级随机数生成
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

    //优先级->随机数生成
    srand((unsigned)time(NULL));
    int priority = rand() % 50 + 1;
    p->priority = priority;

    //下个进程
    p->next = NULL;

    //放入运行队列中
    PCB *s = running_list;
    while (s->next != NULL) {
        s = s->next;
    }
    s->next = p;

}

void create_process1(PCB *running_list,char *name,int need_time,int priority){
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

    //优先级
    p->priority = priority;

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

//优先级调度算法
void priority_scheduling(PCB *running_list){

    //将队列按优先级大小进行排序
    sort(running_list);

    //遍历整个链表依次执行进程
    PCB *s = running_list->next;
    while (s != NULL) {
        s->cpu_time = 1;
        s->time_count = 1;
        s->state = process_type_finish;

        s = s->next;
        set_ready(running_list);
        //每执行完依次进程展示当前队列状况
        show(running_list);
    }
}

//按优先级大小对队列进行排序
void sort(PCB *running_list){
    PCB *s = running_list->next;
    PCB *p = s->next;
    if (s == NULL || p == NULL) {
        //此时链表中只有0或1个结点，无须排序
        return;
    }

    //将链表从第一个结点往后断开
    s->next = NULL;
    //头指针
    PCB *first = running_list;

    while (p != NULL) {
        s = p;
        p = p->next;

        //将s插入断开的新链表中
        PCB *q = first;
        //遍历新链表，找到插入位置
        while (q->next !=NULL) {
            //判断s结点的优先级是否比当前节点小
            if (s->priority < q->next->priority) {
                //是，往后继续寻找
                q = q->next;
            }else{
                //否，就在这里插入
                s->next = q->next;
                q->next = s;
                break;
            }
        }

        if (q->next == NULL) {
            //比之前的优先级都低，插入最后
            q->next = s;
            s->next = NULL;
        }

    }

}

//找到当前未执行的优先级最高的进程，将它状态变为就绪
void set_ready(PCB *running_list){

    PCB *s = running_list->next;
    if (s == NULL) {
        printf("当前队列已空\n");
        return;
    }

    /*
     该方法只适用于排序之后
//    while (s != NULL && s->state != process_type_waitting) {
//        s = s->next;
//    }
//    if (s != NULL) {
//        s->state = process_type_ready;
//    }
     */
    //记录当前优先级的最大值
    int max = 0;
    //记录该值对应的进程
    PCB *p = NULL;

    while (s != NULL) {
        //当该进程不属于等待状态时，直接跳过
        if (s->state != process_type_waitting) {
            s = s->next;
            continue;
        }
        //记录处于等待状态的进程优先级的最大值
        if (s->priority >= max) {
            max = s->priority;
            p = s;
        }
        s = s->next;
    }
    //P为空说明当前已经没有等待中的进程了
    if (p != NULL) {
        p->state = process_type_ready;
    }

}
