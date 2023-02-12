/*
 * UtilsFilters.c
 *
 *  Created on: Apr 5, 2019
 *      Author: Mati
 */
#ifndef UtilsFilters_C_
#define UtilsFilters_C_
#endif

#include "Std_Types.h"
#include "UtilsFilters.h"

real_t SimpleLPFilter(real_t new,real_t old, real_t coef)
{
	return (coef*new)+((1-coef)*old);
}

#ifdef UtilsFilters_C_
#undef UtilsFilters_C_
#endif
