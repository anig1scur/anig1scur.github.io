#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <pthread.h>

#define gettidv1() syscall(__NR_gettid) // new form
#define gettidv2() syscall(SYS_gettid)  // traditional form

void *ThreadFunc1()
{
        printf("the pthread_1 id is %ld\n", pthread_self());
        printf("the thread_1's Pid is %d\n", getpid());
        printf("The LWPID/tid of thread_1 is: %ld\n", (long int)gettidv2());
        pause();

        return 0;
}

void *ThreadFunc2()
{
        printf("the pthread_2 id is %ld\n", pthread_self());
        printf("the thread_2's Pid is %d\n", getpid());
        printf("The LWPID/tid of thread_2 is: %ld\n", (long int)gettidv2());
        pause();

        return 0;
}

int main(int argc, char *argv[])
{
        pid_t tid;
        pthread_t pthread_id;

        printf("the master thread's pthread id is %ld\n", pthread_self());
        printf("the master thread's Pid is %d\n", getpid());
        printf("The LWPID of master thread is: %ld\n", (long int)gettidv1());

        // 创建2个线程
        pthread_create(&pthread_id, NULL, ThreadFunc2, NULL);
        pthread_create(&pthread_id, NULL, ThreadFunc1, NULL);
        // pause();
        pthread_join(pthread_id,NULL);
        return 0;
}