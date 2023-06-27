//
// Created by PC on 27.06.2023.
//

#ifndef CANGPS_QUEUE_H
#define CANGPS_QUEUE_H


#include "IQueue.h"

class Queue
: IQueue {
public:

uint8_t Peek(int i) {
    return 0;
}

int Count;

std::vector <int8_t> WaitDequeue(int i) {
    return {};
}

std::vector <uint8_t> Dequeue(int i) {
    return {};
}

};


#endif //CANGPS_QUEUE_H
