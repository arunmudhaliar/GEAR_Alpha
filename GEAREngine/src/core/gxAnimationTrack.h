#pragma once

#include "basicIncludes.h"
#include "matrix4x4f.h"
#include "../util/gxFile.h"

class object3d;
class DECLSPEC gxAnimationTrack
{
public:
	gxAnimationTrack();
	~gxAnimationTrack();

	void setFPS(int fps)				{	animationFPS=fps;			}
	void setTotalFrames(int nFrames)	{	numberOfFrames=nFrames;	}

	int getFPS()			{	return animationFPS;		}
	int getTotalFrames()	{	return numberOfFrames;	}

	matrix4x4f* allocateTrack();

	matrix4x4f* getTrack()	{	return animationTrack;	}

	void setCurrentFrame(int currentFrame)
	{
		currentAnimationFrame=currentFrame;
	}

	int getCurrentFrame()	{	return currentAnimationFrame;	}

	//void setObject3d(object3d* obj);

	void setName(const char* name)	{	GX_STRCPY(m_szName, name);	}
	const char* getName()			{	return m_szName;			}

	void write(gxFile& file);
	void read(gxFile& file);

private:
	int animationFPS;
	int numberOfFrames;
	int currentAnimationFrame;
	matrix4x4f* animationTrack;
	//object3d* m_pObjectPtr;	//must not delete this pointer
	char m_szName[256];
};

#include "object3d.h"