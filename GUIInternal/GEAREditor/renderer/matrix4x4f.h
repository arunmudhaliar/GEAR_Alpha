/*
 *  matrix4x4x.h
 *  waves2
 *
 *  Created by arun on 31/01/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GEMATRIX4X4F_H
#define GEMATRIX4X4F_H

#include "glincludes.h"


class geMatrix4x4f
{
protected:
	float	m[16];

public:
	geMatrix4x4f()
	{
		identity();
	}
	
	geMatrix4x4f(float v)
	{
		for(register int x=0;x<16;x++)
		{
			m[x]=v;
		}		
	}
	
	geMatrix4x4f(const geMatrix4x4f& matrix)
	{
		copy(matrix);
	}
	
	virtual ~geMatrix4x4f()
	{}
	
	float operator[](int i)			{	return(i<16? m[i]:0.0f);	}

	void operator=(const geMatrix4x4f& matrix)
	{
		copy(matrix);
	}
	
	void copy(const geMatrix4x4f& matrix)
	{
		for(register int x=0;x<16;x++)
		{
			m[x]=matrix.m[x];
		}		
	}
	
	void identity()
	{
		memset(m, 0, sizeof(m));
		m[0] = 1.0f;
		m[5] = m[0];
		m[10] = m[0];
		m[15] = m[0];
	}
	
    void setScale(float sx, float sy, float sz)
    {
        m[0]=sx;
        m[5]=sy;
        m[10]=sz;
    }
    
    const float* getMatrix() const {   return m;   }
    
	geMatrix4x4f operator*(const geMatrix4x4f& matrix) const
	{
		geMatrix4x4f dest;
		
		dest.m[0]	= m[0]* matrix.m[0]  + m[4]* matrix.m[1]  + m[8] * matrix.m[2]  + m[12]* matrix.m[3];
		dest.m[1]	= m[1]* matrix.m[0]  + m[5]* matrix.m[1]  + m[9] * matrix.m[2]  + m[13]* matrix.m[3];
		dest.m[2]	= m[2]* matrix.m[0]  + m[6]* matrix.m[1]  + m[10]* matrix.m[2]  + m[14]* matrix.m[3];
		dest.m[3]	= m[3]* matrix.m[0]  + m[7]* matrix.m[1]  + m[11]* matrix.m[2]  + m[15]* matrix.m[3];
		dest.m[4]	= m[0]* matrix.m[4]  + m[4]* matrix.m[5]  + m[8] * matrix.m[6]  + m[12]* matrix.m[7];
		dest.m[5]	= m[1]* matrix.m[4]  + m[5]* matrix.m[5]  + m[9] * matrix.m[6]  + m[13]* matrix.m[7];
		dest.m[6]	= m[2]* matrix.m[4]  + m[6]* matrix.m[5]  + m[10]* matrix.m[6]  + m[14]* matrix.m[7];
		dest.m[7]	= m[3]* matrix.m[4]  + m[7]* matrix.m[5]  + m[11]* matrix.m[6]  + m[15]* matrix.m[7];
		dest.m[8]	= m[0]* matrix.m[8]  + m[4]* matrix.m[9]  + m[8] * matrix.m[10] + m[12]* matrix.m[11];
		dest.m[9]	= m[1]* matrix.m[8]  + m[5]* matrix.m[9]  + m[9] * matrix.m[10] + m[13]* matrix.m[11];
		dest.m[10]	= m[2]* matrix.m[8]  + m[6]* matrix.m[9]  + m[10]* matrix.m[10] + m[14]* matrix.m[11];
		dest.m[11]	= m[3]* matrix.m[8]  + m[7]* matrix.m[9]  + m[11]* matrix.m[10] + m[15]* matrix.m[11];
		dest.m[12]	= m[0]* matrix.m[12] + m[4]* matrix.m[13] + m[8] * matrix.m[14] + m[12]* matrix.m[15];
		dest.m[13]	= m[1]* matrix.m[12] + m[5]* matrix.m[13] + m[9] * matrix.m[14] + m[13]* matrix.m[15];	
		dest.m[14]	= m[2]* matrix.m[12] + m[6]* matrix.m[13] + m[10]* matrix.m[14] + m[14]* matrix.m[15];
		dest.m[15]	= m[3]* matrix.m[12] + m[7]* matrix.m[13] + m[11]* matrix.m[14] + m[15]* matrix.m[15];

		return dest;
	}
	
	void inverse()	{	*this=getInverse();	}

	geMatrix4x4f getInverse()
	{
		float *mat, *dst; 
		geMatrix4x4f res(0);
		dst = res.m;
		mat = m;
		
		/************************************************************
		 *
		 * input:
		 * mat - pointer to array of 16 no's (source matrix)
		 * output:
		 * dst - pointer to array of 16 no's (invert matrix)
		 *
		 *************************************************************/
		register int i;
		float tmp[12];	/* temp array for pairs */
		float src[16];	/* array of transpose source matrix */
		float det;		/* determinant */
		
		/* transpose matrix */
		for( i = 0; i < 4; i++) 
		{
			src[i]   = mat[i*4];
			src[i+4] = mat[i*4 + 1];
			src[i+8] = mat[i*4 + 2];
			src[i+12]= mat[i*4 + 3];
		}

		/* calculate pairs for first 8 elements (cofactors) */
		tmp[0] = src[10] * src[15];
		tmp[1] = src[11] * src[14];
		tmp[2] = src[9]  * src[15];
		tmp[3] = src[11] * src[13];
		tmp[4] = src[9]  * src[14];
		tmp[5] = src[10] * src[13];
		tmp[6] = src[8]  * src[15];
		tmp[7] = src[11] * src[12];
		tmp[8] = src[8]  * src[14];
		tmp[9] = src[10] * src[12];
		tmp[10]= src[8]  * src[13];
		tmp[11]= src[9]  * src[12];
		
		/* calculate first 8 elements (cofactors) */
		dst[0]  = tmp[0]* src[5] + tmp[3]* src[6] + tmp[4]* src[7];
		dst[0] -= tmp[1]* src[5] + tmp[2]* src[6] + tmp[5]* src[7];
		dst[1]  = tmp[1]* src[4] + tmp[6]* src[6] + tmp[9]* src[7];
		dst[1] -= tmp[0]* src[4] + tmp[7]* src[6] + tmp[8]* src[7];
		dst[2]  = tmp[2]* src[4] + tmp[7]* src[5] + tmp[10]* src[7];
		dst[2] -= tmp[3]* src[4] + tmp[6]* src[5] + tmp[11]* src[7];
		dst[3]  = tmp[5]* src[4] + tmp[8]* src[5] + tmp[11]* src[6];
		dst[3] -= tmp[4]* src[4] + tmp[9]* src[5] + tmp[10]* src[6];
		dst[4]  = tmp[1]* src[1] + tmp[2]* src[2] + tmp[5]* src[3];
		dst[4] -= tmp[0]* src[1] + tmp[3]* src[2] + tmp[4]* src[3];
		dst[5]  = tmp[0]* src[0] + tmp[7]* src[2] + tmp[8]* src[3];
		dst[5] -= tmp[1]* src[0] + tmp[6]* src[2] + tmp[9]* src[3];
		dst[6]  = tmp[3]* src[0] + tmp[6]* src[1] + tmp[11]* src[3];
		dst[6] -= tmp[2]* src[0] + tmp[7]* src[1] + tmp[10]* src[3];
		dst[7]  = tmp[4]* src[0] + tmp[9]* src[1] + tmp[10]* src[2];
		dst[7] -= tmp[5]* src[0] + tmp[8]* src[1] + tmp[11]* src[2];
		
		/* calculate pairs for second 8 elements (cofactors) */
		tmp[0] = src[2]* src[7];
		tmp[1] = src[3]* src[6];
		tmp[2] = src[1]* src[7];
		tmp[3] = src[3]* src[5];
		tmp[4] = src[1]* src[6];
		tmp[5] = src[2]* src[5];
		tmp[6] = src[0]* src[7];
		tmp[7] = src[3]* src[4];
		tmp[8] = src[0]* src[6];
		tmp[9] = src[2]* src[4];
		tmp[10]= src[0]* src[5];
		tmp[11]= src[1]* src[4];
		
		/* calculate second 8 elements (cofactors) */
		dst[8]  = tmp[0]* src[13] + tmp[3]* src[14] + tmp[4]* src[15];
		dst[8] -= tmp[1]* src[13] + tmp[2]* src[14] + tmp[5]* src[15];
		dst[9]  = tmp[1]* src[12] + tmp[6]* src[14] + tmp[9]* src[15];
		dst[9] -= tmp[0]* src[12] + tmp[7]* src[14] + tmp[8]* src[15];
		dst[10] = tmp[2]* src[12] + tmp[7]* src[13] + tmp[10]* src[15];
		dst[10]-= tmp[3]* src[12] + tmp[6]* src[13] + tmp[11]* src[15];
		dst[11] = tmp[5]* src[12] + tmp[8]* src[13] + tmp[11]* src[14];
		dst[11]-= tmp[4]* src[12] + tmp[9]* src[13] + tmp[10]* src[14];
		dst[12] = tmp[2]* src[10] + tmp[5]* src[11] + tmp[1]* src[9];
		dst[12]-= tmp[4]* src[11] + tmp[0]* src[9]  + tmp[3]* src[10];
		dst[13] = tmp[8]* src[11] + tmp[0]* src[8]  + tmp[7]* src[10];
		dst[13]-= tmp[6]* src[10] + tmp[9]* src[11] + tmp[1]* src[8];
		dst[14] = tmp[6]* src[9]  + tmp[11]* src[11]+ tmp[3]* src[8];
		dst[14]-= tmp[10]* src[11]+ tmp[2]* src[8]  + tmp[7]* src[9];
		dst[15] = tmp[10]* src[10]+ tmp[4]* src[8]  + tmp[9]* src[9];
		dst[15]-= tmp[8]* src[9]  + tmp[11]* src[10]+ tmp[5]* src[8];
		
		/* calculate determinant */
		det = src[0]*dst[0] + src[1]*dst[1] + src[2]*dst[2] + src[3]*dst[3];
		
		if(det==0.0f)				return NULL; //singular matrix
		
		/* calculate matrix inverse */
		det = 1.0f/det;
		
		for (i=0; i<16; i++)
			dst[i] = dst[i]*det;
		
		return res;
	}
	
		
	void setPosition(float x, float y, float z)	{	m[12]=x; m[13]=y; m[14]=z;	}

	void setFrustumf2(float left, float right, float bottom, float top,  float znear, float zfar)
	{
		float temp, temp2, temp3, temp4;
		temp = 2.0f * znear;
		temp2 = right - left;
		temp3 = top - bottom;
		temp4 = zfar - znear;
		this->m[0] = temp / temp2;
		this->m[1] = 0.0;
		this->m[2] = 0.0;
		this->m[3] = 0.0;
		this->m[4] = 0.0;
		this->m[5] = temp / temp3;
		this->m[6] = 0.0;
		this->m[7] = 0.0;
		this->m[8] = (right + left) / temp2;
		this->m[9] = (top + bottom) / temp3;
		this->m[10] = (-zfar - znear) / temp4;
		this->m[11] = -1.0;
		this->m[12] = 0.0;
		this->m[13] = 0.0;
		this->m[14] = (-temp * zfar) / temp4;
		this->m[15] = 0.0;
	}
    
    void setOrtho(float left, float right, float bottom, float top, float nearVal, float farVal)
    {
        identity();
        
        float right_left=right-left;
        float top_bottom=top-bottom;
        float far_near=farVal-nearVal;
        
        float right_plus_left=right+left;
        float top_plus_bottom=top+bottom;
        float far_plus_near=farVal+nearVal;
        
        setScale(2.0f/right_left, 2.0f/top_bottom, -2.0f/far_near);
        setPosition(-(right_plus_left/right_left), -(top_plus_bottom/top_bottom), -(far_plus_near/far_near));
    }
};

#endif