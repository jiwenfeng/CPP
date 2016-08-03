#ifndef __SIN_TABLE_H__
#define __SIN_TABLE_H__

//#define M_PI (3.14159265358979323846f)

#define		PAI						3.1415926535897932384f
#define		ANGLE_PER_RADIAN		57.29578f
#define		NUM_SINTABLE			360
#define		NUM_ACOSTABLE			180


#define		FSIN( x )				( g_sintable.get_sin_vals(x) )
#define		FCOS( x )				( g_sintable.get_cos_vals(x) )
#define		FACOS( x )				( g_sintable.get_acos_val(x) )

class sintable
{
public:
	sintable();

	virtual ~sintable();

	float get_sin_vals(int angle);

	float get_cos_vals(int angle);

	int get_acos_val(float x);

private:
	float* sin_table;
	float* cos_table;
	int	*  acos_table;
};

extern sintable	g_sintable;

#endif
