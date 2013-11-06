//
//  aabb.h
//  waves2
//
//  Created by arun on 27/04/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef AABB_H
#define AABB_H

#include "vector2.h"
#include "gxMath.h"
#include "basicIncludes.h"
#include "../hwShader/gxHWShader.h"

class gxHWShader;
template<class T>
class DllExport AABB;
typedef AABB<int> gxAABBi;

template<class T>
class DllExport AABB;
typedef AABB<float> gxAABBf;

template<class T>
class DllExport AABB
{
public:
    AABB<T>()
    {
    }
    
    AABB<T>(const AABB<T>& aabb)
    {
        m_min.set(aabb.m_min.x, aabb.m_min.y, aabb.m_min.z);
        m_max.set(aabb.m_max.x, aabb.m_max.y, aabb.m_max.z);
    }

    AABB<T>(const vector3<T>& _min, const vector3<T>& _max)
    {
        m_min=_min;
        m_max=_max;
    }

    AABB<T>(T min_x, T min_y, T min_z, T max_x, T max_y, T max_z)
    {
        m_min.set(min_x, min_y, min_z);
        m_max.set(max_x, max_y, max_z);
    }

	void set(const vector3<T>& _min, const vector3<T>& _max)
    {
        m_min=_min;
        m_max=_max;
    }

    void operator=(const AABB<T>& aabb)
    {
        m_min.set(aabb.m_min.x, aabb.m_min.y, aabb.m_min.z);
        m_max.set(aabb.m_max.x, aabb.m_max.y, aabb.m_max.z);
	}
   
    vector3<T> getCenter()
	{
		vector3<T> diff(m_max-m_min);
		return vector3<T>(m_min+diff*0.5f);
	}
    
	vector3<T> getSize()	{	return (m_max-m_min);		}
	vector3<T> getHSize()	{	return ((m_max-m_min)*0.5f);}
    
	bool isOverLap(AABB<T>& aabb)//both should be in world space
	{
		vector3<T>	aCenterA(getCenter());
		vector3<T>	aCenterB(aabb.getCenter());
		vector3<T>	aSzA(getHSize());
		vector3<T>	aSzB(aabb.getHSize());
        
		vector3<T>	aV(aCenterA - aCenterB);
		return	ABS(aV.x) <= (aSzA.x + aSzB.x)
        &&
        ABS(aV.y) <= (aSzA.y + aSzB.y)
		&&
		ABS(aV.z) <= (aSzA.z + aSzB.z);
	}
    
	bool isOverlapsSphere(const T r, vector3<T>& C)
	{
		vector3<T>	aCenterA(getCenter());
		vector3<T>	aSzA(getHSize());

		vector3<T>	aV(aCenterA - C);

		return	ABS(aV.x) <= (aSzA.x + r)
				&&
				ABS(aV.y) <= (aSzA.y + r)
				&&
				ABS(aV.z) <= (aSzA.z + r);
	}

    bool isPointInsideAABB(vector3<T> v)
    {
        return (m_min.x<v.x && m_min.y<v.y && m_min.z<v.z && m_max.x>v.x && m_max.y>v.y && m_max.z>v.z);
    }
    
	void scale(float scale)
	{
		vector3<T> sz(getSize());
		vector3<T> center(getCenter());

		sz=sz*(scale*0.5f);
		m_min=center-sz;
		m_max=center+sz;
	}

	void translate(T dx, T dy, T dz)
	{
		m_min.x+=dx;
		m_min.y+=dy;
		m_min.z+=dz;

		m_max.x+=dx;
		m_max.y+=dy;
		m_max.z+=dz;
	}

	T getLongestAxis()
	{
		vector3<T> diff(m_max-m_min);
		diff.x=ABS(diff.x);
		diff.y=ABS(diff.y);
		diff.z=ABS(diff.z);

		return (T)MAX(diff.x,MAX(diff.y, diff.z));
	}

