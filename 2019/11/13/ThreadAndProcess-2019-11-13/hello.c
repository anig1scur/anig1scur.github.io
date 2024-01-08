
/* hello_pthread_bad.c*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>


void * hello_fun(){

  printf("Hello World!\n");

  return NULL;
}


int main(int argc, char * argv[]){

pthread_t thread;

pthread_create(&thread, NULL, hello_fun, NULL);
pthread_join(thread,NULL);

printf("random");
printf("random");
printf("random");
printf("random");
//sleep(1);
//hello_fun只是简单的print

return 0;
}