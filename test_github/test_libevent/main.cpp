#include <stdio.h>

#include <time.h>
#include<functional>
#include<memory>
#include<chrono>


#include "Client.h"

void timerover_cb(int fd, short event, void *arg)    //回调函数
{
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



int main()
{
        // CallbackPtr callbackA = std::make_shared<ICallback>("SertviceA");
        // CallbackPtr callbackB = std::make_shared<ICallback>("SertviceB");
        CallbackPtr callbackA = std::shared_ptr<ICallback>(new CustomerCallback("SertviceA"));
        CallbackPtr callbackB = std::shared_ptr<ICallback>(new CustomerCallback("SertviceB"));

        Client& instance = Client::GetInstance();

        //instance.Subscribe(0, callbackA);
        //instance.Subscribe(1, callbackB);

        instance.Start();

        instance.Triggle(0, callbackA);
        instance.Triggle(1, callbackB);


        printf("start\n");

        struct timeval tv;
        struct event ev;

        struct event_base *base = event_init();  //初始化libevent库      

        tv.tv_sec = 0;
        tv.tv_usec = 10000;
        struct event * pEv;
        printf("event_new\n");
        pEv = event_new(base, -1, EV_PERSIST, timerover_cb, NULL);
        printf("event_add\n");
        event_add(pEv, &tv);  //将event添加到events事件链表，注册事件
        
        event_base_set(base, &ev);
        event_set(&ev, -1, EV_PERSIST, timer_cb, NULL);  //初始化event结构中成员
        
        event_add(&ev, &tv);  //将event添加到events事件链表，注册事件


        event_base_dispatch(base);  //循环、分发事件
        printf("event_base_dispatch\n");

        

        return 0;
}
