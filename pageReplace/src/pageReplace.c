#include <stdio.h>

//https://blog.csdn.net/weixin_42643216/article/details/103446548

//初始化队列
void initializeList(int list[],int number){
    for (int i = 0; i < number; i ++) {
        list[i] = -1;
    }
}
//展示队列状态
void showList(int list[], int number){
    for (int i = 0; i < number; i ++) {
        printf("%2d",list[i]);
    }
    printf("\n");
}

//展示当前内存状态
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
    printf("缺页次数:%d   缺页率:%d/%d\n",missingCount,missingCount,pageNum);
    double result = (double)(pageNum - missingCount)/(double)pageNum;
    printf("置换次数:%d  命中率:%.2f\n",replaceCount,result);
}

//找到该页面下次要访问的位置
int getNextPosition(int currentPage,int currentPosition,int strList[],int pageNum){

    for (int i = currentPosition+1; i < pageNum; i ++) {
        if (strList[i] == currentPage) {
            return i;
        }
    }

    return 100;
}

//最佳置换算法
void replacePageByOPT(int memoryList[],int phyNum,int strList[],int pageNum){

    //置换次数
    int replaceCount = 0;
    //缺页次数
    int missingCount = 0;

    //记录在内存的物理块的下一次访问位置
    int nextPosition[phyNum];
    //初始化
    initializeList(nextPosition, phyNum);

    //记录当前页面的访问情况: 0 未访问
    int isVisited;

    for (int i = 0; i < pageNum; i ++) {
        isVisited = 0;
        //判断是否需要置换->内存已满且需要访问的页面不在内存中
        for (int j = 0; j < phyNum; j ++) {
            if (memoryList[j] == strList[i]) {
                //该页面已经存在内存中
                //记录下一次访问它的位置
                nextPosition[j] = getNextPosition(memoryList[j], i, strList, pageNum);

                //修改访问情况
                isVisited = 1;

                //展示
                printf("%d\n",strList[i]);
                break;
            }
            if (memoryList[j] == -1) {
                //页面不在内存中且内存未满->直接存入
                memoryList[j] = strList[i];
                nextPosition[j] = getNextPosition(memoryList[j], i, strList, pageNum);

                missingCount ++;

                //修改访问情况
                isVisited = 1;

                //展示
                printf("%d\n",strList[i]);
                showMemoryList(memoryList, phyNum);
                break;
            }
        }

        if (!isVisited) {

            //当前页面还没访问过
            //内存已满且当前访问不在内存中->进行置换
            //1.寻找到最晚才被访问到的页面
            int max = 0;
            for (int k = 1; k < phyNum; k ++) {
                if (nextPosition[max] < nextPosition[k]) {
                    max = k;
                }
            }


            //2.将该位置的页面换出
            memoryList[max] = strList[i];
            nextPosition[max] = getNextPosition(memoryList[max], i, strList, pageNum);

            missingCount ++;
            replaceCount ++;

            //展示
            printf("%d\n",strList[i]);
            showMemoryList(memoryList, phyNum);
        }
    }
    informationCount(missingCount, replaceCount,pageNum);
}
//先进先出置换算法
void replacePageByFIFO(int memoryList[],int phyNum,int strList[],int pageNum){

    //置换次数
    int replaceCount = 0;
    //缺页次数
    int missingCount = 0;

    //记录当前最早进入内存的下标
    int pointer = 0;

    //记录当前页面的访问情况: 0 未访问
    int isVisited = 0;
    for (int i = 0; i < pageNum; i ++) {
        isVisited = 0;

        //判断是否需要置换->内存已满且需要访问的页面不在内存中
        for (int j = 0; j < phyNum; j ++) {
            if (memoryList[j] == strList[i]) {
                //该页面已经存在内存中
                //修改访问情况
                isVisited = 1;
                //修改访问时间
                //展示
                printf("%d\n",strList[i]);
                break;
            }
            if (memoryList[j] == -1) {
                //页面不在内存中且内存未满->直接存入
                memoryList[j] = strList[i];
                //修改访问情况
                isVisited = 1;
                missingCount ++;
                //展示
                printf("%d\n",strList[i]);
                showMemoryList(memoryList, phyNum);
                break;
            }
        }

        if (!isVisited) {
            //当前页面还未被访问过->需要进行页面置换
            //直接把这个页面存到所记录的下标中
            memoryList[pointer] = strList[i];

            //下标指向下一个
            pointer ++;

            //如果到了最后一个，将下标归零
            if (pointer > phyNum-1) {
                pointer = 0;
            }


            missingCount ++;
            replaceCount ++;

            //展示
            printf("%d\n",strList[i]);
            showMemoryList(memoryList, phyNum);
        }
    }
    informationCount(missingCount, replaceCount, pageNum);
}

