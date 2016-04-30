#include "gxKeyFrameAnimationTrack.h"

gxKeyFrameAnimationTrack::gxKeyFrameAnimationTrack(const std::string& property):
IAnimationTrack(),
property(property)
{
}

gxKeyFrameAnimationTrack::~gxKeyFrameAnimationTrack()
{
    for (auto frame : keyFrames)
    {
        GX_DELETE(frame.second);
    }
    
    keyFrames.clear();
}

gxKeyFrameAnimationTrack* gxKeyFrameAnimationTrack::create(const std::string& property)
{
    auto newObject = new gxKeyFrameAnimationTrack(property);
    if(newObject)
    {
        newObject->autoRelease();
        REF_RETAIN(newObject);
        
        return newObject;
    }
    
    return nullptr;
}

int gxKeyFrameAnimationTrack::getTotalTimeInMilliSec()
{
    assert(false);
    return 0;
}

float gxKeyFrameAnimationTrack::getTotalTimeInSec()
{
    assert(false);
    return 0;
}

void gxKeyFrameAnimationTrack::addFrame(int timeInMilliSec, gxTweenFunctions::TweenType tweenFunction, matrix4x4f* outMatrix)
{
    if(keyFrames.find(timeInMilliSec)!=keyFrames.end())
    {
        outMatrix = keyFrames[timeInMilliSec]->frame;
    }
    
    keyFrames[timeInMilliSec] = new stAnimationFrameKey();
    keyFrames[timeInMilliSec]->tweenFunction = tweenFunction;
    outMatrix = keyFrames[timeInMilliSec]->frame;
}

bool gxKeyFrameAnimationTrack::getFrame(int frame, matrix4x4f& mat)
{
    if(frame>=numberOfFrames)
        return false;
    
    assert(false);  //NOT IMPLEMENTED
    
    return true;
}

bool gxKeyFrameAnimationTrack::getFrameFromTime(float time, matrix4x4f& mat)
{
    assert(false);  //NOT IMPLEMENTED
    return false;
}

void gxKeyFrameAnimationTrack::write(gxFile& file)
{
    assert(false);  //NOT IMPLEMENTED
}

void gxKeyFrameAnimationTrack::read(gxFile& file)
{
    assert(false);  //NOT IMPLEMENTED
}