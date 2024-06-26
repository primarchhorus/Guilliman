#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cstdint>
#include <mutex>

template <typename T, uint32_t size>
struct circular_buffer
{
    circular_buffer() = default;
    ~circular_buffer() = default;

    circular_buffer(const circular_buffer &) = delete;
    circular_buffer &operator=(const circular_buffer &) = delete;

    circular_buffer(circular_buffer &&other) noexcept
        : buffer_size(other.buffer_size), tail(other.tail), head(other.head), full(other.full)
    {
        std::unique_lock<std::mutex> lock(other.buffer_lock);
        std::copy(std::begin(other.buffer), std::end(other.buffer), std::begin(buffer));
    }

    circular_buffer &operator=(circular_buffer &&other) noexcept
    {
        if (this != &other)
        {
            std::unique_lock<std::mutex> lock(buffer_lock, other.buffer_lock);
            buffer_size = other.buffer_size;
            tail = other.tail;
            head = other.head;
            full = other.full;
            std::copy(std::begin(other.buffer), std::end(other.buffer), std::begin(buffer));
        }
        return *this;
    }

    bool try_push_entity(T entity)
    {
        std::unique_lock<std::mutex> lock{buffer_lock, std::try_to_lock};
        if (full || !lock) {
            return false;
        }
        if(full)
        {
            tail = (tail + 1) % buffer_size;
        }
        head = (head + 1) % buffer_size;
        full = head == tail;
        buffer[head] = entity;
        ready.notify_one();
        return true;
    }

    bool try_pop_entity(T &out_entity)
    {   
        std::unique_lock<std::mutex> lock{buffer_lock, std::try_to_lock};
        if (empty() || !lock) {
            return false;
        }
        
        full = false;
	    tail = (tail + 1) % buffer_size;
        out_entity = buffer[tail];
        return true;
    }

    void push_entity(T entity)
    {
        std::unique_lock<std::mutex> lock(buffer_lock);

        if(full)
        {
            tail = (tail + 1) % buffer_size;
        }
        head = (head + 1) % buffer_size;
        full = head == tail;
        buffer[head] = entity;
        ready.notify_one();
    }

    bool pop_entity(T &out_entity)
    {
        std::unique_lock<std::mutex> lock(buffer_lock);
        ready.wait(lock, [&]() { return !empty(); });
        
        full = false;
	    tail = (tail + 1) % buffer_size;
        out_entity = buffer[tail];
        return true;
    }

    bool empty() const
    {
        return (!full && (head == tail));
    }

    void finish() {
        std::unique_lock<std::mutex> lock(buffer_lock);
        finished = true;
        ready.notify_all();
    }

    T buffer[size];
    mutable std::mutex buffer_lock;
    std::condition_variable ready;
    uint32_t buffer_size{size};
    uint32_t tail{0};
    uint32_t head{0};
    bool full{false};
    bool finished{false};
};