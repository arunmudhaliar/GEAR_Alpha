#pragma once

#include "matrix4x4f.h"
#include "vector3.h"

class DECLSPEC transform : public matrix4x4f
{
public:
	transform():
        matrix4x4f()
	{
	}

	virtual ~transform()
	{
	}

    void copyTransform(transform& tm)
    {
        worldTransformationMatrix.copyMatrix(*tm.getWorldMatrix());
        copyMatrix(tm); //Note: copymatrix will internally call transformationChangedf()
    }
    
	void updatePositionf(vector3f v)
	{
		updatePositionf(v.x, v.y, v.z);
	}

	void updatePositionf(float aXdt, float aYdt, float aZdt)
	{
		m[12]	+= aXdt;
		m[13]	+= aYdt;
		m[14]	+= aZdt;
		
		transformationChangedf();
	}
	
	void updateLocalPositionf(vector3f v)
	{
		updateLocalPositionf(v.x, v.y, v.z);
	}

	void updateLocalPositionf(float aXdt, float aYdt, float aZdt)
	{
		if(aXdt!=0)	translateLocalXf(aXdt);
		if(aYdt!=0)	translateLocalYf(aYdt);
		if(aZdt!=0)	translateLocalZf(aZdt);
		
		transformationChangedf();
	}
	
	void scale(float scale)
    {
        matrix4x4f scaleTM;

        scaleTM.setScale(scale, scale, scale);
        //multiply with transform matrix
		matrix4x4f* p=this;
		*p = (scaleTM * (*this));   
    }

	void scaleX(float scale)
    {
        matrix4x4f scaleTM;

        scaleTM.setScale(scale, m[5], m[10]);
        //multiply with transform matrix
		matrix4x4f* p=this;
		*p = (scaleTM * (*this)); 
    }
	
	void scaleY(float scale)
    {
        matrix4x4f scaleTM;

        scaleTM.setScale(m[0], scale, m[10]);
        //multiply with transform matrix
		matrix4x4f* p=this;
		*p = (scaleTM * (*this)); 
    }

	void scaleZ(float scale)
    {
        matrix4x4f scaleTM;

        scaleTM.setScale(m[0], m[5], scale);
        //multiply with transform matrix
		matrix4x4f* p=this;
		*p = (scaleTM * (*this)); 
    }
    
	void rotateWorldXf(float aAngleInDeg)
	{
		matrix4x4f aRotMat;
		
		aRotMat.setRotationMatrix(aAngleInDeg, true, false, false);
		//multiply with transform matrix
		matrix4x4f* p=this;
		*p = (aRotMat * (*this));
	}
	
	void rotateWorldYf(float aAngleInDeg)
	{
		matrix4x4f aRotMat;
		
		aRotMat.setRotationMatrix(aAngleInDeg, false, true, false);
		//multiply with transform matrix
		matrix4x4f* p=this;
		*p = (aRotMat * (*this));
	}
	
	void rotateWorldZf(float aAngleInDeg)
	{
		matrix4x4f aRotMat;
		
		aRotMat.setRotationMatrix(aAngleInDeg, false, false, true);
		//multiply with transform matrix
		matrix4x4f* p=this;
		*p = (aRotMat * (*this));
	}
	
	void rotateLocalXf(float aAngleInDeg)
	{
		matrix4x4f	aRotMat;
		
		aRotMat.setRotationMatrix(aAngleInDeg, getXAxis());
		vector3f	aTransPoint;
		aTransPoint.x=m[12];
		aTransPoint.y=m[13];
		aTransPoint.z=m[14];
		
		aRotMat.setPosition(-aTransPoint.x, -aTransPoint.y, -aTransPoint.z);
		
		//multiply with transform matrix
		matrix4x4f* p=this;
		*p = (aRotMat * (*this));
		
		m[12]=aTransPoint.x;
		m[13]=aTransPoint.y;
		m[14]=aTransPoint.z;
		
		transformationChangedf();
	}
	
