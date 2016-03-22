#pragma once

#include "basicIncludes.h"
#include "matrix4x4f.h"
#include "../util/gxFile.h"
#include "../autoReleasePool/autoReleasePool.h"

using namespace GEAR::Memory;

class object3d;
class DECLSPEC IAnimationTrack : public Ref
{
public:
	IAnimationTrack();
	virtual ~IAnimationTrack();

    virtual matrix4x4f* allocateFrames() = 0;

    void setName(const char* name)          {	GX_STRCPY(m_szName, name);          }
	void setFPS(int fps)                    {	animationFPS=fps;                   }
	void setTotalFrames(int nFrames)        {	numberOfFrames=nFrames;             }
    void setCurrentFrame(int currentFrame)  {   currentAnimationFrame=currentFrame; }

    const char* getName()           {	return m_szName;                }
	int getFPS()                    {	return animationFPS;            }
	int getTotalFrames()            {	return numberOfFrames;          }
    int getCurrentFrame()           {	return currentAnimationFrame;	}
    const matrix4x4f* getFrames()   {  return animationFrames;          }

	virtual void write(gxFile& file) = 0;
	virtual void read(gxFile& file) = 0;

    virtual bool getFrameFromTime(float time, matrix4x4f& mat) = 0;
    virtual bool getFrame(int frame, matrix4x4f& mat) = 0;
    virtual int getAnimationTrackType() = 0;

protected:
	int animationFPS;
	int numberOfFrames;
	int currentAnimationFrame;
	matrix4x4f* animationFrames;
	char m_szName[256];
};
#include "object3d.h"