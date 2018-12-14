#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

int fd[2];
int pid1,pid2;

void pidKill(int signum){
    kill(pid1, SIGUSR1);
    kill(pid2, SIGUSR2);
}

void pid1Kill(int signum){
    if(signum == SIGUSR1){
        close(fd[0]);
        close(fd[1]);
        printf("Child Prcoess 1 is Killed by Parent!\n");
        exit(0);
    }
}

void pid2Kill(int signum){
    if(signum == SIGUSR2){
        close(fd[0]);
        close(fd[1]);
        printf("Child Prcoess 2 is Killed by Parent!\n");
        exit(0);
    }
}

int main(){
    pipe(fd);   //创建1个无名管
    char  outPipe[100];   //写入管道的内容
    int x=0;  //计数器

    signal(SIGINT, pidKill);

    signal(SIGALRM, pidKill);
    alarm(6);//判断时间

    if((pid1 = fork()) <0){
        exit(-1);
    }
    if(pid1==0){//进程1
        signal(SIGINT, SIG_IGN);   //设置忽略信号
        signal(SIGUSR1, pid1Kill);  //设置信号
        while(1){
            close(fd[0]);
            sprintf(outPipe, "I send you %d time\n", ++x);
            write(fd[1], outPipe, strlen(outPipe));
            sleep(1);
        }
    }

    if((pid2 = fork()) <0){
        exit(-1);
    }
    if(pid2 ==0){//进程2
        signal(SIGINT, SIG_IGN);   //设置忽略信号
        signal(SIGUSR2, pid2Kill);  //设置信号
        while(1){
            int n;
            close(fd[1]);
            read(fd[0], outPipe, 100);
            printf("%s",outPipe);
        }
    }
    //主进程
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    close(fd[0]);
    close(fd[1]);
    printf("Parent Process is killed\n" );
    
}