#ifndef _CMESSAGE_H
#define _CMESSAGE_H
#include <iostream>

#define MSG_KEY 1314

struct congmsgbuf
{
    long mtype;  // 消息类型
    int data[2]; // 消息数据
};

int sendMessage(congmsgbuf msg);

int rcvMessage(congmsgbuf &msg);
#endif