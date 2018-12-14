#include <pthread.h>
#include <sys/types.h>
#include <sys/sem.h>
#include<sys/ipc.h>
#include <iostream>
#include <stdlib.h>

int semID;
int a = 0;

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
 } arg;

//p操作
void P(int semid, int index){
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = -1;
    sem.sem_flg = 0;
    semop(semid, &sem, 1);
    return;
}

//v操作
void V(int semid, int index){
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = 1;
    sem.sem_flg = 0;
    semop(semid, &sem, 1);
    return;
}

void *subp1(void*){
    int i = 1;
    for (; i <= 100; i++){
        P(semID, 0);
        std::cout << "Thread 1 is adding!" << std::endl;
        a += i;
        V(semID, 1);
    }
    return 0;
}

void *subp2(void*){
    int i = 1;
    for(; i <= 100; i++){
        P(semID, 1);
        std::cout<< "Thread 2 is printing : " <<  a << std::endl;
        V(semID, 0);
    }
    return 0;
}

int main(){
    key_t key = 1;//为IPC_PRIVATE表明由系统选用一个关键字
    //创建信号灯
    semID = semget(key, 2, IPC_CREAT|0666);
    
    union semun arg1, arg2;
    arg1.val = 1;
    arg2.val = 0;

    semctl(semID, 0, SETVAL, arg1);
    semctl(semID, 1, SETVAL, arg2);

    //创建两个线程
    pthread_t p1, p2;
    int get1, get2;
    get1 = pthread_create(&p1, NULL, subp1, NULL);
    get2 = pthread_create(&p2, NULL, subp2, NULL);

    //等待两个线程
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    int myDelete;
    union semun delun;
    myDelete = semctl(semID, 2,IPC_RMID,delun); 

    return 0;
}