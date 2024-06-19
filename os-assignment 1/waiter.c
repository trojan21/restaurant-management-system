#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <string.h>
int x=1;

int checkorder(int arr[],int l,int cost[]){
    int sum=0;
    for(int i=0;i<l;i++){
        if(arr[i]>9 || arr[i]<0){
            sum=-1;
            break;
        }
        if(cost[arr[i]]==0){
            sum=-1;
            break;
        }
        sum+=cost[arr[i]];
    }
    return sum;
}
int main(){
    FILE *file;
    char line[100];
    file = fopen("menu.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }
    int cost[]={0,0,0,0,0,0,0,0,0,0,0};
    while (fgets(line, sizeof(line), file) != NULL) {
        int serial_number, price;
        if (sscanf(line, "%d. %*[^0-9]%d INR", &serial_number, &price) == 2) {
            cost[serial_number]=price;
        }
    }
    fclose(file);
    while(1) {
        printf("Enter waiter ID:");
        int a;
        scanf("%d",&a);
        int* shareTable;
        int idTable;
        key_t key = ftok("table.c",3);
        if(key==-1) {
            printf("error in generating key");
        }

        idTable=shmget(key+100*a,2048,0666);
        if(idTable==-1){
            printf("error in shmget(table)");
        }

        shareTable=shmat(idTable,NULL,0);
        if((void *)shareTable==(void *)-1){
            printf("error in shmat(table)");
        }
        int arr[100];
        int l=shareTable[0];
        if (l == -10) break;
        for(int i=1;i<l+1;i++) {
            arr[i-1]=shareTable[i];
        }

        int total=checkorder(arr,l,cost);
        if(total!=-1){
            printf("Bill Amount For table %d: %d INR",a,total);
            printf("\n");
            int* sharemanager;
            int idmanager;
            idmanager=shmget(2222,2048,0666);
            if(idmanager==-1){
                printf("error in shmget(manager)");
            }
            sharemanager=shmat(idmanager,NULL,0);
            if((void*) sharemanager==(void *)-1){
                printf("error in shmat(manager)");
            }
            sharemanager[0]=a;
            sharemanager[1]=total;
            sharemanager[2]=x;
            if(l!=-10){
                x++;
            }
            shareTable[0]=total;
        }
        else{
            shareTable[0]=total;
        }
        

    }
    
    return 0;
}
	

