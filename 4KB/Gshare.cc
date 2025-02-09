#include "predictor.h"

uint global_prediction[16384];
uint global_history;

uint pcbits;
uint histbits;
uint indexxx;

#define global_mask  0x3FFF
#define pc_mask      0xFFFC

//initialization
void initialization(){

	for(uint i=0; i<16384; i++)
	{
		global_prediction[i]=0;
	}
	global_history=0;
	return;
}


bool PREDICTOR::get_prediction(const branch_record_c* br, const op_state_c* os)
{
	pcbits = (br->instruction_addr & pc_mask)>>2;
    histbits = global_history & global_mask;
    indexxx = histbits ^ pcbits;
  
    if(br->is_conditional)
	{
		if(global_prediction[indexxx]>1)
			return true;
		else
			return false;
	}
	else
		return true;
}


void PREDICTOR::update_predictor(const branch_record_c* br, const op_state_c* os, bool taken)
{
    pcbits = (br->instruction_addr & pc_mask)>>2;
    histbits = global_history & global_mask;
    indexxx = histbits ^ pcbits;
	if(br->is_conditional){
		if(taken)
		{
			if(global_prediction[indexxx]!=3)
				global_prediction[indexxx]++;
		}
		else
		{
			if(global_prediction[indexxx]!=0)
				global_prediction[indexxx]--;
		}
		global_history = ((global_history<<1) | taken) & global_mask;
    }
	else
		global_history = ((global_history<<1) | taken) & global_mask;
	
	return;
}
