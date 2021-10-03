#ifndef HIGHLOAD_STATIC_SERVER_THREADPOOL_H
#define HIGHLOAD_STATIC_SERVER_THREADPOOL_H

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include "Handler.h"
#include <condition_variable>

struct Request {
    int socket;
    std::string request;
};

class ThreadPool {
public:
    ThreadPool(int threadCount, std::string doc_root);
    ~ThreadPool();
    void PushTask(int socket, std::basic_string<char> request);
private:
    std::vector<std::thread> _threads;
    std::queue<Request> _queue;
    std::mutex _mx;
    std::condition_variable _takeTask;
    int _threadCount;
    Handler _handler;
    void Run();
};

#endif //HIGHLOAD_STATIC_SERVER_THREADPOOL_H
