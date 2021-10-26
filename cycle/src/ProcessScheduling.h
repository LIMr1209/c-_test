/*
 * ProcessScheduling.h
 *
 *  Created on: 2021��10��26��
 *      Author: LIMr
 */


#ifndef ProcessScheduling_h
#define ProcessScheduling_h

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define TIME_SLICE 2

//�߳�״̬���ȴ� ����  ���
enum process_type{
    process_type_waitting = 'W',
    process_type_ready = 'R',
    process_type_finish = 'F'
};

//���̿��ƿ�ṹ��
typedef struct PCB_Type{
    //���̵�����
    char *name;
    //���̵����ȼ�
    int priority;
    //��������ʱ��
    int need_time;
    //���̵�״̬ ����  �ȴ�
    char state;
    //ʱ��Ƭ
    int time_slice;
    //cpuʱ�� ->������ʱ��
    int cpu_time;
    //������
    int time_count;
    //�ܹ���Ҫ������ʱ��
    int total_time;

    //��һ��Ҫִ�еĽ���
    struct PCB_Type *next;
}PCB;

//�����µĽ���
void create_process(PCB *running_list,char *name,int need_time);
//չʾ��ǰ��������״̬
void show(PCB *running_list);

//��ת��
void round_robin(PCB *running_list);
//�ҵ���ǰ�����е�һ�����̣�����״̬��Ϊ����
void set_readyR(PCB *running_list);

#endif /* ProcessScheduling_h */