	void draw(gxHWShader* shader, bool bDrawBox=false)
	{
				float lineAry[]={
			m_min.x, m_min.y, m_min.z,		//0
			m_min.x, m_min.y, m_max.z,		//1
			m_max.x, m_min.y, m_max.z,		//2
			m_max.x, m_min.y, m_min.z,		//3

			m_max.x, m_min.y, m_min.z,		//3
			m_max.x, m_min.y, m_max.z,		//2
			m_max.x, m_max.y, m_max.z,		//6	
			m_max.x, m_max.y, m_min.z,		//7

			m_max.x, m_max.y, m_min.z,		//7
			m_max.x, m_max.y, m_max.z,		//6	
			m_min.x, m_max.y, m_max.z,		//5
			m_min.x, m_max.y, m_min.z,		//4

			m_min.x, m_max.y, m_min.z,		//4
			m_min.x, m_max.y, m_max.z,		//5
			m_min.x, m_min.y, m_max.z,		//1
			m_min.x, m_min.y, m_min.z,		//0
		};

		float vertAry[]={
			m_min.x, m_min.y, m_min.z,		//0
			m_max.x, m_min.y, m_min.z,		//3
			m_min.x, m_min.y, m_max.z,		//1
			
			m_max.x, m_min.y, m_min.z,		//3
			m_max.x, m_min.y, m_max.z,		//2
			m_min.x, m_min.y, m_max.z,		//1
			
			m_min.x, m_min.y, m_min.z,		//0
			m_min.x, m_max.y, m_max.z,		//5
			m_min.x, m_max.y, m_min.z,		//4

			m_min.x, m_min.y, m_min.z,		//0
			m_min.x, m_min.y, m_max.z,		//1
			m_min.x, m_max.y, m_max.z,		//5

			m_min.x, m_max.y, m_min.z,		//4
			m_min.x, m_max.y, m_max.z,		//5			
			m_max.x, m_max.y, m_min.z,		//7

			m_min.x, m_max.y, m_max.z,		//5	
			m_max.x, m_max.y, m_max.z,		//6	
			m_max.x, m_max.y, m_min.z,		//7
			
			m_max.x, m_max.y, m_min.z,		//7
			m_max.x, m_max.y, m_max.z,		//6	
			m_max.x, m_min.y, m_min.z,		//3
			
			m_max.x, m_min.y, m_min.z,		//3
			m_max.x, m_max.y, m_max.z,		//6	
			m_max.x, m_min.y, m_max.z,		//2
			
			m_min.x, m_min.y, m_max.z,		//1
			m_max.x, m_min.y, m_max.z,		//2
			m_min.x, m_max.y, m_max.z,		//5

			m_min.x, m_max.y, m_max.z,		//5
			m_max.x, m_min.y, m_max.z,		//2		
			m_max.x, m_max.y, m_max.z,		//6	
			
			m_min.x, m_min.y, m_min.z,		//0
			m_min.x, m_max.y, m_min.z,		//4
			m_max.x, m_min.y, m_min.z,		//3

			m_max.x, m_min.y, m_min.z,		//3
			m_min.x, m_max.y, m_min.z,		//4
			m_max.x, m_max.y, m_min.z,		//7
		};

		
		unsigned int mode=(bDrawBox)?GL_TRIANGLES:GL_LINE_LOOP;

#if defined (USE_ProgrammablePipeLine)
        glEnableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4"));
		if(mode==GL_TRIANGLES)
		{
			glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 3, GL_FLOAT, GL_FALSE, 0, vertAry);
			glDrawArrays(mode, 0, 36);
		}
		else
		{
			for(int x=0;x<4;x++)
			{	
				glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 3, GL_FLOAT, GL_FALSE, 0, &lineAry[x*3*4]);
				glDrawArrays(mode, 0, 4);
			}
		}
		glDisableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4"));
#else
		
		glEnableClientState(GL_VERTEX_ARRAY);
		if(mode==GL_TRIANGLES)
		{
			glVertexPointer(3, GL_FLOAT, 0, vertAry);
			glDrawArrays(mode, 0, 36);
		}
		else
		{
			for(int x=0;x<4;x++)
			{	
				glVertexPointer(3, GL_FLOAT, 0, &lineAry[x*3*4]);
				glDrawArrays(mode, 0, 4);
			}
		}
		glDisableClientState(GL_VERTEX_ARRAY);
#endif
	}

    vector3<T> m_min;
    vector3<T> m_max;
};

#endif