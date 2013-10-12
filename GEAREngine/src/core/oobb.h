#ifndef OOBB_H
#define OOBB_H


#include "vector3.h"
#include "gxMath.h"
#include "basicIncludes.h"
#include "../hwShader/gxHWShader.h"


template<class T>
class OOBB;
typedef OOBB<float> gxOOBBf;

template<class T>
class OOBB
{
public:
	OOBB<T>()
    {
    }
    
    OOBB<T>(const OOBB<T>& aabb)
    {
        m_min.set(aabb.m_min.x, aabb.m_min.y, aabb.m_min.z);
        m_max.set(aabb.m_max.x, aabb.m_max.y, aabb.m_max.z);
    }

    OOBB<T>(const vector3<T>& _min, const vector3<T>& _max)
    {
        m_min=_min;
        m_max=_max;
    }

    OOBB<T>(T min_x, T min_y, T min_z, T max_x, T max_y, T max_z)
    {
        m_min.set(min_x, min_y, min_z);
        m_max.set(max_x, max_y, max_z);
    }

	void set(const vector3<T>& _min, const vector3<T>& _max)
    {
        m_min=_min;
        m_max=_max;
    }

    void operator=(const OOBB<T>& aabb)
    {
        m_min.set(aabb.m_min.x, aabb.m_min.y, aabb.m_min.z);
        m_max.set(aabb.m_max.x, aabb.m_max.y, aabb.m_max.z);
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