//最近最久未使用置换算法
void replacePageByLRU(int memoryList[],int phyNum,int strList[],int pageNum){

    //置换次数
    int replaceCount = 0;
    //缺页次数
    int missingCount = 0;

    //记录内存中最近一次访问至今的时间
    int timeRecord[phyNum];
    //初始化
    initializeList(timeRecord, phyNum);

    //记录当前页面的访问情况: 0 未访问
    int isVisited = 0;

    //记录已经在内存中的页面数量
    int pageCount = 0;
    for (int i = 0; i < pageNum; i ++) {
        isVisited = 0;

        //时间加一
        for (int p = 0; p < pageCount; p ++) {
            if (memoryList[p] != -1) {
                timeRecord[p] ++;
            }
        }

        //是否需要置换
        for (int j = 0; j < phyNum; j ++) {
            if (memoryList[j] == strList[i]) {
                //该页面已经存在内存中
                //修改访问情况
                isVisited = 1;
                //重置访问时间
                timeRecord[j] = -1;
                //展示
                printf("%d\n",strList[i]);
                break;
            }
            if (memoryList[j] == -1) {
                //页面不在内存中且内存未满->直接存入
                memoryList[j] = strList[i];
                pageCount ++;
                //修改访问情况
                isVisited = 1;
                //修改访问时间
                timeRecord[j] ++;

                missingCount ++;
                //展示
                printf("%d\n",strList[i]);
                showMemoryList(memoryList, phyNum);
                break;
            }
        }

        if (!isVisited) {
            //需要置换
            //1.遍历时间记录表，寻找最久未访问的页面所在的内存下标
            int max = 0;
            for (int k = 0; k < phyNum; k ++) {
                if (timeRecord[max] < timeRecord[k]) {
                    max = k;
                }
            }

            //2.将该位置的页面换出
            memoryList[max] = strList[i];
            timeRecord[max] = -1;

            missingCount ++;
            replaceCount ++;

            //展示
            printf("%d\n",strList[i]);
            showMemoryList(memoryList, phyNum);

        }
    }
    informationCount(missingCount, replaceCount, pageNum);
}

//最近最久未使用置换算法-2
//void replacePageByLRU(int memoryList[],int phyNum,int strList[],int pageNum){
//
//    int isVisited = 0;
//
//    //记录已经在内存中的页面数量
//    int pageCount = 0;
//    for (int i = 0; i < pageNum; i ++) {
//        isVisited = 0;
//        //判断内存是否需要换页
//        for (int j = 0; j < phyNum; j ++) {
//            if (memoryList[j] == strList[i]) {
//                //已经存在于内存中，把它换到队列尾部
//                int temp = memoryList[j];
//                for (int k = j; k < pageCount-1; k ++) {
//                    memoryList[k] = memoryList[k+1];
//                }
//                memoryList[pageCount-1] = temp;
//
//                //修改访问情况
//                isVisited = 1;
//                //修改访问时间
//                //展示
//                printf("%d\n",strList[i]);
//                break;
//            }
//
//            if (memoryList[j] == -1) {
//                //页面不在内存中且内存未满->直接存入
//                memoryList[j] = strList[i];
//                //修改访问情况
//                isVisited = 1;
//
//                pageCount ++;
//
//                //展示
//                printf("%d\n",strList[i]);
//                showMemoryList(memoryList, phyNum);
//                break;
//            }
//        }
//
//        if (!isVisited) {
//            //需要换页
//            //1.直接将数组整体往前移一位
//            for (int k = 0; k < phyNum; k ++) {
//                memoryList[k] = memoryList[k+1];
//            }
//            //2.将当前页面加到队尾
//            memoryList[phyNum-1] = strList[i];
//            //展示
//            printf("%d\n",strList[i]);
//            showMemoryList(memoryList, phyNum);
//        }
//
//
//    }
//}
int main(int argc, const char * argv[]) {

    //物理块的数量
    int phyBlockNum;
    setvbuf(stdout,NULL,_IONBF,0);
    printf("请输入物理块数量:\n");
    scanf("%d",&phyBlockNum);

    //生成内存队列
    int memoryList[phyBlockNum];
    //初始化内存状态
    initializeList(memoryList, phyBlockNum);
    //showMemoryList(memoryList,phyBlockNum);

    //页面数量
    int pageNum;
    printf("请输入要访问的页面总数:\n");
    scanf("%d",&pageNum);

    //保存页面号引用串
    int pageNumStrList[pageNum];
    printf("请输入要访问的页面号:\n");
    for (int i = 0; i < pageNum; i ++) {
        scanf("%d",&pageNumStrList[i]);
    }
    showList(pageNumStrList, pageNum);

    int chose;
    while (1) {
        printf("请选择所需的置换算法:\n");
        printf("1.OPT 2.FIFO 3.LRU 4.退出\n");
        scanf("%d",&chose);

        switch (chose) {
            case 1:
                showList(pageNumStrList, pageNum);
                replacePageByOPT(memoryList, phyBlockNum, pageNumStrList, pageNum);
                //重新初始化内存
                initializeList(memoryList, phyBlockNum);
                break;
            case 2:
                showList(pageNumStrList, pageNum);
                replacePageByFIFO(memoryList, phyBlockNum, pageNumStrList, pageNum);
                //重新初始化内存
                initializeList(memoryList , phyBlockNum);
                break;
            case 3:
                showList(pageNumStrList, pageNum);
                replacePageByLRU(memoryList, phyBlockNum, pageNumStrList, pageNum);
                //重新初始化内存
                initializeList(memoryList, phyBlockNum);
                break;
            default:
                return 0;
                break;
        }
    }

    return 0;
}
