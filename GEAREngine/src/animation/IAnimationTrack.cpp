#include "IAnimationTrack.h"

IAnimationTrack::IAnimationTrack():
    Ref()
{
	animationFPS=0;
	numberOfFrames=0;
	currentAnimationFrame=0;
}

IAnimationTrack::~IAnimationTrack()
{
}

bool IAnimationTrack::getFrameFromTime(int timeInMilliSec, matrix4x4f& mat)
{
    assert(false);
    return false;
}

bool IAnimationTrack::getFrameFromTime(int timeInMilliSec, gxColor& clr)
{
    assert(false);
    return false;
}

bool IAnimationTrack::getFrameFromTime(int timeInMilliSec, vector3f& v)
{
    assert(false);
    return false;
}

bool IAnimationTrack::getFrameFromTime(int timeInMilliSec, vector4f& v)
{
    assert(false);
    return false;
}

bool IAnimationTrack::getFrameFromTime(int timeInMilliSec, float& v)
{
    assert(false);
    return false;
}

bool IAnimationTrack::getFrame(int frame, matrix4x4f& mat)
{
    assert(false);
    return false;
}

bool IAnimationTrack::getFrame(int frame, gxColor& clr)
{
    assert(false);
    return false;
}

bool IAnimationTrack::getFrame(int frame, vector3f& v)
{
    assert(false);
    return false;
}

bool IAnimationTrack::getFrame(int frame, vector4f& v)
{
    assert(false);
    return false;
}

bool IAnimationTrack::getFrame(int frame, float& v)
{
    assert(false);
    return false;
}

void IAnimationTrack::addFrame(int timeInMilliSec, gxTweenFunctions::TweenType tweenFunction, matrix4x4f* outMatrix)
{
    assert(false);
}

void IAnimationTrack::addFrame(int timeInMilliSec, gxTweenFunctions::TweenType tweenFunction, gxColor* outColor)
{
    assert(false);
}

void IAnimationTrack::addFrame(int timeInMilliSec, gxTweenFunctions::TweenType tweenFunction, vector3f* outVec3)
{
    assert(false);
}

void IAnimationTrack::addFrame(int timeInMilliSec, gxTweenFunctions::TweenType tweenFunction, vector4f* outVec4)
{
    assert(false);
}

void IAnimationTrack::addFrame(int timeInMilliSec, gxTweenFunctions::TweenType tweenFunction, float* outFloat)
{
    assert(false);
}

void IAnimationTrack::updateTweenFunction(int index, gxTweenFunctions::TweenType tweenFunction)
{
    
}