#include "gxKeyFrameAnimationTrack.h"

gxKeyFrameAnimationTrack::gxKeyFrameAnimationTrack():
IAnimationTrack()
{
}

gxKeyFrameAnimationTrack::gxKeyFrameAnimationTrack(const std::string& property):
IAnimationTrack(),
property(property),
totalTimeInMilliSec(0)
{
}

gxKeyFrameAnimationTrack::~gxKeyFrameAnimationTrack()
{
    resetKeyFrames();
}

void gxKeyFrameAnimationTrack::resetKeyFrames()
{
    for (auto frame : keyFrames)
    {
        GX_DELETE(frame.second);
    }
    
    keyFrames.clear();
}

int gxKeyFrameAnimationTrack::getTotalTimeInMilliSec()
{
    return totalTimeInMilliSec;
}

float gxKeyFrameAnimationTrack::getTotalTimeInSec()
{
    return MILLISEC_TO_SEC(totalTimeInMilliSec);
}
