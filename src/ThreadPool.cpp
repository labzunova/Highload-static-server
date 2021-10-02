#include "../include/ThreadPool.h"

ThreadPool::ThreadPool(int threadCount) {
    _handler = Handler();
    for( int i = 0; i < threadCount; i++ ) {
        _threads.emplace_back(ThreadPool;::) // todo
    }
}

ThreadPool::~ThreadPool() {
    // todo удалить треды? да? нет?
}

void ThreadPool::PushTask(int socket, std::string request) {
    _mx.lock(); //todo
    _queue.push(Request{socket, request});
}

void ThreadPool::Run() {
    while(true) {
        _mx.lock(); //todo
        Request task = _queue.front();
        _queue.pop();
        _handler.Handle(task.request, task.socket);
    }
}
