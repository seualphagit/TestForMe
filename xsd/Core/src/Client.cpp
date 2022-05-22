#include <memory>
#include <zmq.hpp>

#include "Client.h"

namespace xsd
{

std::mutex Client::_mtx;
std::condition_variable Client::cond;

// std::mutex _mtx;
// std::condition_variable cond;
    
Client & Client::GetInstance()
{

    static Client instance;
    return instance;
}

Client::Client()
{    
    void * context = zmq_ctx_new();
    sender = zmq_socket(context, ZMQ_REQ);

    //std::thread th = std::thread(&Client::HandleMessage, this);
}

Client::~Client()
{

}

void  Client::Start()
{
    _run_flag = 1;
    _worker = std::thread(&Client::HandleMessage, this);
    _recv = std::thread(&Client::ReceivePackage, this);
}

void Client::Init(SystemCallbackPtr system_callback)
{
    this->Connect();
    if(system_callback == nullptr)
    {
        if(_system_callback == nullptr)
        {
            _system_callback = std::make_shared<ISystemCallback>();
        }
    }
    else
    {
        _system_callback = system_callback;
    }
}

void Client::Stop()
{
    _run_flag = 0;
    _worker.join();    
}

void Client::SetServiceCallback(const uint id, ServiceCallbackPtr callback)
{
    if(callback != nullptr)
    {
        _service_map[id] = callback;
    }
    else
    {
        auto iter = _service_map.find(id);
        if(iter == _service_map.end())
        {
            _service_map[id] = std::make_shared<IServiceCallback>();
        }
    }
}

RESULT  Client::Subscribe(const uint id)
{
    std::unique_lock<std::mutex> locker(_mtx);
    // _service_map[id] = std::move(callback);
    auto iter = _service_map.find(id);
    if(iter == _service_map.end())
    {
        return RESULT_NO_CALLBACK;
    }
    _subscribes.emplace(id);
    std::cout<<"Subscribe: "<<id<<std::endl;
    return RESULT_OK;
}

RESULT  Client::Triggle(const uint id)
{
    std::unique_lock<std::mutex> locker(_mtx);
    auto iter = _service_map.find(id);
    if(iter == _service_map.end())
    {
        return RESULT_NO_CALLBACK;
    }
    if(_triggles.count(id) == 0)
    {
        _triggles.emplace(id);
        std::cout<<"Triggle: "<<id<<std::endl;
        return RESULT_OK;
    }
    else
    {
        return RESULT_ALREADY_ON;
    }
}

void  Client::OnMessage(Package & pkg)
{
    std::unique_lock<std::mutex> locker(_mtx);
    _rev_queue.emplace_back(pkg);
    locker.unlock();
    cond.notify_all();
}

void Client::HandleMessage()
{
    while(_run_flag)
    {
        std::unique_lock<std::mutex> locker(_mtx);
        while(_rev_queue.empty())
        {
            cond.wait(locker);
        }
        std::cout<<"handle pkg, size:"<< _rev_queue.size()<<std::endl;
        auto pkg = CmdPackageInfo(_rev_queue.back());

        auto id = pkg.GetId();
        auto cmd = pkg.GetCmd();
        std::cout<<"handle pkg, size:"<< _rev_queue.size()<<"   id:"<<id<<"   cmd:"<<cmd<<std::endl;
        auto iter = _service_map.find(id);
        if(iter == _service_map.end())
        {
            std::cout<<"No service callback for "<<id<<std::endl;
            return;
        }
        
        if(_triggles.count(id))
        {          
            std::cout<<"Triggle service callback for "<<id<<std::endl;  
            switch (cmd)
            {
            case 0:
                iter->second->OnComplete(pkg);
                break;
            case 1:
                iter->second->OnError(pkg);            
            default:
                break;
            }   

            _triggles.erase(_triggles.find(id));                    ;
        }
        
        if(_subscribes.count(id))
        {
            switch (cmd)
            {
            case 1:
                iter->second->OnError(pkg);            
            default:
                break;
            }
        }
        
        _rev_queue.pop_back();
        locker.unlock();
    }
    std::cout<<"Exit HandleMessage"<<std::endl;
}

// void Client::UpdateTriggleCallback(ServiceCallbackPtr callback)
// {    
//     if(callback != nullptr)
//     {
//         _subscribe_service_map[id] = callback;
//     }
//     else
//     {
//         auto iter = _subscribe_service_map.find(id);
//         if(iter == _subscribe_service_map.end())
//         {
//             _subscribe_service_map[id] = std::make_share<IServiceCallback>(new IServiceCallback);
//         }
//     }
// }


void Client::Connect()
{
    int ret = zmq_connect(sender, "tcp://localhost:5555");
    assert(ret == 0);
    std::cout<<"Connect OK"<<std::endl;
}

void Client::ReceivePackage()
{
    // while (true)
    // {
    //     /* code */
    //     // wait for reply from server
    //     zmq::message_t reply{};
    //     socket.recv(reply, zmq::recv_flags::none);
    //     std::string s = reply.to_string();
    //     Package pkg;
    //     OnMessage(pkg);
    // }   
}


void Client::SendPackage(const Package & pkg)
{

}
}