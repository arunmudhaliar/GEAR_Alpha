#include "gxMatrixAnimationTrack.h"

gxMatrixAnimationTrack* gxMatrixAnimationTrack::create(const std::string& property)
{
    auto newObject = new gxMatrixAnimationTrack(property);
    if(newObject)
    {
        newObject->autoRelease();
        REF_RETAIN(newObject);
        
        return newObject;
    }
    
    return nullptr;
}

gxMatrixAnimationTrack::gxMatrixAnimationTrack():
gxKeyFrameAnimationTrack("")
{
}

gxMatrixAnimationTrack::gxMatrixAnimationTrack(const std::string& property):
gxKeyFrameAnimationTrack(property)
{
}

gxMatrixAnimationTrack::~gxMatrixAnimationTrack()
{
    
}

void gxMatrixAnimationTrack::addFrame(matrix4x4f* source, int timeInMilliSec, gxTweenFunctions::TweenType tweenFunction, matrix4x4f* outMatrix)
{
    if(keyFrames.find(timeInMilliSec)!=keyFrames.end())
    {
        outMatrix = &keyFrames[timeInMilliSec]->animatedMatrixValue;
    }
    
    keyFrames[timeInMilliSec] = new stAnimationFrameKey(stAnimationFrameKey::MATRIX);
    keyFrames[timeInMilliSec]->tweenFunction = tweenFunction;
    outMatrix = &keyFrames[timeInMilliSec]->animatedMatrixValue;
}

void gxMatrixAnimationTrack::applyKeyFrameAnimation()
{
    assert(false);  //NOT IMPLEMENTED
}

bool gxMatrixAnimationTrack::getFrame(int frame, matrix4x4f& mat)
{
    if(frame>=numberOfFrames)
        return false;
    
    assert(false);  //NOT IMPLEMENTED
    
    return true;
}

bool gxMatrixAnimationTrack::getFrameFromTime(float time, matrix4x4f& mat)
{
    assert(false);  //NOT IMPLEMENTED
    return false;
}

void gxMatrixAnimationTrack::write(gxFile& file)
{
    assert(false);  //NOT IMPLEMENTED
}

void gxMatrixAnimationTrack::read(gxFile& file)
{
    assert(false);  //NOT IMPLEMENTED
}