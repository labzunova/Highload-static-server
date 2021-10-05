#include "../include/ThreadPool.h"

#include <utility>

#include "iostream"

ThreadPool::ThreadPool(int threadCount, std::string doc_root) {
    _handler = Handler(std::move(doc_root));
    _threadCount = threadCount;
    for(int i = 0; i < _threadCount; i++) {
        std::thread thr(&ThreadPool::Run, this);
        _threads.emplace_back(std::move(thr));
    }
}

ThreadPool::~ThreadPool() {
    // todo удалить треды? да? нет?
}

void ThreadPool::PushTask(int socket) {
    std::lock_guard<std::mutex> guard(_mx);
    _queue.push(socket);
    _takeTask.notify_one();
}

void ThreadPool::Run() {
    while(true) {
        int socketWithTask;
        {
            std::unique_lock<std::mutex> lock(_mx);

            _takeTask.wait(lock, [this]{
                return (!_queue.empty());
            });
            socketWithTask = _queue.front();
            _queue.pop();
        }

        _handler.Handle(socketWithTask);
    }
}
