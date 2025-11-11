// pipes_processes2.c  (Part 2)
// Runs: cat scores | grep <pattern> | sort
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <grep-pattern>\n", argv[0]);
    return 1;
  }

  char *cat_args[]  = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", argv[1], NULL};
  char *sort_args[] = {"sort", NULL};

  int p1[2];  // cat -> grep
  int p2[2];  // grep -> sort
  if (pipe(p1) == -1) { perror("pipe p1"); return 1; }
  if (pipe(p2) == -1) { perror("pipe p2"); return 1; }

  pid_t pid = fork();
  if (pid < 0) { perror("fork"); return 1; }

  if (pid == 0) {
    // ---- Child: will run grep; first fork grandchild for sort ----
    pid_t gpid = fork();
    if (gpid < 0) { perror("fork (grandchild)"); _exit(1); }

    if (gpid == 0) {
      // ===== Grandchild: exec "sort" =====
      close(p1[0]); close(p1[1]);   // not used here
      close(p2[1]);                 // read-only from p2
      if (dup2(p2[0], STDIN_FILENO) == -1) { perror("dup2 sort stdin"); _exit(1); }
      close(p2[0]);

      execvp("sort", sort_args);
      perror("execvp sort");
      _exit(1);
    } else {
      // ===== Child: exec "grep <pattern>" =====
      close(p1[1]);                 // read from p1
      if (dup2(p1[0], STDIN_FILENO) == -1) { perror("dup2 grep stdin"); _exit(1); }
      close(p1[0]);

      close(p2[0]);                 // write to p2
      if (dup2(p2[1], STDOUT_FILENO) == -1) { perror("dup2 grep stdout"); _exit(1); }
      close(p2[1]);

      execvp("grep", grep_args);
      perror("execvp grep");
      _exit(1);
    }
  } else {
    // ---- Parent: exec "cat scores" ----
    close(p1[0]);                     // write to p1
    if (dup2(p1[1], STDOUT_FILENO) == -1) { perror("dup2 cat stdout"); return 1; }
    close(p1[1]);

    // parent doesn’t use p2 directly
    close(p2[0]);
    close(p2[1]);

    execvp("cat", cat_args);
    perror("execvp cat");
    return 1;
  }
}
