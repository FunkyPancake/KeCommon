/*
 * UtilsLookup.c
 *
 *  Created on: Dec 23, 2018
 *      Author: Mati
 */
#ifndef UTILSLOOKUP_C_
#define UTILSLOOKUP_C_ 
#endif

#include "Std_Types.h"
#include "UtilsLookup.h"

real_t TableLookup1D_f(real_t in, const volatile real_t axis[],
        const volatile real_t table[], uint32 sz)
{
    real_t frac;
    uint32 ord;
    real_t left_value = axis[0];
    real_t right_value = axis[sz - 1];
    real_t tab_val;
    if(left_value <= right_value)
    {
        if(in <= left_value)
        {
            ord = 0;
            frac = 0;
        }
        else if(in >= right_value)
        {
            ord = sz - 1;
            frac = 1.0f;
        }
        else
        {
            for(ord = 1;in>axis[ord];ord++)
            {
                ;
            }
            frac = (in-axis[ord-1])/(axis[ord]-axis[ord-1]);
        }
    }
    else
    {
        if(in <= right_value)
        {
            ord = sz - 1;
            frac = 1.0f;
        }
        else if(in >= left_value)
        {
            ord = 0;
            frac = 0;
        }
        else
        {
            for(ord = 1;in<axis[ord];ord++)
            {
                ;
            }
            frac = (in-axis[ord])/(axis[ord-1]-axis[ord]);
        }    
    }
    if(( ord == 0 )&(frac == 0))
    {
    	tab_val = table[0];
    }
    else
    {
		left_value = table[0];
		right_value = table[sz - 1];
		if(left_value <= right_value)
		{
			tab_val = (table[ord] - table[ord-1]) * frac + table[ord-1];
		}
		else
		{
			tab_val= (table[ord-1] - table[ord]) * frac + table[ord];
		}
    }
        return (tab_val);
    
}
sint16 TableLookup1D_i16(sint16 in, const volatile sint16 axis[],
        const volatile sint16 table[], uint32 sz)
{
    real_t frac;
    uint32 ord;
    sint16 left_value = axis[0];
    sint16 right_value = axis[sz - 1];
    sint16 tab_val;
    if(left_value <= right_value)
    {
        if(in <= left_value)
        {
            ord = 0;
            frac = 0;
        }
        else if(in >= right_value)
        {
            ord = sz - 1;
            frac = 1.0f;
        }
        else
        {
            for(ord = 1;in>axis[ord];ord++)
            {
                ;
            }
            frac = (real_t)(in-axis[ord-1])/(real_t)(axis[ord]-axis[ord-1]);
        }
    }
    else
    {
        if(in <= right_value)
        {
            ord = sz - 1;
            frac = 1.0f;
        }
        else if(in >= left_value)
        {
            ord = 0;
            frac = 0;
        }
        else
        {
            for(ord = 1;in<axis[ord];ord++)
            {
                ;
            }
            frac = (real_t)(in-axis[ord])/(real_t)(axis[ord-1]-axis[ord]);
        }    
    }
    if(( ord == 0) & (frac == 0))
    {
    	tab_val = table[0];
    }
    else
    {
		left_value = table[0];
		right_value = table[sz - 1];
		if(left_value <= right_value)
		{
			tab_val = (sint16)((table[ord] - table[ord-1]) * frac + table[ord-1]);
		}
		else
		{
			tab_val= (sint16)((table[ord-1] - table[ord]) * frac + table[ord]);
		}
    }
        return tab_val;
    
}

uint16 AxisLookup_f(real_t in, const volatile real_t ordarr[], uint32 sz)
{
    real_t left_value = ordarr[0];
    real_t right_value = ordarr[sz - 1];
    real_t lo_value;
    real_t hi_value;
    real_t range;
    uint16 ord;
    if (left_value <= right_value)
    {
        if (in <= left_value)
        {
            ord = 0;
        }
        else if (in >= right_value)
        {
            ord = (uint16) ((sz << 9) - 1);
        }
        else
        {
            ord = 0;
            hi_value = ordarr[ord];
            while (hi_value <= in)
            {
                ord++;
                hi_value = ordarr[ord];
            }
            ord--;
            lo_value = ordarr[ord];
            while (lo_value > in)
            {
                ord--;
                lo_value = ordarr[ord];
            }

            range = (hi_value - lo_value);
            ord = (uint16) ((ord << 9)
                    + (real_t) ((((real_t) (in - lo_value)) * 512.0F
                            + (range / 2.0F)) / range));
        }
    }
    else
    {
        if (in >= left_value)
        {
            ord = 0;
        }
        else if (in <= right_value)
        {
            ord = (uint16) ((sz << 9) - 1);
        }
        else
        {
            ord = 0;
            lo_value = ordarr[ord];
            while (lo_value > in)
            {
                ord++;
                lo_value = ordarr[ord];
            }
            ord--;
            hi_value = ordarr[ord];
            while (hi_value < in)
            {
                ord--;
                hi_value = ordarr[ord];
            }

            range = (hi_value - lo_value);
            ord = (uint16) ((ord << 9)
                    + (real_t) ((((real_t) (hi_value -in )) * 512.0F
                            + (range / 2.0F)) / range));
        }
    }
    return ord;
}

