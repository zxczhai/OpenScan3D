#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "global.h"
#define MSG_KEY 1314

struct congmsgbuf
{
    long mtype;  // 消息类型
    int data[2]; // 消息数据
};

int sendMessage(congmsgbuf msg)
{
    // 创建消息队列
    int msgid = msgget(MSG_KEY, IPC_CREAT | 0666);
    if (msgid == -1)
    {
        std::cerr << "Failed to create message queue!" << std::endl;
        return 1;
    }

    // 发送消息
    int ret = msgsnd(msgid, &msg, sizeof(msg.data), 0);
    if (ret == -1)
    {
        std::cerr << "Failed to send message!" << std::endl;
        return 1;
    }
    // 删除消息队列
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}

int rcvMessage(congmsgbuf msg)
{
    // 连接到消息队列
    int msgid = msgget(MSG_KEY, IPC_CREAT | 0666);
    if (msgid == -1)
    {
        std::cerr << "Failed to connect to message queue!" << std::endl;
        return 1;
    }

    // 接收消息
    int ret = msgrcv(msgid, &msg, sizeof(msg.data), 1, 0);
    if (ret == -1)
    {
        std::cerr << "Failed to receive message!" << std::endl;
        return 1;
    }
    return -1;
}