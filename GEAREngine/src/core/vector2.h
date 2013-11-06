/*
 *  vector2.h
 *  waves2
 *
 *  Created by arun on 01/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef VECTOR2_H
#define VECTOR2_H

#include "gxMath.h"
#include "basicIncludes.h"

template<class T>
class DllExport vector2;
typedef vector2<float> vector2f;

template<class T>
class DllExport vector2;
typedef vector2<int> vector2i;

template<class T>
class DllExport vector2
{
public:
	T x, y;
	
	vector2<T>()					{	x=y=0;			}
	vector2<T>(int xx, int yy)		{	x=(float)xx;	y=(float)yy;	}
	vector2<T>(float xx, float yy)	{	x=xx;	y=yy;	}
	vector2<T>(const vector2<T>& v)	{	x=v.x;	y=v.y;	}
	vector2<T>(T *v)				{	x=v[0];	y=v[1];	}

	~vector2<T>(){}
	
	void operator=(const vector2<T>& v)		{	x=v.x; y=v.y;	}

    bool operator==(const vector2<T>& v)	{	return (x==v.x && y==v.y);	}
    
	vector2<T> operator-()					{	return vector2<T>(-x, -y);	}

	vector2<T> operator+(const vector2<T>& v)	{	return vector2<T>(x+v.x, y+v.y);	}
	vector2<T> operator-(const vector2<T>& v)	{	return vector2<T>(x-v.x, y-v.y);		}
	
	void operator+=(const vector2<T>& v) 		{	x+=v.x; y+=v.y;	}
	void operator-=(const vector2<T>& v)		{	x-=v.x; y-=v.y;	}

	vector2<T> operator*(const int n)		{	return vector2<T>(x*n, y*n);	}
	vector2<T> operator*(const float n)		{	return vector2<T>(x*n, y*n);	}
	
    
	void operator/=(const int n)		{	x/=n, y/=n;	}
	void operator/=(const float n)		{	x/=n, y/=n;	}
	void operator*=(const int n)		{	x*=n, y*=n;	}
	void operator*=(const float n)		{	x*=n, y*=n;	}

	
	void set(T xx, T yy)			{	x=xx;	y=yy;	}
	
	void zero()						{	x=y=0;	}
	
	T lengthSquared()				{	return x*x+y*y;			}
	T length()						{	return gxMath::SQRT(lengthSquared());	}
	T dot(const vector2<T>& v)		{	return x*v.x+y*v.y;		}
	
//	vector2x cross(const vector2x& v)
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
		
		n=gxMath::SQRT(n);
		oon = 1.0f;
		
		oon/=n;
		x*=oon;
		y*=oon;
		
		return oon;
	}
	
};

#endif