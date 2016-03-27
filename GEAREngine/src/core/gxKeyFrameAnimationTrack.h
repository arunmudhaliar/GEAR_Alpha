#pragma once

#include "IAnimationTrack.h"
#include <string>
#include <map>

class DECLSPEC gxKeyFrameAnimationTrack : public IAnimationTrack
{
public:
	~gxKeyFrameAnimationTrack();
    static gxKeyFrameAnimationTrack* create(const std::string& property);

    matrix4x4f* addFrame(int index);
    
	void write(gxFile& file);
	void read(gxFile& file);
    
    int getAnimationTrackType()     {   return 1;   }
    bool getFrame(int frame, matrix4x4f& mat);
    bool getFrameFromTime(float time, matrix4x4f& mat);
    
    bool isReadOnly()   {   return false;    }

private:
    gxKeyFrameAnimationTrack(){}
    gxKeyFrameAnimationTrack(const std::string& property);

    std::map<int, matrix4x4f*> keyFrames;
    std::string property;
};