uint16 AxisLookup_i16(sint16 in, const volatile sint16 ordarr[], uint32 sz)
{
    sint16 left_value = ordarr[0];
    sint16 right_value = ordarr[sz - 1];
    sint16 lo_value;
    sint16 hi_value;
    sint16 range;
    uint16 ord;
    if (left_value <= right_value)
    {
        if (in <= left_value)
        {
            ord = 0;
        }
        else if (in >= right_value)
        {
            ord = (uint16) ((sz << 9) - 1);
        }
        else
        {
            ord = 0;
            hi_value = ordarr[ord];
            while (hi_value <= in)
            {
                ord++;
                hi_value = ordarr[ord];
            }
            ord--;
            lo_value = ordarr[ord];
            while (lo_value > in)
            {
                ord--;
                lo_value = ordarr[ord];
            }

            range = (hi_value - lo_value);
            ord = (uint16) ((ord << 9)+(uint16)(((in - lo_value)<<9)/range));
        }
    }
    else
    {
        if (in >= left_value)
        {
            ord = 0;
        }
        else if (in <= right_value)
        {
            ord = (uint16) ((sz << 9) - 1);
        }
        else
        {
            ord = 0;
            lo_value = ordarr[ord];
            while (lo_value > in)
            {
                ord++;
                lo_value = ordarr[ord];
            }
            ord--;
            hi_value = ordarr[ord];
            while (hi_value < in)
            {
                ord--;
                hi_value = ordarr[ord];
            }

            range = (hi_value - lo_value);
            ord = (uint16) ((ord << 9)+(uint16)(((hi_value - in)<<9)/range));
        }
    }
    return ord;
}





real_t TableLookup2D_f(uint16 row_in, uint16 col_in,
        real_t *map_data, uint32 row_sz, uint32 col_sz)
{
    real_t out;
    real_t result;
    real_t r0c0_value;
    real_t r0c1_value = 0;
    real_t r1c0_value = 0;
    real_t r1c1_value = 0;
    int32_t q_frac;
    int32_t row_frac = (int32_t) ((row_in) & 0x1FF);
    int32_t col_frac = (int32_t) ((col_in) & 0x1FF);
    uint16 row_ord = (row_in) >> 9;
    uint16 col_ord = (col_in) >> 9;
    if (row_ord >= row_sz - 1)
    {
        row_ord = (uint16) (row_sz - 1);
        row_frac = 0;
    }
    if (col_ord >= col_sz - 1)
    {
        col_ord = (uint16) (col_sz - 1);
        col_frac = 0;
    }
    r0c0_value = map_data[(row_ord) * col_sz + (col_ord)];
    if (row_frac > 0)
    {
        r1c0_value = map_data[(row_ord + 1) * col_sz + (col_ord)];
    }
    if (col_frac > 0)
    {
        r0c1_value = map_data[(row_ord) * col_sz + (col_ord + 1)];
    }
    if ((col_frac > 0) && (row_frac > 0))
    {
        r1c1_value = map_data[(row_ord + 1) * col_sz + (col_ord + 1)];
    }
    q_frac = ((row_frac * col_frac) + 256) >> 9;
    result = (512 - row_frac - col_frac + q_frac) * r0c0_value;
    result += (row_frac - q_frac) * r1c0_value;
    result += (col_frac - q_frac) * r0c1_value;
    result += (q_frac) * r1c1_value;
    out = (real_t) (result / 512.0F);
    return out;
}

sint16 TableLookup2D_i16(uint16 row_in, uint16 col_in,
        sint16 *map_data, uint32 row_sz, uint32 col_sz)
{
    sint16 out;
    int32_t result;
    sint16 r0c0_value;
    sint16 r0c1_value = 0;
    sint16 r1c0_value = 0;
    sint16 r1c1_value = 0;
    int32_t q_frac;
    int32_t row_frac = (int32_t) ((row_in) & 0x1FF);
    int32_t col_frac = (int32_t) ((col_in) & 0x1FF);
    uint16 row_ord = (row_in) >> 9;
    uint16 col_ord = (col_in) >> 9;
    if (row_ord >= row_sz - 1)
    {
        row_ord = (uint16) (row_sz - 1);
        row_frac = 0;
    }
    if (col_ord >= col_sz - 1)
    {
        col_ord = (uint16) (col_sz - 1);
        col_frac = 0;
    }
    r0c0_value = map_data[(row_ord) * col_sz + (col_ord)];
    if (row_frac > 0)
    {
        r1c0_value = map_data[(row_ord + 1) * col_sz + (col_ord)];
    }
    if (col_frac > 0)
    {
        r0c1_value = map_data[(row_ord) * col_sz + (col_ord + 1)];
    }
    if ((col_frac > 0) && (row_frac > 0))
    {
        r1c1_value = map_data[(row_ord + 1) * col_sz + (col_ord + 1)];
    }
    q_frac = ((row_frac * col_frac) + 256) >> 9;
    result = (512 - row_frac - col_frac + q_frac) * r0c0_value;
    result += (row_frac - q_frac) * r1c0_value;
    result += (col_frac - q_frac) * r0c1_value;
    result += (q_frac) * r1c1_value;
    out = (sint16)(result >>9);
    return out;
}






#ifdef UTILSLOOKUP_C_
#undef UTILSLOOKUP_C_ 
#endif
