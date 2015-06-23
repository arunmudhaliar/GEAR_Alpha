using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;
using MonoGEAR;

namespace helloworldNS
{
    public class test : monoScript
    {
        protected override void initMonoScript()
        {
            MonoGEAREntryPointClass.engine_consoleLog("test CREATED", 0);
        }

        public void update()
        {
            MonoGEAREntryPointClass.engine_consoleLog("test", 0);
        }
    }
}
