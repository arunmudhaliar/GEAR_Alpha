#pragma once

#include "gxKeyFrameAnimationTrack.h"

class DECLSPEC gxColorAnimationTrack : public gxKeyFrameAnimationTrack
{
public:
    static gxColorAnimationTrack* create(const std::string& property);

    virtual ~gxColorAnimationTrack();

    void addFrame(gxColor* source, int timeInMilliSec, gxTweenFunctions::TweenType tweenFunction=gxTweenFunctions::Tween_None, gxColor* outColor=nullptr);
	void write(gxFile& file);
	void read(gxFile& file);
    
    void applyKeyFrameAnimation();
    
    int getAnimationTrackType()     {   return 3;   }
    bool getFrame(int frame, gxColor& color);
    bool getFrameFromTime(int timeInMilliSec, gxColor& color);
    
private:
    gxColorAnimationTrack();
    gxColorAnimationTrack(const std::string& property);
};