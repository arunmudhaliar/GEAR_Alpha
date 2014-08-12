using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using MonoGEAR;

namespace MonoGEAR
{
    public class object3d : transform
    {
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr object3d_getName(IntPtr obj);
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int object3d_getID(IntPtr obj);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr object3d_find(IntPtr obj, string name);
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int object3d_getChildCount(IntPtr obj);
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr object3d_getChild(IntPtr obj, int index);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr object3d_createAnimationController(IntPtr obj);
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr object3d_getAnimationController(IntPtr obj);
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr object3d_applyAnimationSetRecursive(IntPtr obj, int index);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr object3d_appendChild(IntPtr obj, IntPtr child);
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool object3d_removeChild(IntPtr obj, IntPtr child);
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr object3d_getParent(IntPtr obj);

        protected object3d():
            base(MonoGEAREntryPointClass.engine_createEmptyObject3d(MonoGEAREntryPointClass.engine_getWorld(0), "emptyobject"))
        {
            Console.WriteLine("object3d() called");
            initObject3d();
        }

        public object3d(string name):
            base(MonoGEAREntryPointClass.engine_createEmptyObject3d(MonoGEAREntryPointClass.engine_getWorld(0), name))
        {
            Console.WriteLine("object3d(string name) called");
            initObject3d();
        }

        public object3d(object3d parent, string name):
            base(MonoGEAREntryPointClass.engine_createEmptyObject3d(parent.getHandle(), name))
        {
            Console.WriteLine("object3d(object3d parent, string name) called");
            initObject3d();
        }

        public object3d(IntPtr ptr):
            base(ptr)
        {
            Console.WriteLine("object3d(IntPtr ptr) called");
            initObject3d();
        }

        protected override void setHandle(IntPtr ptr)
        {
            m_pObj3dPtr = ptr;
        }

        public static object3d load(string name)
        {
            return new object3d(MonoGEAREntryPointClass.engine_loadAndAppendMesh(MonoGEAREntryPointClass.engine_getWorld(0), name));
        }

        public static bool destroy(object3d obj)
        {
            return MonoGEAREntryPointClass.engine_destroyObject3d(MonoGEAREntryPointClass.engine_getWorld(0), obj.getHandle());
        }

        public gxAnimationSet applyAnimationSetRecursive(int index)
        {
            return new gxAnimationSet(object3d_applyAnimationSetRecursive(getHandle(), index));
        }

        public gxAnimation createAnimationController()
        {
            m_pAnimation = new gxAnimation(object3d_createAnimationController(getHandle()));
            return m_pAnimation;
        }

        public gxAnimation animation
        {
            get
            {
                if(m_pAnimation!=null)
                    return m_pAnimation;
                else
                    m_pAnimation = new gxAnimation(object3d_getAnimationController(getHandle()));
                return m_pAnimation;
            }
        }

        public object3d parent
        {
            get
            {
                return new object3d(object3d_getParent(getHandle()));
            }
            set
            {
                if(parent!=null)
                    parent.removeChild(this);
                value.appendChild(this);
            }
        }

        public object3d find(string name)
        {
            return new object3d(object3d_find(getHandle(), name));
        }

        public string name
        {
            get
            {
                m_pName = Marshal.PtrToStringAnsi(object3d_getName(getHandle()));
                return m_pName;
            }
            set
            {
                //not implemented
            }
        }
        public static void mono_object3d_onObject3dChildAppend(IntPtr parent, IntPtr child)
        {
           //Console.WriteLine(Marshal.PtrToStringAnsi(object3d_getName(child)) + " added");
        }

        public static void mono_object3d_onObject3dChildRemove(IntPtr parent, IntPtr child)
        {
            //Console.WriteLine(Marshal.PtrToStringAnsi(object3d_getName(child)) + " removed");
        }

        public void appendChild(object3d child)
        {
            object3d_appendChild(m_pObj3dPtr, child.getHandle());
        }

        public bool removeChild(object3d child)
        {
            return object3d_removeChild(m_pObj3dPtr, child.getHandle());
        }
        //List<object3d> m_cChildList;


        protected virtual void initObject3d()
        {
        }

        string m_pName;
        gxAnimation m_pAnimation;

        List<Object> m_vComponents;
    }
}
