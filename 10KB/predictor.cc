#include "predictor.h"

uint local_history[2048];
uint local_prediction[2048];
uint global_prediction[8192];
uint choice_prediction[8192];
uint path_history;

//initialization
void initialization(){

	for(uint i=0; i<2048;i++)
	{
		local_history[i]=0;
	}
	for(uint j=0; j<2048; j++)
	{
		local_prediction[j]=7;
	}
	for(uint k=0; k<8192; k++)
	{
		global_prediction[k]=3;
	}
	for(uint l=0; l<8192; l++)
	{
		choice_prediction[l]=3;
	}
	path_history=0;
	return;
}

#define  global_mask  0x1FFF
#define  local_mask  0x7FF
#define  pc_mask  0x1FFC
 
uint global_idx;
uint choice;
uint pc_idx;
uint local_idx;

bool local_pred;
bool global_pred;
         
bool PREDICTOR::get_prediction(const branch_record_c* br, const op_state_c* os)
{
	pc_idx = (pc_mask & br->instruction_addr)>>2;
    local_idx = local_mask & local_history[pc_idx];		   
	global_idx = global_mask & path_history;
	
    if(br->is_conditional)
    {   if(choice_prediction[global_idx]>1)
		{    if(global_prediction[global_idx]>1)
	            return true;
		    else
				return false;
		}
		else{
            if(local_prediction[local_idx]>3)
	            return true;
		    else
		    	return false;
		}
    }
    else
	   return true;
}

                                                                                                                                            
void PREDICTOR::update_predictor(const branch_record_c* br, const op_state_c* os, bool taken)
{
     
    pc_idx = (pc_mask & br->instruction_addr)>>2;
    local_idx = local_mask & local_history[pc_idx];                   
    global_idx = global_mask & path_history;
	
	if(br->is_conditional){	              
            if(local_prediction[local_idx]>3)
                local_pred = true;
            else
                local_pred = false;

            if(global_prediction[global_idx]>1)
                global_pred= true;
            else
                global_pred = false;

        if(global_pred==taken && local_pred!=taken && choice_prediction[global_idx]!=3)
            choice_prediction[global_idx]++;
        else if(global_pred!=taken && local_pred==taken && choice_prediction[global_idx]!=0)
            choice_prediction[global_idx]--;
		else
			choice_prediction[global_idx]=choice_prediction[global_idx];
		
        if(taken)
        {
            if(global_prediction[global_idx]!=3)
                global_prediction[global_idx]++;
		
            if(local_prediction[local_idx]!=7)
                local_prediction[local_idx]++;
        }
        else
        {
            if(global_prediction[global_idx]!=0)
                global_prediction[global_idx]--;
	 
            if(local_prediction[local_idx]!=0)
                local_prediction[local_idx]--;
        }
	
        local_history[pc_idx] = ((local_history[pc_idx]<<1) | taken) & local_mask;
        path_history = ((path_history<<1) | taken) & global_mask;
    }
	else
		path_history = ((path_history<<1) | taken) & global_mask;
	return;				  
}
