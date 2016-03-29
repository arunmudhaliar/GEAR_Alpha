using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MonoGEAR
{
    public class vector4
    {
        public float x, y, z, w;

		public vector4() { x = y = z = w =0; }
		public vector4(float xx, float yy, float zz, float ww) { x = xx; y = yy; z = zz; w = ww; }
		public vector4(vector4 v) { x = v.x; y = v.y; z = v.z; w = v.w;	}
		public vector4(float[] array)	{
			x = array [0];
			y = array [1];
			z = array [2];
			w = array [3];
		}
		public static vector4 operator +(vector4 a, vector4 b) { return new vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
		public static vector4 operator -(vector4 a, vector4 b) { return new vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
		public static vector4 operator *(vector4 v, float n) { return new vector4(v.x * n, v.y * n, v.z * n, v.w * n); }
		public static vector4 operator /(vector4 v, float n) { return new vector4(v.x / n, v.y / n, v.z / n, v.w / n); }

		public void set(float xx, float yy, float zz, float ww) { x = xx; y = yy; z = zz; w = ww;	}
        public void zero() { x = y = z = w = 0; }

        public float lengthSquared()
        {
			return (x * x + y * y + z * z + w * w);
        }

        public float length() { return (float)Math.Sqrt((float)lengthSquared()); }
		public float dot(vector4 v) { return x * v.x + y * v.y + z * v.z + w * v.w; }

//        public vector3 cross(vector3 v)
//        {
//            vector3 res = new vector3();
//            res.x = y * v.z - z * v.y;
//            res.y = z * v.x - x * v.z;
//            res.z = x * v.y - y * v.x;
//            return res;
//        }

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
			w *= oon;

            return oon;
        }

		public float[] toArray()
		{
			return new float[] { x, y, z, w };
		}
    }
}