using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MonoGEAR
{
    public class gxMath
    {
        public static double M_PI = 3.1415926535897932384626433832795f;    // matches value in gcc v2 math.h

        public static double M_TWO_PI = 6.283185307179586476925286766559f;
        public static double M_180_PI = 57.295779513082320876798154814105f;
        public static double M_PI_180 = 0.017453292519943295769236907684886f;

        public static float DEG2RAD(float a) { return a*(float)M_PI_180;   }
        public static float RAD2DEG(float a) {return a*(float)M_180_PI;    }

        public static float SINF(int ang)
        {
            return (float)Math.Sin(DEG2RAD(ang));
        }

        public static float COSF(int ang)
        {
            return (float)Math.Cos(DEG2RAD(ang));
        }

        public static float SINF(float ang)
        {
            return (float)Math.Sin(DEG2RAD(ang));
        }

        public static float COSF(float ang)
        {
            return (float)Math.Cos(DEG2RAD(ang));
        }

        public static float SQRT(float val)
        {
            return (float)Math.Sqrt(val);
        }

        public static int SQRT(int val)
        {
            return (int)Math.Sqrt((float)val);
        }
    }
}
