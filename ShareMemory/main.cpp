//
//  main.cpp
//  ShareMemory
//
//  Created by Nero Zuo on 16/10/18.
//  Copyright © 2016年 Nero. All rights reserved.
//

#include <iostream>
#include "sharedata.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/shm.h>

void readProcess();
void writeProcess();

int main(int argc, const char * argv[]) {
    pid_t fpid;
    int count = 0;
    
    fpid = fork();
    if (fpid < 0) {
        printf("Error in fork!");
        exit(EXIT_FAILURE);
    }
    
    if (fpid == 0) {
        printf("I am the child proces, my process id is %d\n", getpid());
        count++;
        readProcess();
    }else {
        printf("I am the parent process, my process id is %d\n", getpid());
        count++;
        writeProcess();
    }
    printf("统计结果： %d\n", count);
    return 0;
}


void readProcess() {
    int running = 1;
    void *shareMemory = NULL;
    shared_data *sharedData;
    int shareMemoryId;
    
    shareMemoryId = shmget((key_t)7981, sizeof(shared_data), 0666|IPC_CREAT);
    
    if (-1 == shareMemoryId) {
        printf("Shmget failed\n");
        exit(EXIT_FAILURE);
    }
    
    shareMemory = shmat(shareMemoryId, 0, 0);
    if (shareMemory == (void *)-1) {
        printf("shmat failed\n");
        exit(EXIT_FAILURE);
    }
    
    //    printf("\nMEmory attached at %X\n", (int)shareMemory);
    
    sharedData = (shared_data *)shareMemory;
    
    while (running) {
        if (sharedData->written != 0) {
            printf("You wrote: %s", sharedData->text);
            sleep(rand()%3);
            sharedData->written = 0;
            // if input data's first three character is 'end'
            if (strncmp(sharedData->text, "end", 3) == 0) {
                running = 0;
            }
        }else {
            sleep(1);
        }
    }
    
    if (-1 == shmdt(shareMemory)) {
        printf("shmdt failed\n");
        exit(EXIT_FAILURE);
    }
    
    if (shmctl(shareMemoryId, IPC_RMID, 0) == -1) {
        printf("shmctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

void writeProcess() {
    int running = 1;
    void *shareMemory = NULL;
    shared_data *sharedData = NULL;
    char buffer[BUFSIZ + 1];
    int shareMemoryId;
    
    shareMemoryId = shmget((key_t)7981, sizeof(shared_data), 0666|IPC_CREAT);
    
    if (-1 == shareMemoryId) {
        printf("Shmget failed\n");
        exit(EXIT_FAILURE);
    }
    
    shareMemory = shmat(shareMemoryId, 0, 0);
    if (shareMemory == (void *)-1) {
        printf("shmat failed\n");
        exit(EXIT_FAILURE);
    }
    
    //    printf("\nMEmory attached at %X\n", (int)shareMemory);
    
    sharedData = (shared_data *)shareMemory;
    
    while (running) {
        
        // have not been read
        while (sharedData->written == 1) {
            sleep(1);
            printf("Waitting...\n");
        }
        
        printf("Enter some text: ");
        fgets(buffer, BUFSIZ, stdin);
        strncpy(sharedData->text, buffer, TEXT_SIZE);
        sharedData->written = 1;
        
        if (0 == strncmp(buffer, "end", 3) ) {
            running = 0;
        }
    }
    
    if (-1 == shmdt(shareMemory)) {
        printf("shmdt failed\n");
        exit(EXIT_FAILURE);
    }
    
    sleep(2);
    exit(EXIT_SUCCESS);
}
