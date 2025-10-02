#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

static void child_process(void) {
  pid_t pid = getpid();
  pid_t parent_pid = getppid();

  srandom((unsigned)(time(NULL) ^ pid));

  int n = 1 + (int)(random() % 30);

  for (int i = 0; i < n; i++) {
    int t = 1 + (int)(random() % 10);
    
    printf("Child Pid: %d is going to sleep!\n" , (int)pid);
    fflush(stdout);

    sleep((unsigned)t);

    parent_pid = getppid();
    printf("Child Pid: %d is awake! \n Where is my parent: %d?\n",
    (int)pid, (int)parent_pid);

    fflush(stdout);
  }
  exit(0);
}

// parent process
int main (void){
  for (int i = 0; i < 2; i++) {
    pid_t pid = fork();
    if (pid < 0) {
      perror("fork");
      exit(1);
    }
    if (pid == 0) {
      child_process();
    }
  }

  for (int i = 0; i < 2; i++){
    int status = 0;
    pid_t done = wait(&status);
    if (done == -1) {
      perror("wait");
      exit(1);
    }

  
    if (WIFEXITED(status)) {
        printf("Child Pid: %d has completed with exit status: %d\n",
               (int)done, WEXITSTATUS(status));
      } else {
        printf("Child Pid: %d terminated abnormally (status=%d)\n",
               (int)done, status);
      }
      fflush(stdout);
  }

  return 0;
}


  //for (i=0; i < 2; i++){
   // int status;
   // int completed_pid = wait(&status);
   // printf("Child Pid: %d has completed with exit status: %d\n", completed_pid, status);
  //}
//}

// run one child process
//void child_process(){
 // int pid = getpid();
  //int parent_pid = getppid();
 //srandom(pid % 42); // seeding random number
 // int n = 1 + (random() % 30); // random number no more than 30
  //int i;
  //for (i=0; i < 2; i++){
   // int time = 1 + (random() % 10); // sleep time no more than 10 seconds
    //printf("Child Pid: %d is going to sleep for %d seconds!\n", pid, time);
   //sleep(time); // sleep
   // printf("Child Pid: %d is awake!\nWhere is my Parent: %d?\n", pid, parent_pid); // print if awake
  //}
  //exit(0);
//}