	void rotateLocalYf(float aAngleInDeg)
	{
		matrix4x4f	aRotMat;
		
		aRotMat.setRotationMatrix(aAngleInDeg, getYAxis());
		vector3f	aTransPoint;
		aTransPoint.x=m[12];
		aTransPoint.y=m[13];
		aTransPoint.z=m[14];
		
		aRotMat.setPosition(-aTransPoint.x, -aTransPoint.y, -aTransPoint.z);
		
		//multiply with transform matrix
		matrix4x4f* p=this;
		*p = (aRotMat * (*this));
		
		m[12]=aTransPoint.x;
		m[13]=aTransPoint.y;
		m[14]=aTransPoint.z;
		
		transformationChangedf();
	}
	
	void rotateLocalZf(float aAngleInDeg)
	{
		matrix4x4f	aRotMat;
		
		aRotMat.setRotationMatrix(aAngleInDeg, getZAxis());
		vector3f	aTransPoint;
		aTransPoint.x=m[12];
		aTransPoint.y=m[13];
		aTransPoint.z=m[14];
		
		aRotMat.setPosition(-aTransPoint.x, -aTransPoint.y, -aTransPoint.z);
		
		//multiply with transform matrix
		matrix4x4f* p=this;
		*p = (aRotMat * (*this));
		
		m[12]=aTransPoint.x;
		m[13]=aTransPoint.y;
		m[14]=aTransPoint.z;
		
		transformationChangedf();
	}
	
	void rotateArb(float aAngleInDeg, float* aAxis, vector3f aPos)
	{
		matrix4x4f	aRotMat;
		aRotMat.setRotationMatrix(aAngleInDeg, &aAxis[0]);
		
		vector3f	aTransPoint;
		m[12]-=aPos.x;
		m[13]-=aPos.y;
		m[14]-=aPos.z;
		
		//multiply with transform matrix
		matrix4x4f* p=this;
		*p = (aRotMat * (*this));
		
		m[12]+=aPos.x;
		m[13]+=aPos.y;
		m[14]+=aPos.z;
		
		transformationChangedf();
	}

	virtual void transformationChangedf()=0;

	matrix4x4f* getWorldMatrix()	{	return &worldTransformationMatrix;	}

protected:
    void translateLocalXf(float aXdt)
    {
        m[12] += (aXdt* m[0]);
        m[13] += (aXdt* m[1]);
        m[14] += (aXdt* m[2]);
    }
    
    void translateLocalYf(float aYdt)
    {
        m[12] += (aYdt* m[4]);
        m[13] += (aYdt* m[5]);
        m[14] += (aYdt* m[6]);
    }
    void translateLocalZf(float aZdt)
    {
        m[12] += (aZdt* m[8]);
        m[13] += (aZdt* m[9]);
        m[14] += (aZdt* m[10]);
    }
    
    void matrixChanged()
    {
        transformationChangedf();
    }
    
    matrix4x4f worldTransformationMatrix;	//object-space to world transform
};

extern "C" {
	DECLSPEC void transform_updatePositionf(transform* t, float aXdt, float aYdt, float aZdt);
	DECLSPEC void transform_updateLocalPositionf(transform* t, float aXdt, float aYdt, float aZdt);
	DECLSPEC void transform_scaleX(transform* t, float scale);
	DECLSPEC void transform_scaleY(transform* t, float scale);
	DECLSPEC void transform_scaleZ(transform* t, float scale);
	DECLSPEC void transform_rotateLocalXf(transform* t, float aAngleInDeg);
	DECLSPEC void transform_rotateLocalYf(transform* t, float aAngleInDeg);
	DECLSPEC void transform_rotateLocalZf(transform* t, float aAngleInDeg);
	DECLSPEC void transform_rotateWorldXf(transform* t, float aAngleInDeg);
	DECLSPEC void transform_rotateWorldYf(transform* t, float aAngleInDeg);
	DECLSPEC void transform_rotateWorldZf(transform* t, float aAngleInDeg);
	DECLSPEC void transform_setPosition(transform* t, float x, float y, float z);
	DECLSPEC float transform_getX(transform* t);
	DECLSPEC float transform_getY(transform* t);
	DECLSPEC float transform_getZ(transform* t);
	DECLSPEC void transform_copy(transform* a, transform* b);
}