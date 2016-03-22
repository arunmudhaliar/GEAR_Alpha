#pragma once

#include "IAnimationTrack.h"

class DECLSPEC gxAnimationTrack : public IAnimationTrack
{
public:
	gxAnimationTrack();
	~gxAnimationTrack();
    static gxAnimationTrack* create();
    
    matrix4x4f* allocateFrames();

	void write(gxFile& file);
	void read(gxFile& file);
    
    int getAnimationTrackType()     {   return 1;   }
    bool getFrame(int frame, matrix4x4f& mat);
    bool getFrameFromTime(float time, matrix4x4f& mat);
};