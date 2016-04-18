#pragma once

#include "IAnimationTrack.h"
#include <string>
#include <map>
#include "../tween/gxTweenFunctions.h"

struct stAnimationFrameKey
{
    stAnimationFrameKey()
    {
        frame = new matrix4x4f();
        tweenFunction = gxTweenFunctions::Tween_None;
    }
    ~stAnimationFrameKey()
    {
        GX_DELETE(frame);
    }
    
    matrix4x4f* frame;
    gxTweenFunctions::TweenType tweenFunction;
};

class DECLSPEC gxKeyFrameAnimationTrack : public IAnimationTrack
{
public:
	~gxKeyFrameAnimationTrack();
    static gxKeyFrameAnimationTrack* create(const std::string& property);

    matrix4x4f* addFrame(int index, gxTweenFunctions::TweenType tweenFunction=gxTweenFunctions::Tween_None);
    void updateTweenFunction(int index, gxTweenFunctions::TweenType tweenFunction);
    
	void write(gxFile& file);
	void read(gxFile& file);
    
    int getAnimationTrackType()     {   return 1;   }
    bool getFrame(int frame, matrix4x4f& mat);
    bool getFrameFromTime(float time, matrix4x4f& mat);
    
    bool isReadOnly()   {   return false;    }

private:
    gxKeyFrameAnimationTrack(){}
    gxKeyFrameAnimationTrack(const std::string& property);

    std::map<int, stAnimationFrameKey*> keyFrames;
    std::string property;
};