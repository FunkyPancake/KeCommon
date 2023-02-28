//
// Created by PC on 11.02.2023.
//

#ifndef WGECU_CUSTOM1DMAP_H
#define WGECU_CUSTOM1DMAP_H
#include "IMap.h"
class Custom1DMap : IMap
{
public:
    Custom1DMap(uint16_t &in, uint16_t &out, uint16_t *map);
    void Calc();

private:
    uint16_t &_in;
    uint16_t &_out;
    uint16_t &_map[];
};

#endif//WGECU_CUSTOM1DMAP_H
