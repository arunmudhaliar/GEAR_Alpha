#pragma once

#include "pxMath.h"

template<class T>
class geVector2;
typedef geVector2<float> geVector2f;

template<class T>
class geVector2;
typedef geVector2<int> geVector2i;

template<class T>
class geVector2
{
public:
	T x, y;
	
	geVector2<T>()					{	x=y=0;			}
	geVector2<T>(int xx, int yy)		{	x=(float)xx;	y=(float)yy;	}
	geVector2<T>(float xx, float yy)	{	x=xx;	y=yy;	}
	geVector2<T>(const geVector2<T>& v)	{	x=v.x;	y=v.y;	}
	geVector2<T>(T *v)				{	x=v[0];	y=v[1];	}

	~geVector2<T>(){}
	
	void operator=(const geVector2<T>& v)		{	x=v.x; y=v.y;	}

    bool operator==(const geVector2<T>& v)	{	return (x==v.x && y==v.y);	}
    
	geVector2<T> operator-()					{	return geVector2<T>(-x, -y);	}

	geVector2<T> operator+(const geVector2<T>& v)	{	return geVector2<T>(x+v.x, y+v.y);	}
	geVector2<T> operator-(const geVector2<T>& v)	{	return geVector2<T>(x-v.x, y-v.y);		}
	
	void operator+=(const geVector2<T>& v) 		{	x+=v.x; y+=v.y;	}
	void operator-=(const geVector2<T>& v)		{	x-=v.x; y-=v.y;	}

	geVector2<T> operator*(const int n)		{	return geVector2<T>(x*n, y*n);	}
	geVector2<T> operator*(const float n)		{	return geVector2<T>(x*n, y*n);	}
	
    
	void operator/=(const int n)		{	x/=n, y/=n;	}
	void operator/=(const float n)		{	x/=n, y/=n;	}
	void operator*=(const int n)		{	x*=n, y*=n;	}
	void operator*=(const float n)		{	x*=n, y*=n;	}

	
	void set(T xx, T yy)			{	x=xx;	y=yy;	}
	
	void zero()						{	x=y=0;	}
	
	T lengthSquared()				{	return x*x+y*y;			}
	T length()						{	return pxMath::SQRT(lengthSquared());	}
	T dot(const geVector2<T>& v)		{	return x*v.x+y*v.y;		}
	
//	geVector2x cross(const geVector2x& v)
//	{
//		vector3x res;
//		res.x=MULTX(y, v.z)-MULTX(z, v.y);
//		res.y=MULTX(z, v.x)-MULTX(x, v.z);
//		res.z=MULTX(x, v.y)-MULTX(y, v.x);
//		return res;
//	}
	
	T normalize()
	{
		T n, oon;
		n=lengthSquared();
		if(n==0.0f || n==1.0f) return 0.0f;
		
		n=pxMath::SQRT(n);
		oon = 1.0f;
		
		oon/=n;
		x*=oon;
		y*=oon;
		
		return oon;
	}
	
};