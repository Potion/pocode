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
    //Override this function for threaded functionality
}

poObject* poThreadCenterWorker::getWorkerNotify() {
    return notify;
}



