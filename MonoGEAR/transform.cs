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

        protected IntPtr m_pObj3dPtr;

        public transform(IntPtr ptr):
            base()
        {
            m_pObj3dPtr = ptr;
        }

        protected IntPtr getHandle()
        {
            return m_pObj3dPtr;
        }

        protected abstract void setHandle(IntPtr ptr);

        public void updatePositionf(float aXdt, float aYdt, float aZdt)
        {
            transform_updatePositionf(m_pObj3dPtr, aXdt, aYdt, aZdt);
        }

        public void updateLocalPositionf(float aXdt, float aYdt, float aZdt)
        {
            transform_updateLocalPositionf(m_pObj3dPtr, aXdt, aYdt, aZdt);
        }

        public void scaleX(float scale)
        {
            transform_scaleX(m_pObj3dPtr, scale);
        }

        public void scaleY(float scale)
        {
            transform_scaleY(m_pObj3dPtr, scale);
        }

        public void scaleZ(float scale)
        {
            transform_scaleZ(m_pObj3dPtr, scale);
        }

        public void rotateLocalXf(float aAngleInDeg)
        {
            transform_rotateLocalXf(m_pObj3dPtr, aAngleInDeg);
        }
        public void rotateLocalYf(float aAngleInDeg)
        {
            transform_rotateLocalYf(m_pObj3dPtr, aAngleInDeg);
        }
        public void rotateLocalZf(float aAngleInDeg)
        {
            transform_rotateLocalZf(m_pObj3dPtr, aAngleInDeg);
        }

        public void rotateWorldXf(float aAngleInDeg)
        {
            transform_rotateWorldXf(m_pObj3dPtr, aAngleInDeg);
        }
        public void rotateWorldYf(float aAngleInDeg)
        {
            transform_rotateWorldYf(m_pObj3dPtr, aAngleInDeg);
        }
        public void rotateWorldZf(float aAngleInDeg)
        {
            transform_rotateWorldZf(m_pObj3dPtr, aAngleInDeg);
        }

        public void setPosition(float x, float y, float z)
        {
            transform_setPosition(m_pObj3dPtr, x, y, z);
        }
        public float getX()
        {
            return transform_getX(m_pObj3dPtr);
        }
        public float getY()
        {
            return transform_getY(m_pObj3dPtr);
        }
        public float getZ()
        {
            return transform_getZ(m_pObj3dPtr);
        }

        public void copyTranfrom(transform t)
        {
            transform_copy(m_pObj3dPtr, t.getHandle());
        }
    }
}
