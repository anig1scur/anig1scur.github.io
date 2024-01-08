/* IC221 Fork Lab
 * timer.c : program to execute a command and compute the running time
 * Anig
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>

#define MAX_ARGS 128

/* Subtract the â€˜struct timevalâ€™ values X and Y,
 * storing the result in RESULT.
 * Return 1 if the difference is negative, otherwise 0.
 * source: https://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html
 */
int timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y)
{
  /* Perform the carry for the later subtraction by updating y. */
  if (x->tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000) {
    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }
  /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_usec = x->tv_usec - y->tv_usec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}

int main(int argc, char * argv[]) {
  int status;
  int counter; 
  pid_t c_pid, pid;
  struct timeval start, end, diff;
  gettimeofday(&start, NULL);
    c_pid = fork(); 
  if (c_pid == 0){
    /* CHILD */
    printf("Child: executing\n");
    // execute
    execvp(argv[1], argv);    
    //only get here if exec failed     
    perror("execve failed");
  }else if (c_pid > 0){
    /* PARENT */
    if( (pid = wait(&status)) < 0){
      perror("wait");
      _exit(1);
    }
    /*count time in parent!*/
    gettimeofday(&end, NULL);
    timeval_subtract(&diff, &end, &start);
    printf("Run Time: %ld.%04ld (s)\n", diff.tv_sec, diff.tv_usec / 1000);
    printf("Parent: finished\n");

  }else{
    perror("fork failed");
    _exit(1);
  }
  return 0;
}
