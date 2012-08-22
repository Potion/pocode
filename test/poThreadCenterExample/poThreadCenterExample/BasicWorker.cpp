//
//  BasicWorker.cpp
//  poThreadCenterExample
//
//  Created by Steve Varga on 8/22/12.
//
//

#include "BasicWorker.h"

BasicWorker::BasicWorker() {
    
}

BasicWorker::~BasicWorker() {
    
}

void BasicWorker::workerFunc() {
    std::cout << "Thread is doing some work!" << std::endl;
    sleep(2);
    std::cout << "Thread still doing some work!" << std::endl;
    sleep(2);
    std::cout << "Thread almost done doing some work!" << std::endl;
    sleep(2);
    std::cout << "OK, the thread is done." << std::endl;
}