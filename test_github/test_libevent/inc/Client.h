#pragma once
#include<memory>
#include<thread>
#include<deque>
#include<mutex>
#include<unordered_map>
#include<condition_variable>
#include<atomic>
#include<functional>

#include "Package.h"
#include "Callback.h"

class Client
{
public:
    using CallbakcFunc = std::function<void(int)>;
    static Client & GetInstance();
    void Start(CallbakcFunc start_callback=default_callback);
    void Init(CallbakcFunc start_callback=default_callback);
    void Stop(CallbakcFunc stop_callback=default_callback);
    void Subscribe(const unsigned long id, CallbackPtr callback);
    void Triggle(const unsigned long id, CallbackPtr callback);

    //接收Message，继承于Com模块，将Message放到dequeue中
    void OnMessage(const Package & pkg);

    static void default_callback(int result = 0);

    void Connect();

    void SendPackage(const Package & pkg);

    //receive implement by COM
    void ReceivePackage();

protected:
    //thread处理dequeue中的Message
    void HandleMessage();
   

private:
    Client();
    ~Client();

    Client(const Client & client) = delete;
    Client& operator=(const Client & client) = delete;

    std::thread _worker;

    std::unordered_map<unsigned long, CallbackPtr> _subscribe_service_map;
    std::unordered_map<unsigned long, CallbackPtr> _triggle_service_map;
    std::deque<Package> _rev_queue;
    static std::mutex _mtx;
    static std::condition_variable cond;

    int _run_flag;  
    // std::atomic<int> _run_flag;  

    CallbakcFunc _start_callback;
    CallbakcFunc _init_callback;
    CallbakcFunc _stop_callback;

    //receive thread, implement by COM
    std::thread _recv;

    zmq::socket_t socket;
    void * sender;

};