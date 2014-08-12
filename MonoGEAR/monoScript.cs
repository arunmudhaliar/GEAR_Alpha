using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MonoGEAR
{
    public class monoScript
    {
        protected IntPtr m_pMonoScriptPtr;
        protected object3d m_pObject3dPtr;

        private void setHandle(IntPtr pMonoScriptPtr, IntPtr pObjectPtr)
        {
            m_pMonoScriptPtr = pMonoScriptPtr;
            m_pObject3dPtr = new object3d(pObjectPtr);
        }

        protected virtual void initMonoScript()
        {
        }

        protected virtual void update()
        {
        }
    }
}
