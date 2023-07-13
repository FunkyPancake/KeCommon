/*
 * PID.h
 *
 *  Created on: Mar 10, 2019
 *      Author: Mati
 */

#ifndef UTILSPID_H_
#define UTILSPID_H_

#ifdef UTILSPID_C_
#define EXTERNAL_
#else
#define EXTERNAL_ extern
#endif

typedef struct
{
	real_t p;
	real_t i;
	real_t d;
	real_t iterm_sat_high;
	real_t iterm_sat_low;
	real_t out_sat_high;
	real_t out_sat_low;	
} PidCal_f_t;
typedef struct
{
	real_t last_error;
	real_t iterm;
} PidData_f_t;

typedef struct
{
	sint16 p;
	sint16 i;
	sint16 d;
	sint16 scale;
	int32_t iterm_sat_high;
	int32_t iterm_sat_low;
	sint16 out_sat_high;
	sint16 out_sat_low;	
} PidCal_i16_t;
typedef struct
{
	int32_t last_error;
	int32_t iterm;
} PidData_i16_t;

real_t PidControler_f(real_t trg, real_t proces, PidData_f_t* pid_data, const PidCal_f_t* pid_cal);
sint16 PidControler_i16(sint16 trg,sint16 proces, PidData_i16_t* pid_data, const PidCal_i16_t* pid_cal);
void PidClear_f(PidData_f_t* pid_data);
#ifdef EXTERNAL_
#undef EXTERNAL_
#endif
#endif /* PID_H_ */
