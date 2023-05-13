#include "AutoReconstruction.hpp"
#include <iostream>

int main()
{
    ::system("clear");
    printf("\n-----------------------------------\n");
    printf("        欢迎使用OS3DEngine         \n");
    printf("            程序初始化成功             \n");
    printf("        请使用OS3DGUI程序发起指令       \n");
    printf("     @Basic All rights reserved    \n");
    printf("-----------------------------------\n");

    // Global::processProject = 0;
    // Global::processState = 0;
    // Global::process = PROCESSWORKING;
    MsgProc(CMD_MATCHFEATURES);
}