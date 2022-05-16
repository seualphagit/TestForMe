#include "Client.h"
#include <zmq.hpp>

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

void  Client::Start(CallbakcFunc start_callback=default_callback)
{
    _run_flag = 1;
    _worker = std::thread(&Client::HandleMessage, this);
    _recv = std::thread(&Client::ReceivePackage, this);
    _start_callback = start_callback;
}

void Client::Init(CallbakcFunc init_callback=default_callback)
{
    this->Connect();
    _init_callback = init_callback;
}

void Client::Stop(CallbakcFunc stop_callback=default_callback)
{
    _worker.join();
    _run_flag = 0;
    _stop_callback = stop_callback;
}

void  Client::Subscribe(const unsigned long id, CallbackPtr callback)
{
    std::unique_lock<std::mutex> locker(_mtx);
    _subscribe_service_map[id] = std::move(callback);
    std::cout<<"Subscribe: "<<id<<std::endl;
}

void  Client::Triggle(const unsigned long id, CallbackPtr callback)
{
    std::unique_lock<std::mutex> locker(_mtx);
    _triggle_service_map[id] = std::move(callback);
    std::cout<<"Triggle: "<<id<<std::endl;
}

void  Client::OnMessage(const Package & pkg)
{
    std::unique_lock<std::mutex> locker(_mtx);
    _rev_queue.emplace_back(pkg);
    std::cout<<"push pkg, id:"<<pkg.id<<", cmd:"<<pkg.cmd<<", size:"<< _rev_queue.size()<<std::endl;
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
        Package pkg = _rev_queue.back();
        auto triggle_iter = _triggle_service_map.find(pkg.id);
        bool triggle_flag = false;
        bool subscribe_flag = false;
        if(triggle_iter != _triggle_service_map.end())
        {
            triggle_flag = true;
            switch (pkg.cmd)
            {
            case 0:
                triggle_iter->second->OnComplete(pkg);
                break;
            case 1:
                triggle_iter->second->OnError(pkg);            
            default:
                break;
            }                       
        }
        
        auto iter = _subscribe_service_map.find(pkg.id);
        if(iter != _subscribe_service_map.end())
        {
            subscribe_flag = true;
            switch (pkg.cmd)
            {
            case 1:
                iter->second->OnError(pkg);            
            default:
                break;
            }
        }
        if(triggle_flag)
        {
            if(!subscribe_flag)
            {
                _subscribe_service_map[triggle_iter->first] = std::move(triggle_iter->second);
            }
            
            _triggle_service_map.erase(triggle_iter); 
        }

        
        _rev_queue.pop_back();
        locker.unlock();
    }
    std::cout<<"Exit HandleMessage"<<std::endl;
}

void Client::default_callback(int result)
{
    std::cout<<"default callback result:"<<result<<std::endl;
}

void Client::Connect()
{
    int ret = zmq_connect(sender, "tcp://localhost:5555");
    assert(ret == 0);
    std::cout<<"Connect OK"<<std::endl;
}

void Client::ReceivePackage()
{
    while (true)
    {
        /* code */
        // wait for reply from server
        zmq::message_t reply{};
        socket.recv(reply, zmq::recv_flags::none);
        std::string s = reply.to_string();
        Package pkg;
        OnMessage(pkg);
    }   
}


void Client::SendPackage(const Package & pkg)
{

}