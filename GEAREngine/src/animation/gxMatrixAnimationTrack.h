#pragma once

#include "gxKeyFrameAnimationTrack.h"

class DECLSPEC gxMatrixAnimationTrack : public gxKeyFrameAnimationTrack
{
public:
    static gxMatrixAnimationTrack* create(const std::string& property);

    virtual ~gxMatrixAnimationTrack();

    void addFrame(matrix4x4f* source, int timeInMilliSec, gxTweenFunctions::TweenType tweenFunction=gxTweenFunctions::Tween_None, matrix4x4f* outMatrix=nullptr);
	void write(gxFile& file);
	void read(gxFile& file);
    
    void applyKeyFrameAnimation();

    int getAnimationTrackType()     {   return 2;   }
    bool getFrame(int frame, matrix4x4f& mat);
    bool getFrameFromTime(float time, matrix4x4f& mat);
    
private:
    gxMatrixAnimationTrack();
    gxMatrixAnimationTrack(const std::string& property);
};