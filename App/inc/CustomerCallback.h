#pragma once

#include "ICallback.h"

namespace xsd{

class CustomerCallback: public IServiceCallback
{
public:
    CustomerCallback(const std::string str="default"):_str(str)
    {

    }

    void OnComplete(const CmdPackageInfo & pkg) override;

    void OnError(const CmdPackageInfo & pkg) override;

protected:
    std::string _str;
};
}