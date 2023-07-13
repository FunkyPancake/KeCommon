/*
 * PID.c
 *
 *  Created on: Mar 10, 2019
 *      Author: Mati
 */
#ifndef UTILSPID_C_
#define UTILSPID_C_
#endif

#include "Std_Types.h"
#include "UtilsPID.h"

real_t PidControler_f(real_t trg, real_t proces, PidData_f_t* pid_data, const PidCal_f_t* pid_cal)
{
	real_t output;
	real_t error = trg - proces;
	real_t derror = error - pid_data->last_error;
	real_t iterm = pid_data->iterm + error;
	pid_data->last_error = error;
	//check iterm saturation
	if(iterm >pid_cal->iterm_sat_high)
		iterm = pid_cal->iterm_sat_high;
	if(iterm <pid_cal->iterm_sat_low)
		iterm = pid_cal->iterm_sat_low;	
	pid_data->iterm = iterm;
	output = pid_cal->p *error + pid_cal->i * iterm + pid_cal->d*derror;
	
	//check output saturation
	if(output>pid_cal->out_sat_high)
		output = pid_cal->out_sat_high;
	if(output<pid_cal->out_sat_low)
		output = pid_cal->out_sat_low;	
	return output;
}
sint16 PidControler_i16(sint16 trg,sint16 proces, PidData_i16_t* pid_data,const PidCal_i16_t* pid_cal)
{
	int32_t output;
	int32_t error = trg - (proces);
	int32_t derror = error - pid_data->last_error;
	int32_t iterm = pid_data->iterm + error;
	pid_data->last_error = error;
	//check iterm saturation
	if(iterm >pid_cal->iterm_sat_high)
		iterm = pid_cal->iterm_sat_high;
	if(iterm <pid_cal->iterm_sat_low)
		iterm = pid_cal->iterm_sat_low;	
	pid_data->iterm = iterm;
	output = pid_cal->p *error + pid_cal->i * iterm + pid_cal->d*derror;
	output /= pid_cal->scale;
	//check output saturation
	if(output>pid_cal->out_sat_high)
		output = pid_cal->out_sat_high;
	if(output<pid_cal->out_sat_low)
		output = pid_cal->out_sat_low;	
	return (sint16)output;
}
void PidClear_f(PidData_f_t* pid_data)
{
	pid_data->iterm =0;
	pid_data->last_error = 0;
}
#ifdef UTILSPID_C_
#undef UTILSPID_C_
#endif
