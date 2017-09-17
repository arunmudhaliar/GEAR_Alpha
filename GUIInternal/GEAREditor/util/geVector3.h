#pragma once
//
//  geVector3.h
//  Copyright 2017 GEAR3D
//  Gear3D
//
//  Created by Arun Mudaliar.
//
//
#include "pxMath.h"

template<class T>
class geVector3;
typedef geVector3<float> geVector3f;

template<class T>
class geVector3;
typedef geVector3<int> geVector3i;

template<class T>
class geVector3 {
public:
    T x, y, z;
    
    geVector3<T>()                              {   x = y = z = 0;                      }
    geVector3<T>(float xx, float yy, float zz)  {   x = xx; y = yy; z = zz;             }
    geVector3<T>(const geVector3<T>& v)         {   x = v.x; y = v.y; z = v.z;          }
    geVector3<T>(T *v)                          {   x = v[0]; y = v[1]; z = v[2];       }
    ~geVector3<T>(){}

    geVector3<T> operator+(const geVector3<T>& v)           {   return geVector3<T>(x+v.x, y+v.y, z+v.z);   }
    geVector3<T> operator-(const geVector3<T>& v)           {   return geVector3<T>(x-v.x, y-v.y, z-v.z);   }
    geVector3<T> operator*(const int n)                     {   return geVector3<T>(x*n, y*n, z*n);         }
    geVector3<T> operator*(const float n)                   {   return geVector3<T>(x*n, y*n, z*n);         }
    // geVector3<T> operator/(const short n) const             {   return geVector3<T>(x/n, y/n, z/n);         }
    // geVector3<T> operator/(const unsigned short n) const    {   return geVector3<T>(x/n, y/n, z/n);         }
    geVector3<T> operator/(const int n) const               {   return geVector3<T>(x/n, y/n, z/n);         }
    geVector3<T> operator/(const unsigned int n) const      {   return geVector3<T>(x/n, y/n, z/n);         }
    geVector3<T> operator/(const float n) const             {   return geVector3<T>(x/n, y/n, z/n);         }

    void set(T xx, T yy, T zz)          {   x = xx; y = yy; z = zz;                 }
    void zero()                         {   x = y = z = 0;                          }
    T lengthSquared()                   {   return (x * x + y * y + z * z);         }
    T length()                          {   return gxMath::SQRT(lengthSquared());   }
    T dot(const geVector3<T>& v)        {   return x * v.x + y * v.y + z * v.z;     }
    geVector3<T> cross(const geVector3<T>& v) {
        geVector3<T> res;
        res.x = y*v.z - z*v.y;
        res.y = z*v.x - x*v.z;
        res.z = x*v.y - y*v.x;
        return res;
    }

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
        return oon;
    }
};
