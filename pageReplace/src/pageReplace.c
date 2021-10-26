#include <stdio.h>

//https://blog.csdn.net/weixin_42643216/article/details/103446548

//��ʼ������
void initializeList(int list[],int number){
    for (int i = 0; i < number; i ++) {
        list[i] = -1;
    }
}
//չʾ����״̬
void showList(int list[], int number){
    for (int i = 0; i < number; i ++) {
        printf("%2d",list[i]);
    }
    printf("\n");
}

//չʾ��ǰ�ڴ�״̬
void showMemoryList(int list[],int phyBlockNum){
    for (int i = 0; i < phyBlockNum; i ++) {
        if (list[i] == -1) {
            break;
        }
        printf(" |%d|",list[i]);
    }
    printf("\n");
}

void informationCount(int missingCount,int replaceCount,int pageNum){
    printf("ȱҳ����:%d   ȱҳ��:%d/%d\n",missingCount,missingCount,pageNum);
    double result = (double)(pageNum - missingCount)/(double)pageNum;
    printf("�û�����:%d  ������:%.2f\n",replaceCount,result);
}

//�ҵ���ҳ���´�Ҫ���ʵ�λ��
int getNextPosition(int currentPage,int currentPosition,int strList[],int pageNum){

    for (int i = currentPosition+1; i < pageNum; i ++) {
        if (strList[i] == currentPage) {
            return i;
        }
    }

    return 100;
}

//����û��㷨
void replacePageByOPT(int memoryList[],int phyNum,int strList[],int pageNum){

    //�û�����
    int replaceCount = 0;
    //ȱҳ����
    int missingCount = 0;

    //��¼���ڴ����������һ�η���λ��
    int nextPosition[phyNum];
    //��ʼ��
    initializeList(nextPosition, phyNum);

    //��¼��ǰҳ��ķ������: 0 δ����
    int isVisited;

    for (int i = 0; i < pageNum; i ++) {
        isVisited = 0;
        //�ж��Ƿ���Ҫ�û�->�ڴ���������Ҫ���ʵ�ҳ�治���ڴ���
        for (int j = 0; j < phyNum; j ++) {
            if (memoryList[j] == strList[i]) {
                //��ҳ���Ѿ������ڴ���
                //��¼��һ�η�������λ��
                nextPosition[j] = getNextPosition(memoryList[j], i, strList, pageNum);

                //�޸ķ������
                isVisited = 1;

                //չʾ
                printf("%d\n",strList[i]);
                break;
            }
            if (memoryList[j] == -1) {
                //ҳ�治���ڴ������ڴ�δ��->ֱ�Ӵ���
                memoryList[j] = strList[i];
                nextPosition[j] = getNextPosition(memoryList[j], i, strList, pageNum);

                missingCount ++;

                //�޸ķ������
                isVisited = 1;

                //չʾ
                printf("%d\n",strList[i]);
                showMemoryList(memoryList, phyNum);
                break;
            }
        }

        if (!isVisited) {

            //��ǰҳ�滹û���ʹ�
            //�ڴ������ҵ�ǰ���ʲ����ڴ���->�����û�
            //1.Ѱ�ҵ�����ű����ʵ���ҳ��
            int max = 0;
            for (int k = 1; k < phyNum; k ++) {
                if (nextPosition[max] < nextPosition[k]) {
                    max = k;
                }
            }


            //2.����λ�õ�ҳ�滻��
            memoryList[max] = strList[i];
            nextPosition[max] = getNextPosition(memoryList[max], i, strList, pageNum);

            missingCount ++;
            replaceCount ++;

            //չʾ
            printf("%d\n",strList[i]);
            showMemoryList(memoryList, phyNum);
        }
    }
    informationCount(missingCount, replaceCount,pageNum);
}
//�Ƚ��ȳ��û��㷨
void replacePageByFIFO(int memoryList[],int phyNum,int strList[],int pageNum){

    //�û�����
    int replaceCount = 0;
    //ȱҳ����
    int missingCount = 0;

    //��¼��ǰ��������ڴ���±�
    int pointer = 0;

    //��¼��ǰҳ��ķ������: 0 δ����
    int isVisited = 0;
    for (int i = 0; i < pageNum; i ++) {
        isVisited = 0;

        //�ж��Ƿ���Ҫ�û�->�ڴ���������Ҫ���ʵ�ҳ�治���ڴ���
        for (int j = 0; j < phyNum; j ++) {
            if (memoryList[j] == strList[i]) {
                //��ҳ���Ѿ������ڴ���
                //�޸ķ������
                isVisited = 1;
                //�޸ķ���ʱ��
                //չʾ
                printf("%d\n",strList[i]);
                break;
            }
            if (memoryList[j] == -1) {
                //ҳ�治���ڴ������ڴ�δ��->ֱ�Ӵ���
                memoryList[j] = strList[i];
                //�޸ķ������
                isVisited = 1;
                missingCount ++;
                //չʾ
                printf("%d\n",strList[i]);
                showMemoryList(memoryList, phyNum);
                break;
            }
        }

        if (!isVisited) {
            //��ǰҳ�滹δ�����ʹ�->��Ҫ����ҳ���û�
            //ֱ�Ӱ����ҳ��浽����¼���±���
            memoryList[pointer] = strList[i];

            //�±�ָ����һ��
            pointer ++;

            //����������һ�������±����
            if (pointer > phyNum-1) {
                pointer = 0;
            }


            missingCount ++;
            replaceCount ++;

            //չʾ
            printf("%d\n",strList[i]);
            showMemoryList(memoryList, phyNum);
        }
    }
    informationCount(missingCount, replaceCount, pageNum);
}

