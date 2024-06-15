#include <stdio.h>
#include <iostream>
#include <cstdint>
#include <thread>
#include <functional>

#include "job_system.h"



void test(int x) {
    int i = 0;
    while (i < x) {
        i++;
    }
    std::cout << std::this_thread::get_id() << " func: " << x << std::endl;
}

int main(int argc, char *argv[]) {

    job_system js;
    js.initialize();
    std::cout << "init job system" << std::endl;
    for (size_t i = 0; i < 100000; i++) {

        job j;
        j.action = std::bind(&test, i);
        js.submit_job(j);
        std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
    }
    return 0;
}