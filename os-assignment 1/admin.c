#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#define SHM_KEY 1234

int main() {
    int shmid = shmget((key_t)1234,2048, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("Error creating shared memory");
        exit(EXIT_FAILURE);
    }

    int* shareddata = shmat(shmid, NULL, 0);
    if (shareddata == (void *)-1) {
        perror("Error attaching shared memory");
        exit(EXIT_FAILURE);
    }

    shareddata[0]= 0;

    while (1) {
        char choice;
        printf("Do you want to close the hotel? Enter Y for Yes and N for No: ");
        scanf(" %c", &choice);

        if (choice == 'Y' || choice == 'y') {
            shareddata[0]= 1;
            break; 
        } else if (choice == 'N' || choice == 'n') {
        } else {
            printf("Invalid input. Please enter Y or N.\n");
        }
    }
    shmdt(shareddata);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
	

