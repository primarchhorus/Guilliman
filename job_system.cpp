#include <iostream>
#include "job_system.h"


job_system::job_system() {
    std::cout << PROCESSORCOUNT << std::endl;
}

job_system::~job_system() {
    try
    {
        for (size_t i = 0; i < PROCESSORCOUNT; i++) {
            worker_queues[i].finish();
        }
        for (size_t i = 0; i < PROCESSORCOUNT; i++) {
            worker_threads[i].join();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

void job_system::initialize() {
    for (size_t i = 0; i < PROCESSORCOUNT; i++) {
        circular_buffer<job, PENDINGJOBSNUMBER> cq;
        worker_queues.push_back(std::move(cq));
        std::thread w = std::thread(&job_system::worker, this);
        worker_threads.push_back(std::move(w));
    }
    return;
}

void job_system::worker() {
    std::cout << "worker starting" << std::endl;
    while (true) {
        job j;
        for (size_t i = 0; i != PROCESSORCOUNT; i++) {
            if (worker_queues[((index + i) % PROCESSORCOUNT)].try_pop_entity(j)) {
                break;
            }
        }
        if (!worker_queues[((index) % PROCESSORCOUNT)].pop_entity(j)) {
                break;
                
        }
        if (j.action) {
            j.action(); 
        }
        else {
            std::cout << "why is the action bad" << std::endl;
        }
        
        // 
    }
}

void job_system::submit_job(const job& new_job) {
    size_t current_index = ++index;
    for (size_t i = 0; i < PROCESSORCOUNT; i++) {
        if (worker_queues[((current_index + i) % PROCESSORCOUNT)].try_push_entity(new_job)) {
            return;
        }
        worker_queues[((current_index) % PROCESSORCOUNT)].push_entity(new_job);
    }

}