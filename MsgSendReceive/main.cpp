//
//  main.cpp
//  ShareMemoryMessage
//
//  Created by Nero Zuo on 16/11/7.
//  Copyright © 2016年 Nero. All rights reserved.
//

#include <iostream>
#include "messagedata.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/msg.h>

void messageReceive();
void messageSend();

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
    messageReceive();
  }else {
    printf("I am the parent process, my process id is %d\n", getpid());
    count++;
    messageSend();
  }
  printf("统计结果： %d\n", count);
  return 0;
}

void messageReceive() {
    int running = 1;
    int msgid = -1;
    message_data data;
    long int msg_type = 0;
    
    msgid = msgget((key_t)5023, 0666 | IPC_CREAT);
    if (msgid == -1) {
        printf("msgeet failed with error\n");
        exit(EXIT_FAILURE);
    }
    
    while (running) {
        if(msgrcv(msgid, (void *)&data, TEXT_SIZE, msg_type, 0) == -1) {
            printf("msgrcv failed with error\n");
            exit(EXIT_FAILURE);
        }
        printf("You wrote: %s\n", data.text);
        if(strncmp(data.text, "end", 3) == 0) {
            running = 0;
        }
        sleep(1);
    }
    
    if(msgctl(msgid, IPC_RMID, 0) == -1) {
        printf("msgctl failed with error\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

void messageSend() {
    int running = 1;
    int msgid = -1;
    message_data data;
    char buffer[TEXT_SIZE];
    
    msgid = msgget((key_t)5023, 0666 | IPC_CREAT);
    if (msgid == -1) {
        printf("msgeet failed with error\n");
        exit(EXIT_FAILURE);
    }
    
    while (running) {
        printf("Enter some text: ");
        fgets(buffer, TEXT_SIZE, stdin);
        data.msg_type = 1;
        strncpy(data.text, buffer, TEXT_SIZE);
        
        if (msgsnd(msgid, (void *)&data, sizeof(data.text), IPC_NOWAIT) == -1) {
            printf("msgsnd failed with error\n");
            exit(EXIT_FAILURE);
        }
        
        if (strncmp(buffer, "end", 3) == 0) {
            running = 0;
        }
        sleep(1);
    }
    exit(EXIT_SUCCESS);
    
}


