//
// Created by PC on 11.02.2023.
//

#include "Custom1DMap.h"
#include "Ifx.h"
void Custom1DMap::Calc()
{
    uint16_t size = _map[0];
    _out = Ifx_IntIpoCur_i16_i16(_in,size[0],_map[1]),_map[size+1];
}

Custom1DMap::Custom1DMap(int &in, int &out, int *map)
{
    _in = in;
    _out = out;
    _map = map;
}
