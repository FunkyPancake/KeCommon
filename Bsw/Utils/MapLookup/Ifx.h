#ifndef IFX_H_
#define IFX_H_

#include "Std_Types.h"

typedef struct{
    uint16 Index;
    uint16 Ratio;
}Ifx_DPResultU16_Type;


void Ifx_DPSearch_i16(Ifx_DPResultU16_Type* dpResult,sint16 Xin,uint32 N,const sint16* X_array);
sint16 Ifx_IpoCur_i16(const Ifx_DPResultU16_Type* dpResult,const sint16* Val_array);
sint16 Ifx_IpoMap_i16(const Ifx_DPResultU16_Type* dpResultX,const Ifx_DPResultU16_Type* dpResultY,uint32 num_value,const sint16* Val_array);
sint16 Ifx_Interpolate_i16(sint16 Value1,sint16 Value2,uint16 Coef);
sint16 Ifx_IntIpoCur_i16_i16(sint16 Xin,uint32 N,const sint16* X_array,const sint16* Val_array);
sint16 Ifx_IntIpoMap_i16i16_i16(sint16 Xin,sint16 Yin,uint32 Nx,uint32 Ny,const sint16* X_array,const sint16* Y_array,const sint16* Val_array);

#endif /*IFX_H_*/
