#include <stdio.h>

#include <time.h>
#include<functional>
#include<memory>
#include<chrono>
#include<random>

#include "CustomerCallback.h"
#include "Client.h"
#include "time_callback.h"


using namespace xsd;

void test_package()
{
    Package pkg;
    

    for(int i = 0; i < (int)pkg.max_size(); i++)
    {
        pkg[i] = rand()%0xff;
        srand(time(0) + rand());
    }
    
    CmdPackageInfo pkg_info(pkg);

    Package pkg2;
    pkg_info.Pack(pkg2);

    CmdPackageInfo pkg_info2(pkg2);

    if(pkg_info == pkg_info2)
    {
       
    }
    else{
        std::cout<<"Package pack error\n";
    }
}

int main()
{
    
    // for(int i = 0; i < 1000; i ++)
    // {
    //     test_package();
    // }

    // CallbackPtr callbackA = std::make_shared<ICallback>("SertviceA");
    // CallbackPtr callbackB = std::make_shared<ICallback>("SertviceB");
    ServiceCallbackPtr callbackA = std::shared_ptr<IServiceCallback>(new CustomerCallback("SertviceA"));
    ServiceCallbackPtr callbackB = std::shared_ptr<IServiceCallback>(new CustomerCallback("SertviceB"));

    Client& instance = Client::GetInstance();
    instance.SetServiceCallback(0, callbackA);
    instance.SetServiceCallback(1, callbackB);

    //instance.Subscribe(0);
    //instance.Subscribe(1);

    instance.Start();

    instance.Triggle(0);
    instance.Triggle(1);


    printf("start\n");

    struct timeval tv;
    struct event ev;

    struct event_base *base = event_init();  //初始化libevent库      

    tv.tv_sec = 1;
    tv.tv_usec = 0;
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
