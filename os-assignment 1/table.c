#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_MENU_ITEMS 10


int main()
{

    int table_no;
    int fd[2];
    int orders[100];
    int arr_size;

    printf("Enter Table Number: ");
    scanf("%d", &table_no);
    
    while (1) {
        int num_customers;
        printf("Enter Number of Customers at Table (maximum no. of customers can be 5): ");
        scanf("%d", &num_customers);
        int x = 0;
        int c = 0;

        if (num_customers != -1) {
            FILE *file;
            char ch;
            //read the contents of the file menu.txt
            file = fopen("menu.txt", "r");

            if (file == NULL)
            {
                printf("Error opening the file.\n");
                return 1; 
            }

            while ((ch = fgetc(file)) != EOF)
            {
                printf("%c", ch);
            }
            printf("\n");

            fclose(file);
        }
        
        int flag = 1;
        while(flag) {
            c = 0;
            for (int i = 0; i < num_customers; i++)
            {
                if (pipe(fd) == -1)
                {
                    perror("pipe failure");
                    exit(1);
                }

                pid_t pid = fork();
                if (pid == -1)
                {
                    perror("fork failure");
                    exit(1);
                }

                else if (pid == 0)
                {                 
                    close(fd[0]); 
                    int order_items[11]; 
                    int num_ordered_items = 0;
                    int item_id;
                    printf("Enter the serial number(s) of the item(s) to order from the menu. Enter -1 when done:");
                    while (1)
                    {
                        scanf("%d", &item_id);

                        if (item_id == -1)
                        {
                            break;
                        }

                        else if (item_id > 0)
                        {

                            order_items[num_ordered_items++ + 1] = item_id;
                        }

                        else
                        {
                            printf("Invalid item ID. Please enter a valid ID from the menu.\n");
                        }
                    }
                    order_items[0] = num_ordered_items;
                    
                    write(fd[1], order_items, (num_ordered_items+1) * sizeof(int) + 1);
                    exit(0);
                }

                else
                { 

                    wait(NULL);
                    int arr_size = 11;
                    close(fd[1]); 
                    int received_arr[arr_size];
                    
                    
                    read(fd[0], received_arr, arr_size * sizeof(int) + 1);
                    close(fd[0]); 
                    int received_size = received_arr[0];
                    for (int i = 1; i <= received_size; i++)
                        orders[c++] = received_arr[i];

                }
            }

            key_t key = ftok("table.c",3); 
            if (key == -1) {
                perror("Error in ftok");
                return 1;
            }

            int shm_id = shmget(key+100*table_no,2048, IPC_CREAT | 0666);
            if (shm_id == -1)
            {
                perror("shmget");
                return 1; 
            }

            int *shared_memory = (int *)shmat(shm_id, NULL, 0);
            
            if ((void *)shared_memory == (void *)-1)
            {
                perror("shmat");
                return 1; 
            }

            if (num_customers == -1) {
                shared_memory[0] = -10;
                sleep(10);
                shmdt(shared_memory);
                shmctl(shm_id, IPC_RMID, NULL);
                break;
            }

            shared_memory[0] = c;
            for (int i = 1; i < c+1; i++)
            {
                shared_memory[i] = orders[i-1];
            }
            shared_memory = (int *)shmat(shm_id, NULL, 0);
            while (shared_memory[0] == c) {
                sleep(1);
            }
            
            if (shared_memory[0] != -1) {
                flag = 0;
                printf("The total bill amount is %d INR", shared_memory[0]);
            }
            printf("\n");

        }

        if (num_customers == -1) break;
    }
}

	

