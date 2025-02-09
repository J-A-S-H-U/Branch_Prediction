//Perceptron Based Predictor//
#include "predictor.h"

// Number of weights
#define N 38

// Threshold values
#define ThetaMax (N*1.93+14)
#define ThetaMin ( -1*ThetaMax )

#define BitsInWeight 8
#define MAXVAL 127
#define MINVAL -128

#define SizeOfPerceptron ((N+1)*BitsInWeight)
#define NumberPerceptron 256

int perceptron[NumberPerceptron][N];
int bias[NumberPerceptron];
int GHR;
int indexxx;
int y;
int hist;
int idx;
bool prediction;

void init_predictor ()
{
  for(int i=0; i<NumberPerceptron; i++)
	for (int j=0 ; j <= N ; j++)
		perceptron[i][j] = 0;
  GHR = 0;
}

bool PREDICTOR::get_prediction(const branch_record_c* br, const op_state_c* os)
{
  int j,x;
  indexxx = br->instruction_addr % NumberPerceptron;
  j = 1;
  y = bias[indexxx];
  for(int i=0; i < N; i++){
	  if((GHR&j) == 0) 
		x = -1;
	  else
		x = 1;
	  
	  y += perceptron[indexxx][i]*x;

	  j = j << 1;
	}
  if( y >= 0 )
	prediction = true;
  else
	prediction = false;

 return prediction;
}

void PREDICTOR::update_predictor(const branch_record_c* br, const op_state_c* os, bool taken)
{
 int j,x,t;
 if(taken){
	t = 1;
 }
 else{
	t = -1;
 }
 j = 1;

 if( taken != prediction || (y < ThetaMax && y > ThetaMin) ){
	if ( bias[indexxx] > MINVAL && bias[indexxx] < MAXVAL) {
		bias[indexxx] = bias[indexxx] + t;
	}
	for(int i=0; i < N; i++){	
		if( ( ( (GHR&j) != 0) && taken ) ||  ( ((GHR&j) == 0) && !taken ) ) 
			x = 1;
		else
			x = -1;
		if ( perceptron[indexxx][i] > MINVAL && perceptron[indexxx][i] < MAXVAL ) {
			perceptron[indexxx][i] = perceptron[indexxx][i] + x;
		}
		j = j << 1;
	}

 }
 GHR = GHR << 1 | taken;
}
