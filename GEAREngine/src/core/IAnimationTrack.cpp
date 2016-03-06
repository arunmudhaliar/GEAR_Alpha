#include "IAnimationTrack.h"

IAnimationTrack::IAnimationTrack()
{
	animationFPS=0;
	numberOfFrames=0;
	currentAnimationFrame=0;
	animationFrames=NULL;
}

IAnimationTrack::~IAnimationTrack()
{
	GX_DELETE_ARY(animationFrames);
}