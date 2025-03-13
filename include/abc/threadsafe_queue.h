#ifndef ABC_INCLUDE_ABC_THREADSAFE_QUEUE
#define ABC_INCLUDE_ABC_THREADSAFE_QUEUE

#pragma once

#include "threadsafe_queue_decl.h"

namespace abc
{

template <typename T>
auto ThreadSafeQueue<T>::put(T const & value) -> void
{
    {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(value);
    }
    cv_.notify_one();
}

template <typename T>
auto ThreadSafeQueue<T>::get() const -> T
{
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return !queue_.empty(); });
    T item = queue_.front();
    queue_.pop();
    return item;
}

}

#endif
