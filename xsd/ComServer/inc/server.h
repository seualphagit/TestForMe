#pragma once
#include "CmdPackageInfo.h"

namespace xsd
{
class Server
{
public:
    void OnMessage(const Package & pkg);

    void SendMessage(const Package & pkg);
};
}