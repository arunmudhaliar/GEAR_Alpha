#ifndef GXANIMATIONTRACK_H
#define GXANIMATIONTRACK_H

#include "basicIncludes.h"
#include "matrix4x4f.h"
//#include "object3d.h"

class object3d;
class gxAnimationTrack
{
public:
	gxAnimationTrack();
	~gxAnimationTrack();

	void setFPS(int fps)				{	m_iFPS=fps;			}
	void setTotalFrames(int nFrames)	{	m_nFrames=nFrames;	}

	int getFPS()			{	return m_iFPS;		}
	int getTotalFrames()	{	return m_nFrames;	}

	matrix4x4f* allocateTrack();

	matrix4x4f* getTrack()	{	return m_pTrack;	}

	void setCurrentFrame(int currentFrame)
	{
		m_iCurrentFrame=currentFrame;
	}

	int getCurrentFrame()	{	return m_iCurrentFrame;	}

	void setObject3d(object3d* obj);

private:
	int m_iFPS;
	int m_nFrames;
	int m_iCurrentFrame;
	matrix4x4f* m_pTrack;
	object3d* m_pObjectPtr;	//must not delete this pointer
};

#include "object3d.h"
#endif