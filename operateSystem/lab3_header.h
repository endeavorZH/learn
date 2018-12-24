//
// Created by ubuntu on 18-12-18.
//

#ifndef LAB3_HEADER_H
#define LAB3_HEADER_H

#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 128
#define MAX_NUM 6

void P(int semid, int index){
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = -1;
    sem.sem_flg = 0;
    semop(semid, &sem, 1);
    return;
}

void V(int semid, int index){
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = 1;
    sem.sem_flg = 0;
    semop(semid, &sem, 1);
    return;
}

typedef struct shareBuf{
    char buf[MAX_LENGTH];
    int length;
    int over;
}shareBuf;

shareBuf *shBuf[MAX_NUM];

#endif //LAB3_HEADER_H
