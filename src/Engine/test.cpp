#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_KEY 1201

struct mymsgbuf {
    long mtype;       // 消息类型
    int data[1];      // 消息数据
};

int main() {
    // 创建消息队列
    int msgid = msgget(MSG_KEY, IPC_CREAT | 0222);
    if (msgid == -1) {
        std::cerr << "Failed to create message queue!" << std::endl;
        return 1;
    }

    // 发送消息
    mymsgbuf msg;
    msg.mtype = 1;    // 消息类型为1
    msg.data[0] = 100;

    int ret = msgsnd(msgid, &msg, sizeof(msg.data), 0);
    if (ret == -1) {
        std::cerr << "Failed to send message!" << std::endl;
        return 1;
    }

    std::cout << "Message sent: " << msg.data[0] << std::endl;

    // 删除消息队列
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
