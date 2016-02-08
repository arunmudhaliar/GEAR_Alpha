#pragma once

#include "vector3.h"

#define PLANE_EPSILON 0.01f

template<class T> class plane3;
typedef DECLSPEC plane3<float> gxPlane3f;

template<class T>
class DECLSPEC plane3
{
public:
	enum Halfspace
	{
		NEGATIVE = -1,
		ON_PLANE = 0,
		POSITIVE = 1,
		SPANNING = 2
	};

	plane3<T>()
	{	}

	plane3<T>(const plane3<T>& _plane)
	{
		normal=_plane.normal;
		d=_plane.d;
	}

	T distanceToPoint(const vector3<T>& pt)
	{	return pt.dot(normal) + d;	}

	static T distanceToPoint(const plane3<T>& _plane, const vector3<T>& pt)
	{	return _plane.normal.x*pt.x + _plane.normal.y*pt.y + _plane.normal.z*pt.z + _plane.d;	}

	int classifyPoint(const vector3<T>& pt)
	{
		T distance;
		distance = normal.x*pt.x + normal.y*pt.y + normal.z*pt.z + d;

		//==0.0f is too exact, give a bit of room
		if (distance < -PLANE_EPSILON) return NEGATIVE;
		if (distance > PLANE_EPSILON) return POSITIVE;
		return ON_PLANE;
	}

	static int classifyPoint(const plane3<T>& _plane, const vector3<T>& pt)
	{
		T distance;
		distance = _plane.normal.x*pt.x + _plane.normal.y*pt.y + _plane.normal.z*pt.z + _plane.d;

		//==0.0f is too exact, give a bit of room
		if (distance < -PLANE_EPSILON) return NEGATIVE;
		if (distance > PLANE_EPSILON) return POSITIVE;
		return ON_PLANE;
	}

	int classifyTriangle(const vector3<T>& a, const vector3<T>& b, const vector3<T>& c)
	{
		//numPositive = numNegative = 0;
		//For each point p in polygon
		//value = classifyPoint(plane, polygon);
		//If (value == INFRONT) then numPositive++;
		//If (value == BEHIND) then numNegative++;
		//If (numPositive > 0 && numNegative == 0) then return INFRONT;
		//If (numPositive == 0 && numNegative > 0) then return BEHIND;
		//If (numPositive > 0 && numNegative > 0) then return SPANNING;
		//return COINCIDENT;

		int nPositive, nNegative;
		nPositive=nNegative=0;

		if(ClassifyPoint(a)==POSITIVE) nPositive++;
		if(ClassifyPoint(a)==NEGATIVE) nNegative++;

		if(nPositive > 0 && nNegative == 0) return POSITIVE;
		if(nPositive == 0 && nNegative > 0) return NEGATIVE;
		if(nPositive > 0 && nNegative > 0) return SPANNING;
		return ON_PLANE;
	}

	static vector3<T> intersectionPoint(const plane3<T>& _plane1, const plane3<T>& _plane2, const plane3<T>& _plane3)
	{
		//pxPoint3f aPt;
		T d1,d2,d3;			//Distance
		vector3<T> aN1,aN2,aN3;	//Plane Normals

		d1=-_plane1.d;
		d2=-_plane2.d;
		d3=-_plane3.d;

		aN1	= _plane1.getNormal();
		aN2	= _plane2.getNormal();
		aN3	= _plane3.getNormal();

		vector3<T> N2xN3, N3xN1, N1xN2;		
		N2xN3	= aN2.cross(aN3);
		N3xN1	= aN3.cross(aN1);
		N1xN2	= aN1.cross(aN2);

		vector3<T> d1N2xN3, d2N3xN1, d3N1xN2;
		d1N2xN3	= N2xN3*d1;
		d2N3xN1	= N3xN1*d2;
		d3N1xN2	= N1xN2*d3;

		T N1oN2xN3;
		N1oN2xN3	= aN1.dot(N2xN3);

		//		vector3<T> aAddedResult;
		//		aAddedResult	= d3N1xN2+d1N2xN3+d2N3xN1;

		vector3<T> aIntersectionPoint;
		//aIntersectionPoint	= aAddedResult/N1oN2xN3;
		aIntersectionPoint	= (d3N1xN2/N1oN2xN3)+(d1N2xN3/N1oN2xN3)+(d2N3xN1/N1oN2xN3);

		return aIntersectionPoint; //vector3<T>(aIntersectionPoint.x, aIntersectionPoint.y, aIntersectionPoint.z);
	}

	T signedDistanceTo(const vector3<T>& aPoint) //const
	{	return(aPoint.dot(normal)+ d);	}

	bool isFrontfacingTo(const vector3<T>& aNormal) const
	{
		T dot = normal.dot(aNormal);
		return (dot<=0);
	}

	vector3<T> getNormal() const
	{	return normal;	}

	plane3<T>(vector3<T> aP1, vector3<T> aP2, vector3<T> aP3)
	{
		normal=(aP2-aP1).Cross((aP3-aP1));
		normal.normalize();
		d	= -(normal.x*aP1.x + normal.y*aP1.y + normal.z*aP1.z);
	}

	void create(const vector3<T>& aP1, const vector3<T>& aP2, const vector3<T>& aP3)
	{
		normal=(aP2-aP1).Cross((aP3-aP1));
		normal.normalize();
		d = -(normal.x*aP1.x + normal.y*aP1.y + normal.z*aP1.z);
	}

	void create(vector3<T>& aOrig, vector3<T>& aNrml)
	{
		normal=aNrml;
		d=-(normal.dot(aOrig));
	}

	void normalize()
	{	d = d*normal.normalize();	}

	//Plane Variables
	vector3<T>	normal;
	T			d;
};