#include "gxTweenFunctions.h"
#include "../core/gxMath.h"

// Linear
float gxTweenFunctions::linear(float time)
{
    return time;
}


// Sine Ease
float gxTweenFunctions::sineEaseIn(float time)
{
    return -1 * cosf(time * (float)M_PI_2) + 1;
}

float gxTweenFunctions::sineEaseOut(float time)
{
    return sinf(time * (float)M_PI_2);
}

float gxTweenFunctions::sineEaseInOut(float time)
{
    return -0.5f * (cosf((float)M_PI * time) - 1);
}


// Quad Ease
float gxTweenFunctions::quadEaseIn(float time)
{
    return time * time;
}

float gxTweenFunctions::quadEaseOut(float time)
{
    return -1 * time * (time - 2);
}

float gxTweenFunctions::quadEaseInOut(float time)
{
    time = time*2;
    if (time < 1)
        return 0.5f * time * time;
    --time;
    return -0.5f * (time * (time - 2) - 1);
}



// Cubic Ease
float gxTweenFunctions::cubicEaseIn(float time)
{
    return time * time * time;
}
float gxTweenFunctions::cubicEaseOut(float time)
{
    time -= 1;
    return (time * time * time + 1);
}
float gxTweenFunctions::cubicEaseInOut(float time)
{
    time = time*2;
    if (time < 1)
        return 0.5f * time * time * time;
    time -= 2;
    return 0.5f * (time * time * time + 2);
}


// Quart Ease
float gxTweenFunctions::quartEaseIn(float time)
{
    return time * time * time * time;
}

float gxTweenFunctions::quartEaseOut(float time)
{
    time -= 1;
    return -(time * time * time * time - 1);
}

float gxTweenFunctions::quartEaseInOut(float time)
{
    time = time*2;
    if (time < 1)
        return 0.5f * time * time * time * time;
    time -= 2;
    return -0.5f * (time * time * time * time - 2);
}


// Quint Ease
float gxTweenFunctions::quintEaseIn(float time)
{
    return time * time * time * time * time;
}

float gxTweenFunctions::quintEaseOut(float time)
{
    time -=1;
    return (time * time * time * time * time + 1);
}

float gxTweenFunctions::quintEaseInOut(float time)
{
    time = time*2;
    if (time < 1)
        return 0.5f * time * time * time * time * time;
    time -= 2;
    return 0.5f * (time * time * time * time * time + 2);
}


// Expo Ease
float gxTweenFunctions::expoEaseIn(float time)
{
    return time == 0 ? 0 : powf(2, 10 * (time/1 - 1)) - 1 * 0.001f;
}
float gxTweenFunctions::expoEaseOut(float time)
{
    return time == 1 ? 1 : (-powf(2, -10 * time / 1) + 1);
}
float gxTweenFunctions::expoEaseInOut(float time)
{
    time /= 0.5f;
    if (time < 1)
    {
        time = 0.5f * powf(2, 10 * (time - 1));
    }
    else
    {
        time = 0.5f * (-powf(2, -10 * (time - 1)) + 2);
    }
    
    return time;
}


// Circ Ease
float gxTweenFunctions::circEaseIn(float time)
{
    return -1 * (sqrt(1 - time * time) - 1);
}
float gxTweenFunctions::circEaseOut(float time)
{
    time = time - 1;
    return sqrt(1 - time * time);
}
float gxTweenFunctions::circEaseInOut(float time)
{
    time = time * 2;
    if (time < 1)
        return -0.5f * (sqrt(1 - time * time) - 1);
    time -= 2;
    return 0.5f * (sqrt(1 - time * time) + 1);
}


