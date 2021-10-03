#include "../include/ThreadPool.h"

#include <utility>

#include "iostream"

ThreadPool::ThreadPool(int threadCount, std::string doc_root) {
    _handler = Handler(std::move(doc_root));
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
    std::lock_guard<std::mutex> guard(_mx);
    _queue.push(Request{socket, std::move(request)});
    _takeTask.notify_one();
}

void ThreadPool::Run() {
    while(true) {
        Request task;
        {
            std::unique_lock<std::mutex> lock(_mx);

            _takeTask.wait(lock, [this]{
                return (!_queue.empty()) ;
            });
            task = _queue.front();
            _queue.pop();
        }

        _handler.Handle(task.request, task.socket);
    }
}