//������δʹ���û��㷨
void replacePageByLRU(int memoryList[],int phyNum,int strList[],int pageNum){

    //�û�����
    int replaceCount = 0;
    //ȱҳ����
    int missingCount = 0;

    //��¼�ڴ������һ�η��������ʱ��
    int timeRecord[phyNum];
    //��ʼ��
    initializeList(timeRecord, phyNum);

    //��¼��ǰҳ��ķ������: 0 δ����
    int isVisited = 0;

    //��¼�Ѿ����ڴ��е�ҳ������
    int pageCount = 0;
    for (int i = 0; i < pageNum; i ++) {
        isVisited = 0;

        //ʱ���һ
        for (int p = 0; p < pageCount; p ++) {
            if (memoryList[p] != -1) {
                timeRecord[p] ++;
            }
        }

        //�Ƿ���Ҫ�û�
        for (int j = 0; j < phyNum; j ++) {
            if (memoryList[j] == strList[i]) {
                //��ҳ���Ѿ������ڴ���
                //�޸ķ������
                isVisited = 1;
                //���÷���ʱ��
                timeRecord[j] = -1;
                //չʾ
                printf("%d\n",strList[i]);
                break;
            }
            if (memoryList[j] == -1) {
                //ҳ�治���ڴ������ڴ�δ��->ֱ�Ӵ���
                memoryList[j] = strList[i];
                pageCount ++;
                //�޸ķ������
                isVisited = 1;
                //�޸ķ���ʱ��
                timeRecord[j] ++;

                missingCount ++;
                //չʾ
                printf("%d\n",strList[i]);
                showMemoryList(memoryList, phyNum);
                break;
            }
        }

        if (!isVisited) {
            //��Ҫ�û�
            //1.����ʱ���¼��Ѱ�����δ���ʵ�ҳ�����ڵ��ڴ��±�
            int max = 0;
            for (int k = 0; k < phyNum; k ++) {
                if (timeRecord[max] < timeRecord[k]) {
                    max = k;
                }
            }

            //2.����λ�õ�ҳ�滻��
            memoryList[max] = strList[i];
            timeRecord[max] = -1;

            missingCount ++;
            replaceCount ++;

            //չʾ
            printf("%d\n",strList[i]);
            showMemoryList(memoryList, phyNum);

        }
    }
    informationCount(missingCount, replaceCount, pageNum);
}

