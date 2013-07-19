#ifndef GXRENDERER_H
#define GXRENDERER_H

#include "../core/matrix4x4f.h"
#include "../core/vector2.h"

class gxRenderer
{
public:
	gxRenderer();
	~gxRenderer();

	void setViewPort(float x, float y, float cx, float cy);

	void setProjectionMatrixToGL(matrix4x4f* matrix);
	void setViewMatrixToGL(matrix4x4f* matrix);
	void setViewProjectionMatrix(matrix4x4f* matrix);

	const vector2f& getViewPortSz()			{	return m_cViewPortSz;					}

	matrix4x4f* getProjectionMatrix()		{	return m_pProjectionMatrixPtr;			}
	matrix4x4f* getViewMatrix()				{	return m_pViewMatrixPtr;				}
	matrix4x4f* getViewProjectionMatrix()	{	return m_pViewProjectionMatrixPtr;		}
	matrix4x4f* getOrthoProjectionMatrix()	{	return &m_cOrthogonalProjectionMatrix;	}

private:

	vector2f	m_cViewPortSz;
	matrix4x4f* m_pProjectionMatrixPtr;			//must not delete this pointer
	matrix4x4f* m_pViewMatrixPtr;				//must not delete this pointer
	matrix4x4f* m_pViewProjectionMatrixPtr;     //must not delete this pointer
    matrix4x4f  m_cOrthogonalProjectionMatrix;
};

#endif