// Elastic Ease
float gxTweenFunctions::elasticEaseIn(float time, float period)
{
    
    float newT = 0;
    if (time == 0 || time == 1)
    {
        newT = time;
    }
    else
    {
        float s = period / 4;
        time = time - 1;
        newT = -powf(2, 10 * time) * sinf((time - s) * M_TWO_PI / period);
    }
    
    return newT;
}
float gxTweenFunctions::elasticEaseOut(float time, float period)
{
    
    float newT = 0;
    if (time == 0 || time == 1)
    {
        newT = time;
    }
    else
    {
        float s = period / 4;
        newT = powf(2, -10 * time) * sinf((time - s) * M_TWO_PI / period) + 1;
    }
    
    return newT;
}
float gxTweenFunctions::elasticEaseInOut(float time, float period)
{
    
    float newT = 0;
    if (time == 0 || time == 1)
    {
        newT = time;
    }
    else
    {
        time = time * 2;
        if (! period)
        {
            period = 0.3f * 1.5f;
        }
        
        float s = period / 4;
        
        time = time - 1;
        if (time < 0)
        {
            newT = -0.5f * powf(2, 10 * time) * sinf((time -s) * M_TWO_PI / period);
        }
        else
        {
            newT = powf(2, -10 * time) * sinf((time - s) * M_TWO_PI / period) * 0.5f + 1;
        }
    }
    return newT;
}


// Back Ease
float gxTweenFunctions::backEaseIn(float time)
{
    float overshoot = 1.70158f;
    return time * time * ((overshoot + 1) * time - overshoot);
}
float gxTweenFunctions::backEaseOut(float time)
{
    float overshoot = 1.70158f;
    
    time = time - 1;
    return time * time * ((overshoot + 1) * time + overshoot) + 1;
}
float gxTweenFunctions::backEaseInOut(float time)
{
    float overshoot = 1.70158f * 1.525f;
    
    time = time * 2;
    if (time < 1)
    {
        return (time * time * ((overshoot + 1) * time - overshoot)) / 2;
    }
    else
    {
        time = time - 2;
        return (time * time * ((overshoot + 1) * time + overshoot)) / 2 + 1;
    }
}



// Bounce Ease
float gxTweenFunctions::bounceTime(float time)
{
    if (time < 1 / 2.75)
    {
        return 7.5625f * time * time;
    }
    else if (time < 2 / 2.75)
    {
        time -= 1.5f / 2.75f;
        return 7.5625f * time * time + 0.75f;
    }
    else if(time < 2.5 / 2.75)
    {
        time -= 2.25f / 2.75f;
        return 7.5625f * time * time + 0.9375f;
    }
    
    time -= 2.625f / 2.75f;
    return 7.5625f * time * time + 0.984375f;
}
float gxTweenFunctions::bounceEaseIn(float time)
{
    return 1 - bounceTime(1 - time);
}

float gxTweenFunctions::bounceEaseOut(float time)
{
    return bounceTime(time);
}

float gxTweenFunctions::bounceEaseInOut(float time)
{
    float newT = 0;
    if (time < 0.5f)
    {
        time = time * 2;
        newT = (1 - bounceTime(1 - time)) * 0.5f;
    }
    else
    {
        newT = bounceTime(time * 2 - 1) * 0.5f + 0.5f;
    }
    
    return newT;
}


// Custom Ease
//float customEase(float time, float *easingParam)
//{
//    if (easingParam)
//    {
//        float tt = 1-time;
//        return easingParam[1]*tt*tt*tt + 3*easingParam[3]*time*tt*tt + 3*easingParam[5]*time*time*tt + easingParam[7]*time*time*time;
//    }
//    return time;
//}

float gxTweenFunctions::easeIn(float time, float rate)
{
    return powf(time, rate);
}

float gxTweenFunctions::easeOut(float time, float rate)
{
    return powf(time, 1 / rate);
}

float gxTweenFunctions::easeInOut(float time, float rate)
{
    time *= 2;
    if (time < 1)
    {
        return 0.5f * powf(time, rate);
    }
    else
    {
        return (1.0f - 0.5f * powf(2 - time, rate));
    }
}

float gxTweenFunctions::quadraticIn(float time)
{
    return   powf(time,2);
}

float gxTweenFunctions::quadraticOut(float time)
{
    return -time*(time-2);
}

float gxTweenFunctions::quadraticInOut(float time)
{
    
    float resultTime = time;
    time = time*2;
    if (time < 1)
    {
        resultTime = time * time * 0.5f;
    }
    else
    {
        --time;
        resultTime = -0.5f * (time * (time - 2) - 1);
    }
    return resultTime;
}

float gxTweenFunctions::bezieratFunction( float a, float b, float c, float d, float t )
{
    return (powf(1-t,3) * a + 3*t*(powf(1-t,2))*b + 3*powf(t,2)*(1-t)*c + powf(t,3)*d );
}
