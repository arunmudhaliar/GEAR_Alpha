using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;
using MonoGEAR;

namespace helloworldNS
{
    public class helloworld : monoScript
    {
        protected override void initMonoScript()
        {
        }

        public void update()
        {
        }

        public static void Main(string[] args)
        {
			Console.WriteLine("monogear_engine Main() called from C#222");
            MonoGEAREntryPointClass.engine_consoleLog("HELLOWORLD", 0);
        }
    }
}
