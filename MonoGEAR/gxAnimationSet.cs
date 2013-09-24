using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace MonoGEAR
{
    public class gxAnimationSet
    {
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr gxAnimationSet_getAnimationName(IntPtr animSet);

        IntPtr m_pHandle;

        public gxAnimationSet(IntPtr handle)
        {
            m_pHandle = handle;
        }

        public IntPtr getHandle()
        {
            return m_pHandle;
        }

        public string name
        {
            get
            {
                return Marshal.PtrToStringAnsi(gxAnimationSet_getAnimationName(getHandle()));
            }
        }

    }
}
