#include <stdio.h>
#include <event.h>
#include <time.h>
#include<chrono>
#include "Client.h"
#include "Callback.h"

void timer_cb(int fd, short event, void *arg)    //回调函数
{
        // tv.tv_sec = 1;
        // tv.tv_usec = 0;
        printf("timer_cb\n");
        //event_add(&ev, &tv);    //重新注册
        Package pkg;
        srand(time(0));
        pkg.id = rand()%2;
        pkg.cmd = rand()%2;
        auto now = std::chrono::system_clock::now();
        auto tt = std::chrono::system_clock::to_time_t(now);
        pkg.content = ctime(&tt);
        std::cout<<pkg.content<<std::endl;
        Client::GetInstance().OnMessage(pkg);
}

void CustomerCallback::OnComplete(const Package & pkg)
{
        std::cout<<_str<<"  CustomerCallback::OnComplete\n";
}


void CustomerCallback::OnError(const Package & pkg)
{
        std::cout<<_str<<"  CustomerCallback::OnError\n";
}

