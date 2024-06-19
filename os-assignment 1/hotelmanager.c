#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_KEY 1234

int main() {
    FILE *file = fopen("earnings.txt", "w");
    fclose(file);
    int* close;
    int admin;
    int total_earnings = 0;
    int num_tables;
    printf("enter number of tables:");
    scanf("%d", &num_tables);
    int x= 1 ;

    admin=shmget(1234,2048,0666);
    if(admin==-1){
        printf("error in admin shmget");
    }
    close=shmat(admin,NULL,0);
    if((void *)close==(void *)-1){
        printf("error in shmat admin");
    }
   
    int idWaiter= shmget(2222,2048,0666|IPC_CREAT);
    if(idWaiter==-1)
    {
        printf("error");
    }
    int *sharewaiter = shmat(idWaiter, NULL, 0);
    if((void *)sharewaiter==(void *)-1)
    {
        printf("error");
    }
    int l=sharewaiter[0];
    int e=sharewaiter[1];
    int s=sharewaiter[2];
    while(l==sharewaiter[0] && e==sharewaiter[1] && s==sharewaiter[2]){
    sleep(1);
    }
   
    file = fopen("earnings.txt", "w");
    fclose(file);
    while(1){
    if(close[0]==1){
        break;
    }
    int idWaiter= shmget(2222,2048,0666);
    if(idWaiter==-1)
    {
        break;
    }
    int *sharewaiter = shmat(idWaiter, NULL, 0);
    if((void *)sharewaiter==(void *)-1)
    {
        break;
    }
    int earnings=sharewaiter[1];
    int a=sharewaiter[2];
    total_earnings+=sharewaiter[1];
   
    FILE *file = fopen("earnings.txt", "a");
    if (file == NULL) {
      printf("Error opening file!\n");
       exit(1);
    } else {

     fprintf(file, "Earning from Table %d: %d INR\n", sharewaiter[0], sharewaiter[1]);
    }
    while(sharewaiter[1]==earnings && sharewaiter[2]==a){
        if(close[0]){
            break;
        }
        sleep(1);
    }
}

int total_wages = total_earnings * 0.4;
int profit = total_earnings - total_wages;
printf("Total Earnings of Hotel: %d INR\n", total_earnings);
printf("Total Wages of Waiters: %d INR\n", total_wages);
printf("Total Profit of Hotel: %d INR\n", profit);
printf("Thank you for visiting the Hotel!\n");
file = fopen("earnings.txt", "a");
if (file == NULL) {
    printf("Error opening file!\n");
    exit(1);
}
else {

    fprintf(file, "Total Earnings of Hotel: %d INR\n",total_earnings);
    fprintf(file, "Total Wages of Waiters:  %d INR\n",total_wages);
    fprintf(file, "Total Profit of Hotel: %d INR\n",profit);
}
shmdt(sharewaiter);
shmctl(idWaiter, IPC_RMID, NULL);
return 0;
}
