using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using MonoGEAR;

namespace MonoGEAR
{
    public sealed class object3d
    {
        private IntPtr m_pObj3dPtr;

        public object3d(IntPtr ptr)
        {
            m_pObj3dPtr = ptr;
        }

        public static object3d create(string name)
        {
            return new object3d(MonoGEAREntryPointClass.engine_loadAndAppendFBX(MonoGEAREntryPointClass.engine_getWorld(0), name));
        }
    }
}
