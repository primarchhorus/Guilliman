#pragma once

#include <stdlib.h>
#include <cstdint>
#include <functional>
#include <thread>
#include <vector>

#include "circular_buffer.hpp"

// #include "common.h"

const uint32_t PENDINGJOBSNUMBER = 2048;
const uint32_t JOBPOOLSIZE = 256;
const auto PROCESSORCOUNT = std::thread::hardware_concurrency();

enum class job_state {
    PENDING,
    RUNNING,
    COMPLETED,
};

struct job {
    uint32_t handle = 0;
    uint32_t id = 0;
    job_state state = job_state::COMPLETED;
    std::function<void(void)> action;
};

struct job_system {

    job_system();
    ~job_system();

    void initialize();
    void submit_job(const job& new_job);
    void worker();

    std::vector<circular_buffer<job, PENDINGJOBSNUMBER>> worker_queues;
    std::vector<std::thread> worker_threads;
    bool job_pool_empty{true};
    bool job_pool_full{false};
    std::mutex job_mutex;
    std::condition_variable job_wait;
    uint32_t index{0};
};