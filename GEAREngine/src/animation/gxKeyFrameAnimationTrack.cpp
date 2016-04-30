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

void gxKeyFrameAnimationTrack::addFrame(int index, gxTweenFunctions::TweenType tweenFunction, matrix4x4f* outMatrix)
{
    if(keyFrames.find(index)!=keyFrames.end())
    {
        outMatrix = keyFrames[index]->frame;
    }
    
    keyFrames[index] = new stAnimationFrameKey();
    keyFrames[index]->tweenFunction = tweenFunction;
    outMatrix = keyFrames[index]->frame;
}

void gxKeyFrameAnimationTrack::updateTweenFunction(int index, gxTweenFunctions::TweenType tweenFunction)
{
    if(keyFrames.find(index)!=keyFrames.end())
    {
        keyFrames[index]->tweenFunction=tweenFunction;
    }
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