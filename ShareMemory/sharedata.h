//
//  sharedata.h
//  ShareMemory
//
//  Created by Nero Zuo on 16/10/18.
//  Copyright © 2016年 Nero. All rights reserved.
//

#ifndef sharedata_h
#define sharedata_h

#define TEXT_SIZE 1024

typedef struct shared_data {
  // flag 1: can read   0: can write
  int written;
  char text[TEXT_SIZE];
}shared_data;

#endif /* sharedata_h */

