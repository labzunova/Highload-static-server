#include "../include/ThreadPool.h"

#include <utility>

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

void ThreadPool::PushTask(int socket, std::basic_string<char> request) {
    std::cout << "gonna lock" << std::endl;
    std::lock_guard<std::mutex> guard(_mx);
//    _mx.lock(); //todo
    std::cout << "pushTask lock" << std::endl;
    _queue.push(Request{socket, std::move(request)});
//    _mx.unlock();
    _takeTask.notify_one();
    std::cout << "pushTask unlock" << std::endl;
}

void ThreadPool::Run() {
    while(true) {
//        _mx.lock(); //todo
        Request task;
        {
            std::cout << "run lock" << std::endl;
            std::unique_lock<std::mutex> lock(_mx);

            _takeTask.wait(lock, [this]{
                return (!_queue.empty()) ;
            });
            task = _queue.front();
            _queue.pop();
            std::cout << "pop task done" << std::endl;
        }

        _handler.Handle(task.request, task.socket);
//        _mx.unlock();
        std::cout << "handle task done" << std::endl;
    }
}
