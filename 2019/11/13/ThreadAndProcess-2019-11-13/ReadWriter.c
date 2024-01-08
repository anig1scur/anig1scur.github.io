
/*
Using Locks and CVs solution1
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
using namespace std;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t OkToRead = PTHREAD_COND_INITIALIZER;
pthread_cond_t OkToWrite = PTHREAD_COND_INITIALIZER;

void *Read(void *arg);
void *Write(void *arg);
void ReadWrite(char *file);

#define MAX_THREAD_NUM 10
#define READ 'R'
#define WRITE 'W'

static int AR=0,AW=0,WR=0,WW=0;

typedef struct ThreadInfo
{
	int    serial;
	char   entity;
	double delay;
	double persist;
}ThreadInfo;

int main() {

  int n_thread=0 ;
  pthread_t threads[MAX_THREAD_NUM];
  ThreadInfo thread_info[MAX_THREAD_NUM];
  int i;
  ifstream in("text.txt", ios::in);
  if (!in.is_open()) {
    cout << "error" << endl;
    exit(1);
  }
  while (!in.eof()) {
 in>>thread_info[n_thread].serial>> thread_info[n_thread].entity>>thread_info[n_thread].delay>>thread_info[n_thread].persist;
n_thread++;
    }
    cout<<n_thread<<std::endl;
        for (i = 0; i < n_thread; i++) {
          if (thread_info[i].entity == READ) {
            pthread_create(&threads[i], NULL, &Read, &thread_info[i]);
      printf("%d %c %f %f\n",thread_info[i].serial,thread_info[i].entity,thread_info[i].delay,thread_info[i].persist);
        
            }
	
	   	else if(thread_info[i].entity == WRITE){
              pthread_create(&threads[i], NULL, &Write, &thread_info[i]);
               printf("%d %c %f %f\n",thread_info[i].serial,thread_info[i].entity,thread_info[i].delay,thread_info[i].persist);
                }
			else
			{
				puts("Bad File\n");
				exit(0);
                }
        }
            for (i = 0; i < n_thread; i++) {
                    pthread_join(threads[i], NULL);
            }
        
  return 0;
}

void *Read(void *arg) {
  int  count = 0;
  ThreadInfo *threadInfo = (ThreadInfo *)(arg);

  while (1) {
      sleep(threadInfo->delay);
    pthread_mutex_lock(&mutex);
    while (AW + WW > 0) {
      WR++;
      pthread_cond_wait(&OkToRead, &mutex);
      WR--;
    }
    AR++;
    pthread_mutex_unlock(&mutex);
    printf("Entity %d  starts reading\n", threadInfo->serial);

    if (AR == 1)
      printf("Now 1 Reader is reading\n");
    if(AR>1)
    printf("Now %d Readers are reading\n",AR);
    sleep(threadInfo->persist); // read...
    printf("Entity %d read over\n",threadInfo->serial);
  pthread_mutex_lock(&mutex);
AR--;
if (AR == 0 && WW >= 0)
  pthread_cond_signal(&OkToWrite);
pthread_mutex_unlock(&mutex);
count++;
  
}
}
void *Write(void *arg) {
  int  count = 0;
  ThreadInfo *threadInfo = (ThreadInfo *)(arg);
  while (1) {
      sleep(threadInfo->delay);
    pthread_mutex_lock(&mutex);
    while (AW + AR > 0) {
      WW++;
      pthread_cond_wait(&OkToWrite, &mutex);
      WW--;
      }
      AW++;
      pthread_mutex_unlock(&mutex);
      printf("Entity %d  starts writing\n", threadInfo->serial);
      sleep(threadInfo->persist); // write...
     
       printf("Entity %d wrote over\n",threadInfo->serial);
       pthread_mutex_lock(&mutex);
       AW--;
       if (WW > 0) {
        pthread_cond_signal(&OkToWrite);
        }
        else
        {
          pthread_cond_broadcast(&OkToRead);
        }
      pthread_mutex_unlock(&mutex);
    }
  }
