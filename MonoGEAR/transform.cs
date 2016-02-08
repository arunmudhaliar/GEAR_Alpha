using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace MonoGEAR
{
    public abstract class transform : object
    {
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void transform_updatePositionf(IntPtr t, float aXdt, float aYdt, float aZdt);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void transform_updateLocalPositionf(IntPtr t, float aXdt, float aYdt, float aZdt);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void transform_scaleX(IntPtr t, float scale);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void transform_scaleY(IntPtr t, float scale);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void transform_scaleZ(IntPtr t, float scale);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void transform_rotateLocalXf(IntPtr t, float aAngleInDeg);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void transform_rotateLocalYf(IntPtr t, float aAngleInDeg);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void transform_rotateLocalZf(IntPtr t, float aAngleInDeg);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void transform_rotateWorldXf(IntPtr t, float aAngleInDeg);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void transform_rotateWorldYf(IntPtr t, float aAngleInDeg);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void transform_rotateWorldZf(IntPtr t, float aAngleInDeg);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void transform_setPosition(IntPtr t, float x, float y, float z);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern float transform_getX(IntPtr t);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern float transform_getY(IntPtr t);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern float transform_getZ(IntPtr t);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void transform_copy(IntPtr a, IntPtr b);

        protected IntPtr handle;

		private transform():
			base()
		{
			Console.WriteLine("PRIVATE CONSTRUCTOR CALLED in transform");
		}

        public transform(IntPtr ptr):
            base()
        {
			handle = ptr;
			Console.WriteLine("transform::transform(IntPtr ptr) called "+handle);
        }

        protected IntPtr getHandle()
        {
			return handle;
        }

        protected abstract void setHandle(IntPtr ptr);

        public void updatePositionf(float aXdt, float aYdt, float aZdt)
        {
			transform_updatePositionf(handle, aXdt, aYdt, aZdt);
        }

        public void updateLocalPositionf(float aXdt, float aYdt, float aZdt)
        {
			transform_updateLocalPositionf(handle, aXdt, aYdt, aZdt);
        }

        public void scaleX(float scale)
        {
			transform_scaleX(handle, scale);
        }

        public void scaleY(float scale)
        {
			transform_scaleY(handle, scale);
        }

        public void scaleZ(float scale)
        {
			transform_scaleZ(handle, scale);
        }

        public void rotateLocalXf(float aAngleInDeg)
        {
			transform_rotateLocalXf(handle, aAngleInDeg);
        }
        public void rotateLocalYf(float aAngleInDeg)
        {
			transform_rotateLocalYf(handle, aAngleInDeg);
        }
        public void rotateLocalZf(float aAngleInDeg)
        {
			transform_rotateLocalZf(handle, aAngleInDeg);
        }

        public void rotateWorldXf(float aAngleInDeg)
        {
			transform_rotateWorldXf(handle, aAngleInDeg);
        }
        public void rotateWorldYf(float aAngleInDeg)
        {
			transform_rotateWorldYf(handle, aAngleInDeg);
        }
        public void rotateWorldZf(float aAngleInDeg)
        {
			transform_rotateWorldZf(handle, aAngleInDeg);
        }

        public void setPosition(float x, float y, float z)
        {
			transform_setPosition(handle, x, y, z);
        }
        public float getX()
        {
			return transform_getX(handle);
        }
        public float getY()
        {
			return transform_getY(handle);
        }
        public float getZ()
        {
			return transform_getZ(handle);
        }

        public void copyTranfrom(transform t)
        {
			transform_copy(handle, t.getHandle());
        }
    }
}
