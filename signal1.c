#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t got_alarm = 0;

void handler(int signum) {
  (void)signum;
  printf("Hello World!\n");
  got_alarm = 1;
}

int main(void) {
  signal(SIGALRM, handler);
  alarm(5);
  while (1) {
    if (got_alarm) {
      printf("Turing was right!\n");
      return 0;
    }
  }
}
