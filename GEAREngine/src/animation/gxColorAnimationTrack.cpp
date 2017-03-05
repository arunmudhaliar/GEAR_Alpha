#include "gxColorAnimationTrack.h"

gxColorAnimationTrack* gxColorAnimationTrack::create(const std::string& property)
{
    auto newObject = new gxColorAnimationTrack(property);
    if(newObject)
    {
        newObject->autoRelease();
        REF_RETAIN(newObject);
        
        return newObject;
    }
    
    return nullptr;
}

gxColorAnimationTrack::gxColorAnimationTrack():
gxKeyFrameAnimationTrack("")
{
}

gxColorAnimationTrack::gxColorAnimationTrack(const std::string& property):
gxKeyFrameAnimationTrack(property)
{
}

gxColorAnimationTrack::~gxColorAnimationTrack()
{
    
}

void gxColorAnimationTrack::addFrame(gxColor* source, int timeInMilliSec, gxTweenFunctions::TweenType tweenFunction, gxColor* outColor)
{
    if(keyFrames.find(timeInMilliSec)!=keyFrames.end())
    {
        outColor = &keyFrames[timeInMilliSec]->animatedColorValue;
    }
    
    keyFrames[timeInMilliSec] = new stAnimationFrameKey(stAnimationFrameKey::COLOR);
    keyFrames[timeInMilliSec]->tweenFunction = tweenFunction;
    outColor = &keyFrames[timeInMilliSec]->animatedColorValue;
}

void gxColorAnimationTrack::applyKeyFrameAnimation()
{
    assert(false);  //NOT IMPLEMENTED
}

bool gxColorAnimationTrack::getFrame(int frame, gxColor& color)
{
    if(frame>=numberOfFrames)
        return false;
    
    assert(false);  //NOT IMPLEMENTED
    
    return true;
}

bool gxColorAnimationTrack::getFrameFromTime(int timeInMilliSec, gxColor& color)
{
    if(keyFrames.find(timeInMilliSec)!=keyFrames.end())
    {
        color = keyFrames[timeInMilliSec]->animatedColorValue;
        return true;
    }
    return false;
}

void gxColorAnimationTrack::write(gxFile& file)
{
    file.Write((int)keyFrames.size());
    for(auto keyFrame : keyFrames)
    {
        file.Write(keyFrame.first);
        file.Write(keyFrame.second->animatedColorValue.r);
        file.Write(keyFrame.second->animatedColorValue.g);
        file.Write(keyFrame.second->animatedColorValue.b);
        file.Write(keyFrame.second->animatedColorValue.a);
    }
}

void gxColorAnimationTrack::read(gxFile& file)
{
    resetKeyFrames();
    
    int totalKeys = 0;
    file.Read(totalKeys);
    for (int x=0; x<totalKeys; x++)
    {
        int time=0;
        gxColor clr;
        file.Read(time);
        file.Read(clr.r);
        file.Read(clr.g);
        file.Read(clr.b);
        file.Read(clr.a);
        
        keyFrames[time] = new stAnimationFrameKey(stAnimationFrameKey::COLOR);
        keyFrames[time]->animatedColorValue = clr;
    }
}