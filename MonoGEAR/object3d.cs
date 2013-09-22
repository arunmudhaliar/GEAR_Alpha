using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using MonoGEAR;

namespace MonoGEAR
{
    public sealed class object3d : transform
    {
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern string object3d_getName(IntPtr obj);
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int object3d_getID(IntPtr obj);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr object3d_find(IntPtr obj, string name);
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int object3d_getChildCount(IntPtr obj);
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr object3d_getChild(IntPtr obj, int index);


        public object3d(IntPtr ptr):
            base(ptr)
        {
        }

        public static object3d create(string name)
        {
            return new object3d(MonoGEAREntryPointClass.engine_loadAndAppendFBX(MonoGEAREntryPointClass.engine_getWorld(0), name));
        }

        public static bool destroy(object3d obj)
        {
            return MonoGEAREntryPointClass.engine_destroyObject3d(MonoGEAREntryPointClass.engine_getWorld(0), obj.getHandle());
        }

        public object3d find(string name)
        {
            return new object3d(object3d_find(getHandle(), name));
        }

        public static void mono_object3d_onObject3dChildAppend(IntPtr parent, IntPtr child)
        {
            Console.WriteLine(object3d_getID(child) + " added");
        }

        public static void mono_object3d_onObject3dChildRemove(IntPtr parent, IntPtr child)
        {
            Console.WriteLine(object3d_getID(child) + " removed");
        }
        //List<object3d> m_cChildList;
    }
}
