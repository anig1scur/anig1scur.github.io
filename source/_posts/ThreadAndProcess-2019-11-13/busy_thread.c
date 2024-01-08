/* busy_thread.c*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

#define gettidv1() syscall(__NR_gettid) // new form
#define gettidv2() syscall(SYS_gettid)  // traditional form

void *hello_fun(void *args) {
    printf("the pthread_1 id is %ld\n", pthread_self());
    printf("the thread_1's Pid is %d\n", getpid());
    printf("The LWPID/tid of thread_1 is: %ld\n", (long int)gettidv1());

  while(1){
    //busy wait
  }
  return NULL;
}

int main(int argc, char * argv[]){
  pthread_t thread[4];  //thread identifier
  int i;
  //create 4 threads
  for(i = 0 ; i < 4; i++){
    pthread_create(&thread[i], NULL, hello_fun, NULL);
  }
  //wait for all threads to finish
  for(i = 0 ; i < 4; i++){
    pthread_join(thread[i], NULL);
  }
  return 0;
}
