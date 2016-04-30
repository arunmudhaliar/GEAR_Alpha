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

bool IAnimationTrack::getFrameFromTime(float time, matrix4x4f& mat)
{
    assert(false);
    return false;
}

bool IAnimationTrack::getFrameFromTime(float time, gxColor& clr)
{
    assert(false);
    return false;
}

bool IAnimationTrack::getFrameFromTime(float time, vector3f& v)
{
    assert(false);
    return false;
}

bool IAnimationTrack::getFrameFromTime(float time, vector4f& v)
{
    assert(false);
    return false;
}

bool IAnimationTrack::getFrameFromTime(float time, float& v)
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

void IAnimationTrack::addFrame(int index, gxTweenFunctions::TweenType tweenFunction, matrix4x4f* outMatrix)
{
    assert(false);
}

void IAnimationTrack::addFrame(int index, gxTweenFunctions::TweenType tweenFunction, gxColor* outColor)
{
    assert(false);
}

void IAnimationTrack::addFrame(int index, gxTweenFunctions::TweenType tweenFunction, vector3f* outVec3)
{
    assert(false);
}

void IAnimationTrack::addFrame(int index, gxTweenFunctions::TweenType tweenFunction, vector4f* outVec4)
{
    assert(false);
}

void IAnimationTrack::addFrame(int index, gxTweenFunctions::TweenType tweenFunction, float* outFloat)
{
    assert(false);
}
