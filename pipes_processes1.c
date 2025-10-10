// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main(void) 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
  
    char fixed_str[] = "howard.edu"; 
    char input_str[100]; 
    pid_t p; 
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    printf("Other string is: howard.edu\n");
    printf("Input : ");
    fflush(stdout);
    scanf("%99s", input_str); 
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
  
        close(fd1[0]);  // Close reading end of pipes 
        close(fd2[1]);
  
        // Write input string and close writing end of first 
        // pipe. 
        write(fd1[1], input_str, strlen(input_str)+1); 
        
  
        // Wait for child to print the concatenated string 
        char back_str[200];
        ssize_t n = read(fd2[0], back_str, sizeof(back_str)-1);
        if (n > 0) {
            back_str[n] = '\0';
            const char *fixed_str2 = "gobison.org";
            if (strlen(back_str) + strlen(fixed_str2) + 1 < sizeof(back_str)) {
                strcat(back_str, fixed_str2);
            }
            printf("Output : %s\n", back_str);
        }
  
        close(fd2[0]); // Close writing end of pipes 
        close(fd1[1]);

        wait(NULL); 
    } 
  
    // child process 
    else
    { 
        close(fd1[1]);  // Close writing end of first pipes 
        close(fd2[0]); 

        char concat_str[100];
        read(fd1[0], concat_str, 100);

        int k = strlen(concat_str);
        int i;
        for (i=0; i<strlen(fixed_str); i++) 
            concat_str[k++] = fixed_str[i];
  
        concat_str[k] = '\0';   // string ends with '\0' 
  
        printf("Output : %s\n", concat_str);

        printf("Input : ");
        char extra[100];
        scanf("%99s", extra);

        for (i = 0; i < strlen(extra) && k < (int)sizeof(concat_str) - 1; i++) // NEW
            concat_str[k++] = extra[i];                      // NEW
        concat_str[k] = '\0';  

        write(fd2[1], concat_str, strlen(concat_str)+1);
        // Close both reading ends 
        close(fd1[0]); 
        close(fd2[1]); 

  
        exit(0); 
    } 
} 
