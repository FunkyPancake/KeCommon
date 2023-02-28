#include "Ifx.h"

void Ifx_DPSearch_i16(Ifx_DPResultU16_Type* dpResult,sint16 Xin,uint32 N,const sint16* X_array)
{
    sint16 left_value = X_array[0];
    sint16 right_value = X_array[N - 1];
    uint32 idx;
    if (left_value <= right_value)
    {
        if (Xin <= left_value)
        {
            dpResult->Index = 0;
            dpResult->Ratio = 0;
        }
        else if (Xin >= right_value)
        {
            dpResult->Index = N-1;
            dpResult->Ratio = 0;
        }
        else
        {
            for(idx = 0; Xin >= X_array[idx+1];idx++)
            ;
            dpResult->Index = idx;
            if(Xin == X_array[idx])
                dpResult->Ratio = 0;
            else
            {
                dpResult->Ratio = (sint16)(((sint32)(Xin - X_array[idx])*0xffff)/(X_array[idx+1]-X_array[idx]));
            }
        }
    }
    else
    {
         if (Xin >= left_value)
        {
            dpResult->Index = 0;
            dpResult->Ratio = 0;
        }
        else if (Xin <= right_value)
        {
            dpResult->Index = N-1;
            dpResult->Ratio = 0;
        }
        else
        {
            for(idx = 0; Xin <= X_array[idx+1];idx++)
            ;
            dpResult->Index = idx;
            if(Xin == X_array[idx])
                dpResult->Ratio = 0;
            else
            {
                dpResult->Ratio = 0xffff- (sint16)(((sint32)(Xin - X_array[idx])*0xffff)/(X_array[idx+1]-X_array[idx]));
            }
        }
    }
}
sint16 Ifx_IpoCur_i16(const Ifx_DPResultU16_Type* dpResult,const sint16* Val_array)
{
    sint16 rtnVal;
    if(dpResult->Ratio == 0.0)
    {
        rtnVal = Val_array[dpResult->Index];
    }
    else
    {
        rtnVal = Ifx_Interpolate_i16(Val_array[dpResult->Index],Val_array[dpResult->Index+1],dpResult->Ratio);
    }
    return rtnVal;
}
sint16 Ifx_IpoMap_i16(const Ifx_DPResultU16_Type* dpResultX,const Ifx_DPResultU16_Type* dpResultY,uint32 num_value,const sint16* Val_array)
{
    sint16 Result,LowerX,UpperX,LowerY,UpperY;
    uint32 BaseIndex = dpResultX->Index * num_value + dpResultY->Index;
    if (dpResultX->Ratio == 0)
    {
        if (dpResultY->Ratio == 0)
        {
            Result = Val_array [BaseIndex];
        }
        else
        {
            LowerY = Val_array [BaseIndex];
            UpperY = Val_array [BaseIndex + 1];
            Result = LowerY + (UpperY -LowerY) * dpResultY->Ratio;
        }
    }
    else 
    {
        if (dpResultY->Ratio == 0)
        {
                LowerX = Val_array [BaseIndex];
                UpperX = Val_array [BaseIndex + num_value];
                Result = LowerX + (UpperX -LowerX) * dpResultX->Ratio;
        }
        else
        {
            LowerY = Val_array [BaseIndex] ;
            UpperY = Val_array [BaseIndex + 1] ;
            LowerX = LowerY + (UpperY -LowerY) * dpResultY->Ratio ;
            LowerY = Val_array [BaseIndex + num_value] ;
            UpperY = Val_array [BaseIndex + num_value + 1] ;
            UpperX = LowerY + (UpperY -LowerY) * dpResultY->Ratio ;
            Result = LowerX + (UpperX -LowerX) * dpResultX->Ratio;
        }
    }
}
sint16 Ifx_Interpolate_i16(sint16 Value1,sint16 Value2,uint16 Coef)
{
    return Value1 + ((Value2-Value1) * Coef)/(0xffff);
}
sint16 Ifx_IntIpoCur_i16_i16(sint16 Xin,uint32 N,const sint16* X_array,const sint16* Val_array)
{
	Ifx_DPResultU16_Type res;
    Ifx_DPSearch_i16(&res,Xin,N,X_array);
    return Ifx_IpoCur_i16(&res,Val_array);
}
sint16 Ifx_IntIpoMap_i16i16_i16(sint16 Xin,sint16 Yin,uint32 Nx,uint32 Ny,const sint16* X_array,const sint16* Y_array,const sint16* Val_array)
{
	Ifx_DPResultU16_Type res_x,res_y;
    Ifx_DPSearch_i16(&res_x,Xin,Nx,X_array);
    Ifx_DPSearch_i16(&res_y,Yin,Ny,Y_array);
    return Ifx_IpoMap_i16(&res_x,&res_y,Ny,Val_array);
}

