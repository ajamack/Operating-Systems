/* hello_signal.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t tick = 0;

void handler(int signum) {
  (void)signum;
  printf("Hello World!\n");
  tick = 1;
  alarm(5);
}

int main(void) {


  signal(SIGALRM, handler);
  alarm(5);

  while (1) {
    if(tick) {
        printf("Turing was right!\n");
        tick = 0;

     }
   }

   return 0;
 }


