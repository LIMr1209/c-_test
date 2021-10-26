/*
 * ProcessScheduling.c
 *
 *  Created on: 2021��10��26��
 *      Author: LIMr
 */


#include "ProcessScheduling.h"

//�����µĽ���->���ȼ����������
void create_process(PCB *running_list,char *name,int need_time){
    //����һ���ڴ���ƿ�Ŀռ�
    PCB *p = (PCB *)malloc(sizeof(PCB));
    assert(p != NULL);

    //���øÿ��ƿ��ֵ
    p->name = name;
    p->need_time = need_time;

    //״̬
    p->state = process_type_waitting;
    //ʱ��Ƭ
    p->time_slice = 0;
    //cpuʱ��
    p->cpu_time = 0;
    //������
    p->time_count = 0;
    //������ʱ
    p->total_time = need_time;

    //���ȼ�->���������
    srand((unsigned)time(NULL));
    int priority = rand() % 50 + 1;
    p->priority = priority;

    //�¸�����
    p->next = NULL;

    //�������ж�����
    PCB *s = running_list;
    while (s->next != NULL) {
        s = s->next;
    }
    s->next = p;

}

void create_process1(PCB *running_list,char *name,int need_time,int priority){
    //����һ���ڴ���ƿ�Ŀռ�
    PCB *p = (PCB *)malloc(sizeof(PCB));
    assert(p != NULL);

    //���øÿ��ƿ��ֵ
    p->name = name;
    p->need_time = need_time;

    //״̬
    p->state = process_type_waitting;
    //ʱ��Ƭ
    p->time_slice = 0;
    //cpuʱ��
    p->cpu_time = 0;
    //������
    p->time_count = 0;
    //������ʱ
    p->total_time = need_time;

    //���ȼ�
    p->priority = priority;

    //�¸�����
    p->next = NULL;

    //�������ж�����
    PCB *s = running_list;
    while (s->next != NULL) {
        s = s->next;
    }
    s->next = p;
}

//չʾ��ǰ��������״̬
void show(PCB *running_list){
    PCB *p = running_list->next;
    if (p == NULL) {
        printf("��ǰ�������޽���\n");
        return;
    }

    printf("������  ���ȼ�  ʱ��Ƭ  cpuʱ��  ��Ҫʱ��  ����״̬  ������\n");
    while (p != NULL) {
        printf("%s    %4d   %4d   %4d     %4d       %c    %4d\n",p->name,p->priority,p->time_slice,p->cpu_time,p->need_time,p->state,p->time_count);

        p = p->next;
    }
    printf("\n");
}

//���ȼ������㷨
void priority_scheduling(PCB *running_list){

    //�����а����ȼ���С��������
    sort(running_list);

    //����������������ִ�н���
    PCB *s = running_list->next;
    while (s != NULL) {
        s->cpu_time = 1;
        s->time_count = 1;
        s->state = process_type_finish;

        s = s->next;
        set_ready(running_list);
        //ÿִ�������ν���չʾ��ǰ����״��
        show(running_list);
    }
}

//�����ȼ���С�Զ��н�������
void sort(PCB *running_list){
    PCB *s = running_list->next;
    PCB *p = s->next;
    if (s == NULL || p == NULL) {
        //��ʱ������ֻ��0��1����㣬��������
        return;
    }

    //������ӵ�һ���������Ͽ�
    s->next = NULL;
    //ͷָ��
    PCB *first = running_list;

    while (p != NULL) {
        s = p;
        p = p->next;

        //��s����Ͽ�����������
        PCB *q = first;
        //�����������ҵ�����λ��
        while (q->next !=NULL) {
            //�ж�s�������ȼ��Ƿ�ȵ�ǰ�ڵ�С
            if (s->priority < q->next->priority) {
                //�ǣ��������Ѱ��
                q = q->next;
            }else{
                //�񣬾����������
                s->next = q->next;
                q->next = s;
                break;
            }
        }

        if (q->next == NULL) {
            //��֮ǰ�����ȼ����ͣ��������
            q->next = s;
            s->next = NULL;
        }

    }

}

//�ҵ���ǰδִ�е����ȼ���ߵĽ��̣�����״̬��Ϊ����
void set_ready(PCB *running_list){

    PCB *s = running_list->next;
    if (s == NULL) {
        printf("��ǰ�����ѿ�\n");
        return;
    }

    /*
     �÷���ֻ����������֮��
//    while (s != NULL && s->state != process_type_waitting) {
//        s = s->next;
//    }
//    if (s != NULL) {
//        s->state = process_type_ready;
//    }
     */
    //��¼��ǰ���ȼ������ֵ
    int max = 0;
    //��¼��ֵ��Ӧ�Ľ���
    PCB *p = NULL;

    while (s != NULL) {
        //���ý��̲����ڵȴ�״̬ʱ��ֱ������
        if (s->state != process_type_waitting) {
            s = s->next;
            continue;
        }
        //��¼���ڵȴ�״̬�Ľ������ȼ������ֵ
        if (s->priority >= max) {
            max = s->priority;
            p = s;
        }
        s = s->next;
    }
    //PΪ��˵����ǰ�Ѿ�û�еȴ��еĽ�����
    if (p != NULL) {
        p->state = process_type_ready;
    }

}
