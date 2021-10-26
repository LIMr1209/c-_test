/*
 * ProcessScheduling.c
 *
 *  Created on: 2021��10��26��
 *      Author: LIMr
 */


#include "ProcessScheduling.h"

//�����µĽ���
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

    //Ĭ�����ȼ�һ��
    p->priority = 1;

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

//��ת��
void round_robin(PCB *running_list){
    /*
     ÿ����������̺󣬻Ὣ�ý��̴Ӷ��׵�����β
     ����һ�����е����������һ�����Ľ���
     ������ɵĽ��̴�����ĩβ
     */
    //��¼��һ������λ��
    PCB *index = running_list->next;
    if (index == NULL) {
        printf("��ǰ�����ѿ�\n");
        return;
    }


    while (index != NULL && index->state != process_type_finish) {
        //��ʱ��Ƭ���иý��̣����޸Ľ��̵�cpuʱ�����Ҫʱ�䡢������
        PCB *s = index;
        s->time_slice = TIME_SLICE;

        //cpuʱ�䣨��������ʱ��) = ����ʱ�� -����ǰcpuʱ��+ʱ��Ƭ)
        //���������ֱ����ʾ����ʱ��
        s->cpu_time = (s->cpu_time + TIME_SLICE)<s->total_time?s->cpu_time + TIME_SLICE:s->total_time;
        //����ǰ����ʱ���ʱ��ƬС�ڵ����㣬��˵�����������
        s->need_time = (s->need_time - TIME_SLICE)>0?s->need_time - TIME_SLICE:0;
        //������+1
        s->time_count += 1;

        //�жϸý����Ƿ����
        if (s->need_time == 0) {
            //�޸Ľ���״̬
            s->state = process_type_finish;
        }else{
            s->state = process_type_waitting;
        }

        //���ý��̵�����β
        //1.ͷָ��ָ����Ԫ������һ�����
        running_list->next = s->next;

        //2.������������������뵽δ��ɶ��е���β��(���������ɵĶ���)
        PCB *p = running_list;
        //2.1Ѱ�Ҳ���λ��
        while (p->next != NULL && p->next->state != process_type_finish) {
            p = p->next;
        }
        //2.2�жϲ���λ��
        if (p->next == NULL) {
            //���һ��
            p->next = s;
            p->next->next = NULL;
        }else{
            //�������һ����������н��
            s->next = p->next;
            p->next = s;
        }

        //����ָ����Ԫ���
        index = running_list->next;
        set_readyR(running_list);
        //չʾ��ǰ����״��
        show(running_list);
    }
}
//�ҵ���ǰ�����е�һ�����̣�����״̬��Ϊ����
void set_readyR(PCB *running_list){
    PCB *s = running_list->next;
    if (s == NULL) {
        printf("��ǰ�����ѿ�\n");
        return;
    }

    if (s->state != process_type_finish) {
        s->state = process_type_ready;
        return;
    }
}
