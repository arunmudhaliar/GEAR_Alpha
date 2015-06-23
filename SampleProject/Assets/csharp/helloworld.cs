using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;
using MonoGEAR;

namespace helloworldNS
{
    class hhh
    {
    }

    public class helloworld : monoScript
    {
        object3d m_pObject3D;
        protected override void initMonoScript()
        {
            MonoGEAREntryPointClass.engine_consoleLog("HELLOWORLD CREATED", 0);
            //m_pObject3D = new object3d("hello");
        }

        public void update()
        {
            m_pObject3dPtr.rotateLocalYf(1);
        }

        public static void Main(string[] args)
        {
			Console.WriteLine("monogear_engine Main() called from C#222");
            MonoGEAREntryPointClass.engine_consoleLog("HELLOWORLD", 0);
        }
    }
}
