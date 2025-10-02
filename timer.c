#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

volatile sig_atomic_t seconds_count = 0;
volatile sig_atomic_t quit = 0;
time_t start_time;

void on_alarm(int signum) {
  (void)signum;
  seconds_count++;
  alarm(1);
}

void on_int(int signum) {
  (void)signum;
  quit = 1;
}

int main(void) {
  start_time = time(NULL);
  signal(SIGALRM, on_alarm);
  signal(SIGINT, on_int);
  alarm(1);
  while (1) {
    if (quit) {
      time_t end_time = time(NULL);
      printf("Total alarms received: %d\n", (int)seconds_count);
      printf("Total wall time: %ld seconds\n", (long)(end_time - start_time));
      return 0;
    }
  }
}
