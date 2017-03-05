#pragma once

#include "IAnimationTrack.h"
#include <string>
#include <map>
#include "../tween/gxTweenFunctions.h"

struct stAnimationFrameKey
{
    enum EANIMATIONFRAMETYPE
    {
        MATRIX,
        COLOR,
        VECTOR3,
        VECTRO4,
        FLOAT,
        FRAMETYPE_MAX
    };
    
    stAnimationFrameKey(EANIMATIONFRAMETYPE type)
    {
        animatedfloatValue = 0.0f;
        time = 0.0f;
        frameType = type;
        tweenFunction = gxTweenFunctions::Tween_None;
        
        sourceMatrix        = nullptr;
        sourcefloat         = nullptr;
        sourceColorValue    = nullptr;
        sourceVector3Value  = nullptr;
        sourceVector4Value  = nullptr;
    }
    ~stAnimationFrameKey()
    {
    }
    
    //animated values
    matrix4x4f animatedMatrixValue;
    float      animatedfloatValue;
    gxColor    animatedColorValue;
    vector3f   animatedVector3Value;
    vector4f   animatedVector4Value;
    
    //input pointers
    matrix4x4f* sourceMatrix;
    float*      sourcefloat;
    gxColor*    sourceColorValue;
    vector3f*   sourceVector3Value;
    vector4f*   sourceVector4Value;

    //types & time
    EANIMATIONFRAMETYPE frameType;
    gxTweenFunctions::TweenType tweenFunction;
    float time;
};

class DECLSPEC gxKeyFrameAnimationTrack : public IAnimationTrack
{
public:
    bool isReadOnly()   {   return false;    }
    virtual void applyKeyFrameAnimation() = 0;

    int getTotalTimeInMilliSec();
    float getTotalTimeInSec();

protected:
    gxKeyFrameAnimationTrack();
    gxKeyFrameAnimationTrack(const std::string& property);
    virtual ~gxKeyFrameAnimationTrack();
    
    void resetKeyFrames();
    
    int totalTimeInMilliSec;
    std::map<int, stAnimationFrameKey*> keyFrames;
    std::string property;
};
