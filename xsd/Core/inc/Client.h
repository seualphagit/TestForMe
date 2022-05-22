#pragma once
#include<memory>
#include<thread>
#include<deque>
#include<mutex>
#include<unordered_map>
#include<condition_variable>
#include<atomic>
#include<functional>
#include<set>

#include "CmdPackageInfo.h"
#include "ICallback.h"
#include "CommonDef.h"

namespace xsd
{

class Client
{
public:
    static Client & GetInstance();
    void Init(SystemCallbackPtr start_callback=nullptr);

    void Start();
    
    void Stop();

    void SetServiceCallback(const uint id, ServiceCallbackPtr callback);

    RESULT Subscribe(const uint id);

    RESULT Triggle(const uint id);

    //接收Message，继承于Com模块，将Message放到dequeue中
    void OnMessage(Package & pkg);

    static void default_callback(int result = 0);

    void Connect();

    void SendPackage(const Package & pkg);

    //receive implement by COM
    void ReceivePackage();

protected:
    //thread处理dequeue中的Message
    void HandleMessage();

    void UpdateSubscribeCallback(uint id, ServiceCallbackPtr callback);
   
    // void UpdateTriggleCallback(uint id, ServiceCallbackPtr callback);

private:
    Client();
    ~Client();

    Client(const Client & client) = delete;
    Client& operator=(const Client & client) = delete;

    std::thread _worker;

    std::unordered_map<unsigned long, ServiceCallbackPtr> _service_map;
    std::set<uint> _subscribes;
    std::set<uint> _triggles;

    // std::unordered_map<unsigned long, ServiceCallbackPtr> _triggle_service_map;
    std::deque<std::array<uchar,64>> _rev_queue;
    static std::mutex _mtx;
    static std::condition_variable cond;

    int _run_flag;  
    // std::atomic<int> _run_flag;  

    SystemCallbackPtr _system_callback = std::make_shared<ISystemCallback>();

    //receive thread, implement by COM
    std::thread _recv;

    // zmq::socket_t socket;
    void * sender;

};
    
}