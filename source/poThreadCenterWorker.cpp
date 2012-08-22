#include "poThreadCenterWorker.h"

poThreadCenterWorker::poThreadCenterWorker() {
    workerAutoDelete = true;
}

poThreadCenterWorker::~poThreadCenterWorker() {
}

void poThreadCenterWorker::setWorkerParams(poObject *notify, std::string message, const poDictionary &dict) {
    this->notify = notify;
    this->workerMessage = message;
    this->dict.append(dict);
}

void poThreadCenterWorker::run() {
    workerFunc();
    poThreadCenter::get()->workerDone(this);
}

void poThreadCenterWorker::workerFunc() {
    std::cout << "Thread is doing some work!" << std::endl;
    sleep(2);
    std::cout << "Thread still doing some work!" << std::endl;
    sleep(2);
    std::cout << "Thread almost done doing some work!" << std::endl;
    sleep(2);
    std::cout << "OK, the thread is done." << std::endl;
}

poObject* poThreadCenterWorker::getWorkerNotify() {
    return notify;
}



