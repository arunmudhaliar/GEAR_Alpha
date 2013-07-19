#ifndef GXMATH_H
#define GXMATH_H

//#include "types.h"
//#include "gxDebug.h"

//#define EPSILON 0.01f

#ifndef M_PI      // may already been defined
#define M_PI  3.1415926535897932384626433832795f    // matches value in gcc v2 math.h
#endif

#define M_TWO_PI  6.283185307179586476925286766559f
#define M_180_PI  57.295779513082320876798154814105f
#define M_PI_180  0.017453292519943295769236907684886f

#define DEG2RAD(a) ((a)*M_PI_180)
#define RAD2DEG(a) ((a)*M_180_PI)

#define CLAMP(arg1, arg2, arg3) ((arg1<arg2)?arg2:(arg1>arg3)?arg3:arg1)

#ifndef MAX
#define  MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#endif

#ifndef MIN
#define  MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )
#endif

#ifndef ABS
#define ABS(val) (((val)>0)?(val):(-(val)))
#endif


#ifdef __cplusplus
extern "C"
{
#endif
extern double sin(double);
extern double cos(double);
extern double tan(double);
extern double atan(double);
extern double sqrt(double);
#ifdef __cplusplus
}
#endif


class gxMath
{
public:
	static float SINF(int ang)
	{
		return (float)sin(DEG2RAD(ang));
	}

	static float COSF(int ang)
	{
		return (float)cos(DEG2RAD(ang));
	}

	static float SINF(float ang)
	{
		return (float)sin(DEG2RAD(ang));
	}

	static float COSF(float ang)
	{
		return (float)cos(DEG2RAD(ang));
	}


	static float SQRT(float val)
	{
		return (float)sqrt(val);
	}
	
	static int SQRT(int val)
	{
		return (int)sqrt((float)val);
	}
	
	static int RAND(int min, int max)
	{
		int r=rand();
		int val=min+r%(max-min);
		return val;
	}
};


#endif //GXMATH_H