#include "AutoReconstruction.hpp"
#include <iostream>
#include "message.hpp"

int main()
{
    ::system("clear");
    printf("\n-----------------------------------\n");
    printf("        欢迎使用OS3DEngine         \n");
    printf("            程序初始化成功             \n");
    printf("        请使用OS3DGUI程序发起指令       \n");
    printf("     @Basic All rights reserved    \n");
    printf("-----------------------------------\n");

    Global::processProject = 0;
    Global::processState = 0;
    Global::process = PROCESSWORKING;
    congmsgbuf mymsg;
    mymsg.mtype = 1;

    // MsgProc(CMD_SFMANDSFP);

    // MsgProc(CMD_EXPORTDENSECLOUD);

    // MsgProc(CMD_RECONSTRUCTMESH);

    // MsgProc(CMD_TEXTUREMESH);

    MsgProc(CMD_FULLAUTO);

    // while (1)
    // {
    //     int ret = rcvMessage(mymsg);
    //     if (0 == ret)
    //     {
    //         MsgProc(mymsg.data[0]);
    //     }
    // }
    return 0;
}