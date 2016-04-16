#pragma once

#include "gxMath.h"
#include "basicIncludes.h"


class gxColor
{
public:
    static const gxColor RED;
    static const gxColor GREEN;
    static const gxColor BLUE;
    static const gxColor WHITE;
    static const gxColor BLACK;
    static const gxColor GREY;
    
    /*
    public static gxColor RED
    {
        return new gxColor (1.0f, 0.0f, 0.0f, 1.0f);
    }

    public static gxColor GREEN
    {
        get
        {
            return new gxColor (0.0f, 1.0f, 0.0f, 1.0f);
        }
    }

    public static gxColor BLUE
    {
        get
        {
            return new gxColor (0.0f, 0.0f, 1.0f, 1.0f);
        }
    }

    public static gxColor WHITE
    {
        get
        {
            return new gxColor (1.0f, 1.0f, 1.0f, 1.0f);
        }
    }

    public static gxColor BLACK
    {
        get
        {
            return new gxColor (0.0f, 0.0f, 0.0f, 1.0f);
        }
    }

    public static gxColor GREY
    {
        get
        {
            return new gxColor (0.5f, 0.5f, 0.5f, 1.0f);
        }
    }
     */

    gxColor() { r = g = b = a =0; }
    gxColor(float red, float green, float blue, float alpha) { r = red; g = green; b = blue; a = alpha; }
    gxColor(const gxColor& v) { r = v.r; g = v.g; b = v.b; a = v.a;	}
    gxColor(float* array)	{
        r = array [0];
        g = array [1];
        b = array [2];
        a = array [3];
    }

    void set(float red, float green, float blue, float alpha) { r = red; g = green; b = blue; a = alpha;	}

    float r, g, b, a;
};
