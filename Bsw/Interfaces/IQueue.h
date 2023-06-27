//
// Created by PC on 27.06.2023.
//

#ifndef CANGPS_IQUEUE_H
#define CANGPS_IQUEUE_H

#include <cstdint>

class IQueue {

public:
    virtual uint8_t Peek(int i) = 0;

    int Count;

    virtual std::vector<int8_t> WaitDequeue(int i) = 0;

    virtual std::vector<uint8_t> Dequeue(int i) = 0;
};

#endif //CANGPS_IQUEUE_H
