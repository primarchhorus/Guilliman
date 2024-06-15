#include <stdio.h>
#include <iostream>
#include <cstdint>
#include <thread>
#include <functional>

#include "job_system.h"



void test(int x) {
    // std::cout << "func: " << x << std::endl;
    x = x + 1;
}

int main(int argc, char *argv[]) {

    job_system js;
    js.initialize();
    std::cout << "init job system" << std::endl;
    for (size_t i = 0; i < 10000000; i++) {

        job j;
        j.action = std::bind(&test, i);
        js.submit_job(std::move(j));
        // std::cout << "submited job " << i << std::endl;
    }
    return 0;
}

// Maybe i can change this, keep all the job items in a vector and just operate on it via handle (some int) 
// Will need functions to allow operating on the job in place in the array, pass handle as you set each item, indidually, could use a struct as input?
// queue of jobs is just the handle id?