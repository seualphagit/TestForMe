#pragma once
#include<chrono>
#include "Client.h"
#include "CmdPackageInfo.h"


namespace xsd{

void timerover_cb(int fd, short event, void *arg)    //回调函数
{
        printf("timerover_cb, init package\n");
        //event_add(&ev, &tv);    //重新注册
        CmdPackageInfo pkg_info;
        srand(time(0));
        pkg_info.SetCmd(rand()%2);
        pkg_info.SetId(rand()%2);

        auto now = std::chrono::system_clock::now();
        auto tt = std::chrono::system_clock::to_time_t(now);
        pkg_info.SetContent(ctime(&tt));
        pkg_info.Print();
        Package pkg;
        pkg_info.Pack(pkg);
        Client::GetInstance().OnMessage(pkg);
}
}