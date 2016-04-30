#pragma once

#include "IAnimationTrack.h"

class DECLSPEC gxAnimationTrack : public IAnimationTrack
{
public:
	~gxAnimationTrack();
    static gxAnimationTrack* create();
    
    matrix4x4f* allocateFrames();
    
    int getTotalTimeInMilliSec();
    float getTotalTimeInSec();

	void write(gxFile& file);
	void read(gxFile& file);
    
    int getAnimationTrackType()     {   return 1;   }
    bool getFrame(int frame, matrix4x4f& mat);
    bool getFrameFromTime(int timeInMilliSec, matrix4x4f& mat);

    bool isReadOnly()   {   return true;    }
    
private:
    gxAnimationTrack();
    matrix4x4f* animationFrames;
};