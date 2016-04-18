#pragma once

class gxTweenFunctions
{
public:
    enum TweenType
    {
        Tween_None,
        
        Tween_Linear,
        
        Tween_Sine_EaseIn,
        Tween_Sine_EaseOut,
        Tween_Sine_EaseInOut,
        
        Tween_Quad_EaseIn,
        Tween_Quad_EaseOut,
        Tween_Quad_EaseInOut,
        
        Tween_Cubic_EaseIn,
        Tween_Cubic_EaseOut,
        Tween_Cubic_EaseInOut,
        
        Tween_Quart_EaseIn,
        Tween_Quart_EaseOut,
        Tween_Quart_EaseInOut,
        
        Tween_Quint_EaseIn,
        Tween_Quint_EaseOut,
        Tween_Quint_EaseInOut,
        
        Tween_Expo_EaseIn,
        Tween_Expo_EaseOut,
        Tween_Expo_EaseInOut,
        
        Tween_Circ_EaseIn,
        Tween_Circ_EaseOut,
        Tween_Circ_EaseInOut,
        
        Tween_Elastic_EaseIn,
        Tween_Elastic_EaseOut,
        Tween_Elastic_EaseInOut,
        
        Tween_Back_EaseIn,
        Tween_Back_EaseOut,
        Tween_Back_EaseInOut,
        
        Tween_Bounce_EaseIn,
        Tween_Bounce_EaseOut,
        Tween_Bounce_EaseInOut,
        
        Tween_MAX
    };

    gxTweenFunctions(){}
    ~gxTweenFunctions(){}
    
    float easeIn(float time, float rate);
    float easeOut(float time, float rate);
    float easeInOut(float time, float rate);
    float bezieratFunction( float a, float b, float c, float d, float t );
    float quadraticIn(float time);
    float quadraticOut(float time);
    float quadraticInOut(float time);
    //float quadraticInOut(float time);
    //float tweenTo(float time, TweenType type, float *easingParam);
    float linear(float time);
    float sineEaseIn(float time);
    float sineEaseOut(float time);
    float sineEaseInOut(float time);
    float quadEaseIn(float time);
    float quadEaseOut(float time);
    float quadEaseInOut(float time);
    float cubicEaseIn(float time);
    float cubicEaseOut(float time);
    float cubicEaseInOut(float time);
    float quartEaseIn(float time);
    float quartEaseOut(float time);
    float quartEaseInOut(float time);
    float quintEaseIn(float time);
    float quintEaseOut(float time);
    float quintEaseInOut(float time);
    float expoEaseIn(float time);
    float expoEaseOut(float time);
    float expoEaseInOut(float time);
    float circEaseIn(float time);
    float circEaseOut(float time);
    float circEaseInOut(float time);
    float elasticEaseIn(float time, float period);
    float elasticEaseOut(float time, float period);
    float elasticEaseInOut(float time, float period);
    float backEaseIn(float time);
    float backEaseOut(float time);
    float bounceTime(float time);
    float backEaseInOut(float time);
    float bounceEaseIn(float time);
    float bounceEaseOut(float time);
    float bounceEaseInOut(float time);
};
