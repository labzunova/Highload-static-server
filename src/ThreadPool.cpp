#include "../include/ThreadPool.h"

#include "iostream"

ThreadPool::ThreadPool(int threadCount) {
    _handler = Handler();
    _threadCount = threadCount;
    for(int i = 0; i < _threadCount; i++) {
        std::thread thr(&ThreadPool::Run, this);
        _threads.emplace_back(std::move(thr));
//        _threads.emplace_back(&ThreadPool::Run, this); // todo
    }
}

ThreadPool::~ThreadPool() {
    // todo удалить треды? да? нет?
}

void ThreadPool::PushTask(int socket, std::string& request) {
    _mx.lock(); //todo
    _queue.push(Request{socket, request});
    _mx.unlock();
}

void ThreadPool::Run() {
    while(true) {
        _mx.lock(); //todo
        if (_queue.empty()) {
            return;
        }
        Request task = _queue.front();
        _queue.pop();
        _handler.Handle(task.request, task.socket);
        _mx.unlock();
    }
}
