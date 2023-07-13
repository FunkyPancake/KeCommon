/*
 * UtilsFilters.h
 *
 *  Created on: Apr 5, 2019
 *      Author: Mati
 */

#ifndef UTILSFILTERS_H_
#define UTILSFILTERS_H_
#ifdef UtilsFilters_C_
	#define EXTERNAL_
#else
	#define EXTERNAL_ extern
#endif

real_t SimpleLPFilter(real_t new,real_t old, real_t coef);

#ifdef EXTERNAL_
#undef EXTERNAL_
#endif

#endif /* UTILSFILTERS_H_ */
