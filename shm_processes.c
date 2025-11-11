#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <unistd.h>
#include  <sys/wait.h>
#include  <time.h>

int main(void)
{
    int    ShmID;
    int    *ShmPTR;
    pid_t  pid;
    int    status;

    ShmID = shmget(IPC_PRIVATE, 2 * sizeof(int), IPC_CREAT | 0666);
    if (ShmID < 0) {
        perror("*** shmget error (server) ***");
        exit(1);
    }
    printf("Server created shared memory for 2 ints (BankAccount, Turn)...\n");

    ShmPTR = (int *)shmat(ShmID, NULL, 0);
    if (ShmPTR == (void *)-1) {
        perror("*** shmat error (server) ***");
        exit(1);
    }
    printf("Server attached shared memory...\n");

    ShmPTR[0] = 0;  /* BankAccount */
    ShmPTR[1] = 0;  /* Turn: 0 = Dad, 1 = Student */
    printf("Initialized BankAccount=%d, Turn=%d\n", ShmPTR[0], ShmPTR[1]);

    printf("Server is about to fork a child process...\n");
    pid = fork();
    if (pid < 0) {
        perror("*** fork error (server) ***");
        shmdt((void *)ShmPTR);
        shmctl(ShmID, IPC_RMID, NULL);
        exit(1);
    } else if (pid == 0) {
        srandom((unsigned)(time(NULL) ^ getpid()));
        for (int i = 0; i < 25; i++) {
            sleep((unsigned)(random() % 6));
            while (ShmPTR[1] != 1) { }
            int account = ShmPTR[0];
            int need = (int)(random() % 51);
            printf("Poor Student needs $%d\n", need);
            if (need <= account) {
                account -= need;
                printf("Poor Student: Withdraws $%d / Balance = $%d\n", need, account);
            } else {
                printf("Poor Student: Not Enough Cash ($%d)\n", account);
            }
            ShmPTR[0] = account;
            ShmPTR[1] = 0;
        }
        shmdt((void *)ShmPTR);
        exit(0);
    }

    srandom((unsigned)(time(NULL) ^ getpid()));
    for (int i = 0; i < 25; i++) {
        sleep((unsigned)(random() % 6));
        while (ShmPTR[1] != 0) { }
        int account = ShmPTR[0];
        if (account <= 100) {
            int balance = (int)(random() % 101);
            if ((balance % 2) == 0) {
                account += balance;
                printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, account);
            } else {
                printf("Dear old Dad: Doesn't have any money to give\n");
            }
        } else {
            printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", account);
        }
        ShmPTR[0] = account;
        ShmPTR[1] = 1;
    }

    wait(&status);
    printf("Server detected child completion.\n");
    shmdt((void *)ShmPTR);
    shmctl(ShmID, IPC_RMID, NULL);
    printf("Server detached and removed shared memory. Exiting.\n");
    return 0;
}
