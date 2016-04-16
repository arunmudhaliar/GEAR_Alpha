using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace MonoGEAR
{
    public class monoScript
    {
		[DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern IntPtr monoScriptObjectInstance_getAttachedObject(IntPtr instance);

        protected IntPtr scriptInstanceHandle_cpp;
        protected object3d object3dInstance;
		protected IntPtr attachedObjectHandle_cpp;

		public monoScript()
		{
			Console.WriteLine("CONSTRUCTOR CALLED in monoScript");
		}

		protected void setHandle(IntPtr script)
        {
			scriptInstanceHandle_cpp = script;
			attachedObjectHandle_cpp = monoScriptObjectInstance_getAttachedObject(scriptInstanceHandle_cpp);
			if (object3dInstance == null)
			{
				object3dInstance = new object3d (attachedObjectHandle_cpp);
				Console.WriteLine ("monoScript::setHandler() invoked with " + attachedObjectHandle_cpp);
			}
        }

		public object3d getAttachedObject()
		{
			return object3dInstance;
		}

        protected virtual void initMonoScript()
        {
        }

        protected virtual void update()
        {
        }
    }
}
