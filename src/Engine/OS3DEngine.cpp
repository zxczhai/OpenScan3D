#include "AutoReconstruction.hpp"
#include <iostream>
#include "LinuxMessage.hpp"
int main()
{

    if (!savePid())
    {
        std::cerr << "Can't Write Property, Please Use Root Try Again\n";
        return -1;
    }
    ::system("clear");
    // hide user input
    ::system("stty -echo");
    // hide cursor
    std::cout << "\033[?25l";
    printf("\n-----------------------------------\n");
    printf("        欢迎使用OS3DEngine         \n");
    printf("            程序初始化成功             \n");
    printf("        请使用OpenScan3D程序发起指令       \n");
    printf("     @Basic All Rights Reserved    \n");
    printf("-----------------------------------\n");
    checkGPUExist();
    Global::processProject = 0;
    Global::processState = 0;
    Global::process = PROCESSWORKING;
    congmsgbuf mymsg;
    mymsg.mtype = 1;

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