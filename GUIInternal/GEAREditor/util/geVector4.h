#pragma once
//
//  geVector4.h
//  Copyright 2017 GEAR3D
//  Gear3D
//
//  Created by Arun Mudaliar.
//
//
#include "pxMath.h"

template<class T>
class geVector4;
typedef geVector4<float> geVector4f;

template<class T>
class geVector4;
typedef geVector4<int> geVector4i;

template<class T>
class geVector4 {
public:
    T x, y, z, w;

    geVector4<T>()                                          {   x = y = z = w = 0;                      }
    geVector4<T>(int xx, int yy, int zz, int ww)            {   x = xx; y = yy; z = zz; w = ww;         }
    geVector4<T>(float xx, float yy, float zz, float ww)    {   x = xx; y = yy; z = zz; w = ww;         }
    geVector4<T>(const geVector4<T>& v)                     {   x = v.x; y = v.y; z = v.z; w = v.w;     }
    geVector4<T>(T *v)                                      {   x = v[0]; y = v[1]; z = v[2]; w = v[3]; }
    ~geVector4<T>(){}

    geVector4<T> operator+( geVector4<T>& v)            {   return geVector4<T>(x+v.x, y+v.y, z+v.z, w+v.w);    }
    geVector4<T> operator-( geVector4<T>& v)            {   return geVector4<T>(x-v.x, y-v.y, z-v.z, w-v.w);    }
    geVector4<T> operator*(const int n)                 {   return geVector4<T>(x*n, y*n, z*n, w*n);            }
    geVector4<T> operator*(const float n)               {   return geVector4<T>(x*n, y*n, z*n, w*n);            }
//    geVector4<T> operator/(const short n) const         {   return geVector4<T>(x/n, y/n, z/n, w/n);            }
//    geVector4<T> operator/(const unsigned short n) const{   return geVector4<T>(x/n, y/n, z/n, w/n);            }
    geVector4<T> operator/(const int n) const           {   return geVector4<T>(x/n, y/n, z/n, w/n);            }
    geVector4<T> operator/(const unsigned int n) const  {   return geVector4<T>(x/n, y/n, z/n, w/n);            }
    geVector4<T> operator/(const float n) const         {   return geVector4<T>(x/n, y/n, z/n, w/n);            }

    void set(T xx, T yy, T zz, T ww)        {   x = xx; y = yy; z = zz; w = ww;                 }
    void zero()                             {   x = y = z = w = 0;                              }
    T lengthSquared()                       {   return (x * x + y * y + z * z + w * w);         }
    T length()                              {   return pxMath::SQRT(lengthSquared());           }
    T dot(const geVector4<T>& v)            {   return x * v.x + y * v.y + z * v.z + w * v.w;   }
    T normalize() {
        T n, oon;
        n = lengthSquared();
        if (n == 0.0f || n == 1.0f) return 0.0f;
        n = gxMath::SQRT(n);
        oon = 1.0f;
        oon /= n;
        x *= oon;
        y *= oon;
        z *= oon;
        w *= oon;
        return oon;
    }
};
