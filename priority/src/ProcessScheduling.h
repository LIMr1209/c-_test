/*
 * ProcessScheduling.h
 *
 *  Created on: 2021年10月26日
 *      Author: LIMr
 */

#ifndef ProcessScheduling_h
#define ProcessScheduling_h

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define TIME_SLICE 2

//线程状态：等待 就绪  完成
enum process_type{
    process_type_waitting = 'W',
    process_type_ready = 'R',
    process_type_finish = 'F'
};

//进程控制块结构体
typedef struct PCB_Type{
    //进程的名字
    char *name;
    //进程的优先级
    int priority;
    //仍需运行时间
    int need_time;
    //进程的状态 就绪  等待
    char state;
    //时间片
    int time_slice;
    //cpu时间 ->已运行时间
    int cpu_time;
    //计数器
    int time_count;
    //总共需要的运行时间
    int total_time;

    //下一个要执行的进程
    struct PCB_Type *next;
}PCB;

//创建新的进程
void create_process(PCB *running_list,char *name,int need_time);
void create_process1(PCB *running_list,char *name,int need_time,int priority);
//展示当前就绪队列状态
void show(PCB *running_list);

//优先级调度法
void priority_scheduling(PCB *running_list);
//按优先级大小对队列进行排序
void sort(PCB *running_list);
//找到当前未执行的优先级最高的进程，将它状态变为就绪
void set_ready(PCB *running_list);

#endif /* ProcessScheduling_h */
