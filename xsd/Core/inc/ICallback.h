#pragma once
#include<iostream>
#include<memory>
#include <event.h>

#include "CmdPackageInfo.h"

namespace xsd
{

void timer_cb(int fd, short event, void *arg);    //回调函数


class IServiceCallback
{
    public:
    virtual void OnComplete(const CmdPackageInfo & pkg){ std::cout<<"  IServiceCallback::OnComplete\n"; }

    virtual void OnError(const CmdPackageInfo & pkg){std::cout<<"  IServiceCallback::OnError\n";}
} ;


class ISystemCallback
{
    public:
    virtual void OnStart(){}

    virtual void OnInit(){}

    virtual void OnStop(){}
};


using ServiceCallbackPtr = std::shared_ptr<IServiceCallback>;
using SystemCallbackPtr = std::shared_ptr<ISystemCallback>;

}