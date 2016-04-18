#pragma once

#include "IAnimationTrack.h"

class DECLSPEC gxAnimationTrack : public IAnimationTrack
{
public:
	~gxAnimationTrack();
    static gxAnimationTrack* create();
    
    matrix4x4f* allocateFrames();

	void write(gxFile& file);
	void read(gxFile& file);
    
    int getAnimationTrackType()     {   return 1;   }
    bool getFrame(int frame, matrix4x4f& mat);
    bool getFrameFromTime(float time, matrix4x4f& mat);
    
    matrix4x4f* addFrame(int index, gxTweenFunctions::TweenType tweenFunction=gxTweenFunctions::Tween_None);
    void updateTweenFunction(int index, gxTweenFunctions::TweenType tweenFunction);

    bool isReadOnly()   {   return true;    }
    
private:
    gxAnimationTrack();
    matrix4x4f* animationFrames;
};