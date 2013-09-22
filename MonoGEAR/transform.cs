using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MonoGEAR
{
    public class transform
    {
        protected IntPtr m_pObj3dPtr;

        public transform(IntPtr ptr)
        {
            m_pObj3dPtr = ptr;
        }

        protected IntPtr getHandle()
        {
            return m_pObj3dPtr;
        }
    }
}
