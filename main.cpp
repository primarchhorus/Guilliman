#include <stdio.h>
#include <iostream>
#include <cstdint>
#include <thread>
#include <functional>

#include "job_system.h"



void test(int x) {
    x = x + 1;
}

int main(int argc, char *argv[]) {

    job_system js;
    js.initialize();
    std::cout << "init job system" << std::endl;
    for (size_t i = 0; i < 10000000; i++) {

        job j;
        j.action = std::bind(&test, i);
        js.submit_job(j);
    }
    return 0;
}