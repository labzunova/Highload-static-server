#ifndef HIGHLOAD_STATIC_SERVER_THREADPOOL_H
#define HIGHLOAD_STATIC_SERVER_THREADPOOL_H

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include "Handler.h"
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(int threadCount, std::string doc_root);
    ~ThreadPool();
    void PushTask(int socket);
private:
    std::vector<std::thread> _threads;
    std::queue<int> _queue;
    std::mutex _mx;
    std::condition_variable _takeTask;
    int _threadCount;
    Handler _handler;
    void Run();
};

#endif //HIGHLOAD_STATIC_SERVER_THREADPOOL_H
