#include "predictor.h"

uint32_t local_history[1024];
uint32_t local_prediction[1024];
uint32_t global_prediction[4096];
uint32_t choice_prediction[4096];
uint16_t path_history;



uint32_t global_mask = 0x00000FFF;
uint32_t local_mask	 = 0x000003FF;
uint32_t pc_mask	 = 0x000003FF;

uint32_t global_histbits;
uint32_t pc_idx;
uint32_t local_hist;

void initialization() {
    for (int i = 0; i < 1024; i++) 
	{
        local_history[i] = 0;		// Local Predictor
		local_prediction[i] = 0;
    }
    for (int i = 0; i < 4096; i++) 
	{
        global_prediction[i] = 0;	//Global Predictor
		choice_prediction[i] = 0;	//Choice Predictor
    }
    path_history = 0;
}




bool PREDICTOR::get_prediction(const branch_record_c* br, const op_state_c* os) {



    bool prediction;
	uint32_t choice;

    global_histbits = global_mask & path_history;
    choice	= choice_prediction[global_histbits];
	
    if (choice <2 )
	{
        pc_idx = (pc_mask & br->instruction_addr)>>2;
        local_hist = local_mask & local_history[pc_idx];

        if (local_prediction[local_hist] >=4) 
		{
            prediction = true;
        } 
		else 
		{
            prediction = false;
        }
    } 
	else 
	{
        if (global_prediction[global_histbits] >=2) 
		{
            prediction = true;
        } 
		else 
		{
            prediction = false;
        }
    }

    return prediction;
}








void PREDICTOR::update_predictor(const branch_record_c* br, const op_state_c* os, bool taken) {
    
	uint32_t local_pred;
    uint32_t global_pred;

    global_histbits = global_mask & path_history;
    pc_idx 			= (pc_mask & br->instruction_addr) >> 2;
    local_hist 		= local_mask & local_history[pc_idx];

    local_pred 		= local_prediction[local_hist] >=4 ? true : false;
    global_pred 	= global_prediction[global_histbits] >=2 ? true : false;


    if (global_pred == taken && local_pred != taken && choice_prediction[global_histbits] != 3) 
	{
        choice_prediction[global_histbits]++;
    } 
	else if (global_pred != taken && local_pred == taken && choice_prediction[global_histbits] != 0) 
	{
        choice_prediction[global_histbits]--;
    }


    if (taken) 
	{
        if (global_prediction[global_histbits] != 3)
            global_prediction[global_histbits]++;
        if (local_prediction[local_hist] != 7)
            local_prediction[local_hist]++;
    } 
	else 
	{
        if (global_prediction[global_histbits] != 0)
            global_prediction[global_histbits]--;
        if (local_prediction[local_hist] != 0)
            local_prediction[local_hist]--;
    }

    local_history[pc_idx] = ((local_history[pc_idx] << 1) | taken) & local_mask;
    path_history		  = (((path_history << 1) | taken) & global_mask);
}
