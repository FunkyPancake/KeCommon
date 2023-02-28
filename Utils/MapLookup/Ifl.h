/*
 * Ifl.h
 *
 *  Created on: Dec 23, 2018
 *      Author: Mati
 */

#ifndef IFL_H
#define IFL_H

#include "Std_Types.h"

typedef struct
{
    uint32 Index;
    float32 Ratio;
}Ifl_DPResultF32_Type;


void Ifl_DPSearch_f32(Ifl_DPResultF32_Type* dpResult,float32 Xin,uint32 N,const float32* X_array);
float32 Ifl_IpoCur_f32(const Ifl_DPResultF32_Type* dpResult,const float32* Val_array);
float32 Ifl_IpoMap_f32(const Ifl_DPResultF32_Type* dpResultX,const Ifl_DPResultF32_Type* dpResultY,uint32 num_value,const float32* Val_array);
float32 Ifl_Interpolate_f32(float32 Value1,float32 Value2,float32 Coef);
float32 Ifl_IntIpoCur_f32_f32(float32 Xin,uint32 N,const float32* X_array,const float32* Val_array);
float32 Ifl_IntIpoMap_f32f32_f32(float32 Xin,float32 Yin,uint32 Nx,uint32 Ny,const float32* X_array,const float32* Y_array,const float32* Val_array);

#endif /* IFL_H */