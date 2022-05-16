#pragma once
#include "Package.h"

class Sever
{
public:
    void OnMessage(const Package & pkg);

    void SendMessage(const Package & pkg);
};