//������δʹ���û��㷨-2
//void replacePageByLRU(int memoryList[],int phyNum,int strList[],int pageNum){
//
//    int isVisited = 0;
//
//    //��¼�Ѿ����ڴ��е�ҳ������
//    int pageCount = 0;
//    for (int i = 0; i < pageNum; i ++) {
//        isVisited = 0;
//        //�ж��ڴ��Ƿ���Ҫ��ҳ
//        for (int j = 0; j < phyNum; j ++) {
//            if (memoryList[j] == strList[i]) {
//                //�Ѿ��������ڴ��У�������������β��
//                int temp = memoryList[j];
//                for (int k = j; k < pageCount-1; k ++) {
//                    memoryList[k] = memoryList[k+1];
//                }
//                memoryList[pageCount-1] = temp;
//
//                //�޸ķ������
//                isVisited = 1;
//                //�޸ķ���ʱ��
//                //չʾ
//                printf("%d\n",strList[i]);
//                break;
//            }
//
//            if (memoryList[j] == -1) {
//                //ҳ�治���ڴ������ڴ�δ��->ֱ�Ӵ���
//                memoryList[j] = strList[i];
//                //�޸ķ������
//                isVisited = 1;
//
//                pageCount ++;
//
//                //չʾ
//                printf("%d\n",strList[i]);
//                showMemoryList(memoryList, phyNum);
//                break;
//            }
//        }
//
//        if (!isVisited) {
//            //��Ҫ��ҳ
//            //1.ֱ�ӽ�����������ǰ��һλ
//            for (int k = 0; k < phyNum; k ++) {
//                memoryList[k] = memoryList[k+1];
//            }
//            //2.����ǰҳ��ӵ���β
//            memoryList[phyNum-1] = strList[i];
//            //չʾ
//            printf("%d\n",strList[i]);
//            showMemoryList(memoryList, phyNum);
//        }
//
//
//    }
//}
int main(int argc, const char * argv[]) {

    //����������
    int phyBlockNum;
    setvbuf(stdout,NULL,_IONBF,0);
    printf("���������������:\n");
    scanf("%d",&phyBlockNum);

    //�����ڴ����
    int memoryList[phyBlockNum];
    //��ʼ���ڴ�״̬
    initializeList(memoryList, phyBlockNum);
    //showMemoryList(memoryList,phyBlockNum);

    //ҳ������
    int pageNum;
    printf("������Ҫ���ʵ�ҳ������:\n");
    scanf("%d",&pageNum);

    //����ҳ������ô�
    int pageNumStrList[pageNum];
    printf("������Ҫ���ʵ�ҳ���:\n");
    for (int i = 0; i < pageNum; i ++) {
        scanf("%d",&pageNumStrList[i]);
    }
    showList(pageNumStrList, pageNum);

    int chose;
    while (1) {
        printf("��ѡ��������û��㷨:\n");
        printf("1.OPT 2.FIFO 3.LRU 4.�˳�\n");
        scanf("%d",&chose);

        switch (chose) {
            case 1:
                showList(pageNumStrList, pageNum);
                replacePageByOPT(memoryList, phyBlockNum, pageNumStrList, pageNum);
                //���³�ʼ���ڴ�
                initializeList(memoryList, phyBlockNum);
                break;
            case 2:
                showList(pageNumStrList, pageNum);
                replacePageByFIFO(memoryList, phyBlockNum, pageNumStrList, pageNum);
                //���³�ʼ���ڴ�
                initializeList(memoryList , phyBlockNum);
                break;
            case 3:
                showList(pageNumStrList, pageNum);
                replacePageByLRU(memoryList, phyBlockNum, pageNumStrList, pageNum);
                //���³�ʼ���ڴ�
                initializeList(memoryList, phyBlockNum);
                break;
            default:
                return 0;
                break;
        }
    }

    return 0;
}
