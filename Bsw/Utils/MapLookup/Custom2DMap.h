//
// Created by PC on 11.02.2023.
//

#ifndef WGECU_CUSTOM2DMAP_H
#define WGECU_CUSTOM2DMAP_H

class Custom2DMap : IMap
{
public:
    Custom1DMap(uint16_t &x, uint16_t &y, uint16_t &out, uint16_t *map);
    void Calc();

private:
    uint16_t &_x;
    uint16_t &_y;
    uint16_t &_out;
    uint16_t &_map[];
};

#endif//WGECU_CUSTOM2DMAP_H
