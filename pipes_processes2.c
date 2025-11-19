// pipes_processes2.c
// 2-way communication between parent and child using pipes.
// P1 -> P2: initial string
// P2: append "howard.edu", print Output, prompt for extra input,
//     append it, send back to parent.
// P1: append "gobison.org", print final Output.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main(void)
{ 
    int fd1[2];  // parent -> child
    int fd2[2];  // child -> parent
  
    char fixed_str[] = "howard.edu"; 
    char input_str[100]; 
    pid_t p; 
  
    if (pipe(fd1) == -1) { 
        fprintf(stderr, "Pipe Failed\n"); 
        return 1; 
    } 
    if (pipe(fd2) == -1) { 
        fprintf(stderr, "Pipe Failed\n"); 
        return 1; 
    } 
  
    printf("Input : ");
    fflush(stdout);
    scanf("%99s", input_str); 
  
    p = fork(); 
  
    if (p < 0) { 
        fprintf(stderr, "fork Failed\n"); 
        return 1; 
    } 
  
    // Parent process (P1)
    if (p > 0) { 
        // close unused ends
        close(fd1[0]);  // parent doesn't read from fd1
        close(fd2[1]);  // parent doesn't write to fd2
  
        // send initial string to child
        write(fd1[1], input_str, strlen(input_str) + 1);
        close(fd1[1]);  // done writing
  
        // wait for child's result
        char back_str[200];
        ssize_t n = read(fd2[0], back_str, sizeof(back_str) - 1);
        close(fd2[0]);

        if (n > 0) {
            back_str[n] = '\0';
            const char *fixed_str2 = "gobison.org";

            // append "gobison.org" if it fits
            if (strlen(back_str) + strlen(fixed_str2) + 1 < sizeof(back_str)) {
                strcat(back_str, fixed_str2);
            }

            printf("Output : %s\n", back_str);
        }
  
        // wait for child to exit
        wait(NULL); 
    } 
  
    // Child process (P2)
    else { 
        // close unused ends
        close(fd1[1]);  // child doesn't write to fd1
        close(fd2[0]);  // child doesn't read from fd2

        char concat_str[100];
        if (read(fd1[0], concat_str, sizeof(concat_str) - 1) <= 0) {
            close(fd1[0]);
            close(fd2[1]);
            exit(1);
        }
        close(fd1[0]);

        // ensure null-terminated
        concat_str[sizeof(concat_str) - 1] = '\0';

        // append "howard.edu"
        int k = (int)strlen(concat_str);
        for (int i = 0; i < (int)strlen(fixed_str) && k < (int)sizeof(concat_str) - 1; i++) {
            concat_str[k++] = fixed_str[i];
        }
        concat_str[k] = '\0';
  
        // first output (P2)
        printf("Output : %s\n", concat_str);

        // second input prompt (P2)
        printf("Input : ");
        fflush(stdout);
        char extra[100];
        scanf("%99s", extra);

        // append extra user input
        for (int i = 0; i < (int)strlen(extra) && k < (int)sizeof(concat_str) - 1; i++) {
            concat_str[k++] = extra[i];
        }
        concat_str[k] = '\0';  

        // send back to parent
        write(fd2[1], concat_str, strlen(concat_str) + 1);
        close(fd2[1]); 
  
        exit(0); 
    } 
}

