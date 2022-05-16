#pragma once
#include<iostream>
#include<memory>
#include <event.h>

#include "Package.h"



void timer_cb(int fd, short event, void *arg);    //回调函数


class ICallback
{
    public:
    virtual void OnComplete(const Package & pkg){}

    virtual void OnError(const Package & pkg){}
} ;


class CustomerCallback: public ICallback
{
public:
    CustomerCallback(const std::string str="default"):_str(str)
    {

    }

    void OnComplete(const Package & pkg) override;

    void OnError(const Package & pkg) override;

protected:
    std::string _str;
};


using CallbackPtr = std::shared_ptr<ICallback>;