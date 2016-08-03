#include <stdio.h>
#include <math.h>
#include "sintable.h"

sintable	g_sintable;

float sintable::get_sin_vals(int angle)
{
	while (1){
		if( angle < 0  ){
			angle += NUM_SINTABLE;
		}else if( angle >= NUM_SINTABLE ){
			angle -= NUM_SINTABLE;
		}else{
			break;
		}
	}

	return g_sintable.sin_table[angle];
}

float sintable::get_cos_vals(int angle)
{
	while (1){
		if( angle < 0  ){
			angle += NUM_SINTABLE;
		}else if( angle >= NUM_SINTABLE ){
			angle -= NUM_SINTABLE;
		}else{
			break;
		}
	}

	return g_sintable.cos_table[angle];
}

int	sintable::get_acos_val(float x)
{
	return (90*x) > 0 ?  (g_sintable.acos_table[(int)(90*x)]) : (g_sintable.acos_table[(int)(90 - 90*x)]);
}

sintable::sintable()
{	
	int i;

	sin_table = new float[NUM_SINTABLE + NUM_SINTABLE / 4+1];
	cos_table = sin_table + NUM_SINTABLE / 4;

	for( i = 0 ; i < ( NUM_SINTABLE + NUM_SINTABLE / 4 + 1) ; ++i )
	{
		sin_table[i] = float( sin( PAI * i * 2 / NUM_SINTABLE ) );
	}
	cos_table[NUM_SINTABLE / 4] = 0;

	float val;
	acos_table = new int[NUM_ACOSTABLE+1];

	for( i = 0; i <= NUM_ACOSTABLE/2; ++i )
	{
		val = (float)i / 90;
		acos_table[i] = (int)( acosf(val) * 180 / PAI );
	}

	for( i = NUM_ACOSTABLE/2+1; i <= NUM_ACOSTABLE; ++i )
	{
		val = 1 - (float)i / 90;
		acos_table[i] = (int)( acosf(val) * 180 / PAI );
	}
}


sintable::~sintable()
{
	if (sin_table){ 
		delete [] sin_table; 
		sin_table	= NULL;
	}
	
	if (acos_table){ 
		delete [] acos_table; 
		acos_table	= NULL;
	}

	cos_table = 0;
}


