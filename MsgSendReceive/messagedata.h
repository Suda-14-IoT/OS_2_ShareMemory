//
//  messagedata.h
//  ShareMemoryMessage
//
//  Created by Nero Zuo on 16/11/7.
//  Copyright © 2016年 Nero. All rights reserved.
//

#ifndef messagedata_h
#define messagedata_h

#define TEXT_SIZE 1024

typedef struct message_data {
  long int msg_type;
  char text[TEXT_SIZE];
}message_data;

#endif /* messagedata_h */
