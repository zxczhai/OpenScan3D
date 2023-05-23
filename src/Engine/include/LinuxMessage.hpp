#ifndef _LMESSAGE_H
#define _LMESSAGE_H
#define MSG_KEY 1314
#include <iostream>
struct congmsgbuf
{
    long mtype;  // 消息类型
    int data[2]; // 消息数据
};

int sendMessage(congmsgbuf msg);

int rcvMessage(congmsgbuf &msg);
#endif