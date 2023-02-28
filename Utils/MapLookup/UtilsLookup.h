/*
 * UtilsLookup.h
 *
 *  Created on: Dec 23, 2018
 *      Author: Mati
 */

#ifndef UTILSLOOKUP_H_
#define UTILSLOOKUP_H_

#include "Std_Types.h"

float32 CurveLookup_f(float32 in, const volatile float32 axis[], const volatile float32 table[], uint32 size);
sint16 CurveLookup_i16(sint16 in, const volatile sint16 axis[],const volatile sint16 table[], uint32 sz);
uint16 AxisLookup_f(float32 in, const volatile float32 ordarr[], uint32 sz);
uint16 AxisLookup_i16(sint16 in, const volatile sint16 ordarr[], uint32 sz);
float32 TableLookup2D_f(uint16 row_in, uint16 col_in, float32 *map_data, uint32 row_sz, uint32 col_sz);
sint16 TableLookup2D_i16(uint16 row_in, uint16 col_in,sint16 *map_data, uint32 row_sz, uint32 col_sz);

#endif /* UTILSLOOKUP_H_ */
