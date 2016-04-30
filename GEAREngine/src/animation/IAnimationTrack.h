#pragma once

#include "../core/basicIncludes.h"
#include "../core/matrix4x4f.h"
#include "../util/gxFile.h"
#include "../autoReleasePool/autoReleasePool.h"
#include "../tween/gxTweenFunctions.h"
#include "../core/gxColor.h"
#include "../core/vector4.h"

using namespace GEAR::Memory;

class object3d;
class DECLSPEC IAnimationTrack : public Ref
{
public:
	IAnimationTrack();
	virtual ~IAnimationTrack();

    void setName(const char* name)          {	GX_STRCPY(m_szName, name);          }
	void setFPS(int fps)                    {	animationFPS=fps;                   }
	void setTotalFrames(int nFrames)        {	numberOfFrames=nFrames;             }
    void setCurrentFrame(int currentFrame)  {   currentAnimationFrame=currentFrame; }

    virtual int getAnimationTrackType() = 0;

    const char* getName()           {	return m_szName;                }
	int getFPS()                    {	return animationFPS;            }
	int getTotalFrames()            {	return numberOfFrames;          }
    int getCurrentFrame()           {	return currentAnimationFrame;	}

	virtual void write(gxFile& file) = 0;
	virtual void read(gxFile& file) = 0;

    virtual bool getFrameFromTime(float time, matrix4x4f& mat);
    virtual bool getFrameFromTime(float time, gxColor& clr);
    virtual bool getFrameFromTime(float time, vector3f& v);
    virtual bool getFrameFromTime(float time, vector4f& v);
    virtual bool getFrameFromTime(float time, float& v);
    
    virtual bool getFrame(int frame, matrix4x4f& mat);
    virtual bool getFrame(int frame, gxColor& clr);
    virtual bool getFrame(int frame, vector3f& v);
    virtual bool getFrame(int frame, vector4f& v);
    virtual bool getFrame(int frame, float& v);
    
    virtual void addFrame(int index, gxTweenFunctions::TweenType tweenFunction=gxTweenFunctions::Tween_None, matrix4x4f* outMatrix=nullptr);
    virtual void addFrame(int index, gxTweenFunctions::TweenType tweenFunction=gxTweenFunctions::Tween_None, gxColor* outColor=nullptr);
    virtual void addFrame(int index, gxTweenFunctions::TweenType tweenFunction=gxTweenFunctions::Tween_None, vector3f* outVec3=nullptr);
    virtual void addFrame(int index, gxTweenFunctions::TweenType tweenFunction=gxTweenFunctions::Tween_None, vector4f* outVec4=nullptr);
    virtual void addFrame(int index, gxTweenFunctions::TweenType tweenFunction=gxTweenFunctions::Tween_None, float* outFloat=nullptr);
    
    virtual void updateTweenFunction(int index, gxTweenFunctions::TweenType tweenFunction) = 0;
    
    virtual bool isReadOnly() = 0;
    
protected:
	int animationFPS;
	int numberOfFrames;
	int currentAnimationFrame;
	char m_szName[256];
};
#include "../core/object3d.h"