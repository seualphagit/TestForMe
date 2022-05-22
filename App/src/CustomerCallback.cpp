#include "CustomerCallback.h"

using namespace xsd;

void CustomerCallback::OnComplete(const CmdPackageInfo & pkg)
{
        std::cout<<_str<<"  CustomerCallback::OnComplete\n";
}


void CustomerCallback::OnError(const CmdPackageInfo & pkg)
{
        std::cout<<_str<<"  CustomerCallback::OnError\n";
}
