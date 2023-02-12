#include "Ifl.h"

inline void Ifl_DPSearch_f32(Ifl_DPResultF32_Type* dpResult,float32 Xin,uint32 N,const float32* X_array)
{
    float32 left_value = X_array[0];
    float32 right_value = X_array[N - 1];
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
            for(idx = 0; Xin > X_array[idx+1];idx++)
            ;
            dpResult->Index = idx;
            if(Xin == X_array[idx])
                dpResult->Ratio = 0.0f;
            else
            {
                dpResult->Ratio = (Xin - X_array[idx])/(X_array[idx+1]-X_array[idx]);
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
            for(idx = 0; Xin < X_array[idx+1];idx++)
            ;
            dpResult->Index = idx;
            if(Xin == X_array[idx])
                dpResult->Ratio = 0.0f;
            else
            {
                dpResult->Ratio = 1-(Xin - X_array[idx])/(X_array[idx+1]-X_array[idx]);
            }
        }
    }
}
inline float32 Ifl_IpoCur_f32(const Ifl_DPResultF32_Type* dpResult,const float32* Val_array)
{
    float32 rtnVal;
    if(dpResult->Ratio == 0.0)
    {
        rtnVal = Val_array[dpResult->Index];
    }
    else
    {
        rtnVal = Ifl_Interpolate_f32(Val_array[dpResult->Index],Val_array[dpResult->Index+1],dpResult->Ratio);
    }
    return rtnVal;
}
inline float32 Ifl_IpoMap_f32(const Ifl_DPResultF32_Type* dpResultX,const Ifl_DPResultF32_Type* dpResultY,uint32 num_value,const float32* Val_array)
{
    float32 Result,LowerX,UpperX,LowerY,UpperY;
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
inline float32 Ifl_Interpolate_f32(float32 Value1,float32 Value2,float32 Coef)
{
    return Value1 + (Value2-Value1) * Coef;
}
float32 Ifl_IntIpoCur_f32_f32(float32 Xin,uint32 N,const float32* X_array,const float32* Val_array)
{
    Ifl_DPResultF32_Type res;
    Ifl_DPSearch_f32(&res,Xin,N,X_array);
    return Ifl_IpoCur_f32(&res,Val_array);
}
float32 Ifl_IntIpoMap_f32f32_f32(float32 Xin,float32 Yin,uint32 Nx,uint32 Ny,const float32* X_array,const float32* Y_array,const float32* Val_array)
{
    Ifl_DPResultF32_Type res_x,res_y;
    Ifl_DPSearch_f32(&res_x,Xin,Nx,X_array);
    Ifl_DPSearch_f32(&res_y,Yin,Ny,Y_array);
    return Ifl_IpoMap_f32(&res_x,&res_y,Ny,Val_array);
}

