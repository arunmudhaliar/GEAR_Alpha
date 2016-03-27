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