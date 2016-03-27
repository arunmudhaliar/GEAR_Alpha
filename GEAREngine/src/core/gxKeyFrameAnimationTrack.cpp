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

matrix4x4f* gxKeyFrameAnimationTrack::addFrame(int index)
{
    if(keyFrames.find(index)!=keyFrames.end())
    {
        return keyFrames[index];
    }
    
    keyFrames[index] = new matrix4x4f();
    return keyFrames[index];
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