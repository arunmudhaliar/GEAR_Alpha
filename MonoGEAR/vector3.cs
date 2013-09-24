using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MonoGEAR
{
    public class vector3
    {
        public float x, y, z;

        public vector3() { x = y = z = 0; }
        public vector3(float xx, float yy, float zz) { x = xx; y = yy; z = zz; }
        public vector3(vector3 v) { x = v.x; y = v.y; z = v.z; }

        public static vector3 operator +(vector3 a, vector3 b) { return new vector3(a.x + b.x, a.y + b.y, a.z + b.z); }
        public static vector3 operator -(vector3 a, vector3 b) { return new vector3(a.x - b.x, a.y - b.y, a.z - b.z); }
        public static vector3 operator *(vector3 v, float n) { return new vector3(v.x * n, v.y * n, v.z * n); }
        public static vector3 operator /(vector3 v, float n) { return new vector3(v.x / n, v.y / n, v.z / n); }

        public void set(float xx, float yy, float zz) { x = xx; y = yy; z = zz; }
        public void zero() { x = y = z = 0; }

        public float lengthSquared()
        {
            return (x * x + y * y + z * z);
        }

        public float length() { return (float)Math.Sqrt((float)lengthSquared()); }
        public float dot(vector3 v) { return x * v.x + y * v.y + z * v.z; }

        public vector3 cross(vector3 v)
        {
            vector3 res = new vector3();
            res.x = y * v.z - z * v.y;
            res.y = z * v.x - x * v.z;
            res.z = x * v.y - y * v.x;
            return res;
        }

        public float normalize()
        {
            float n, oon;
            n = lengthSquared();
            if (n == 0.0f || n == 1.0f) return 0.0f;

            n = (float)Math.Sqrt((float)n);
            oon = 1.0f;

            oon /= n;

            x *= oon;
            y *= oon;
            z *= oon;

            return oon;
        }
    }
}