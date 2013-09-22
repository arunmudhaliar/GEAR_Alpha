using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace MonoGEAR
{
    class MonoGEAREntryPointClass
    {
        /*
        DllExport void engine_test_function_for_mono();
        DllExport void engine_init(int nWorldToCreate);
        DllExport gxWorld* engine_getWorld(int index);

        DllExport void engine_update(gxWorld* world, float dt);
        DllExport void engine_resize(gxWorld* world, float x, float y, float cx, float cy);
        DllExport void engine_render(gxWorld* world);
        DllExport void engine_renderSingleObject(gxWorld* world, object3d* obj);
        DllExport object3d* engine_loadAndAppendFBX(gxWorld* world, const char* filename);
        DllExport object3d* engine_loadFBX(gxWorld* world, const char* filename);
        DllExport object3d* engine_appendObject3dToRoot(gxWorld* world, object3d* obj);
        
        DllExport void engine_mouseLButtonDown(gxWorld* world, int x, int y, int flag);
        DllExport void engine_mouseLButtonUp(gxWorld* world, int x, int y, int flag);
        DllExport void engine_mouseRButtonDown(gxWorld* world, int x, int y, int flag);
        DllExport void engine_mouseRButtonUp(gxWorld* world, int x, int y, int flag);
        DllExport void engine_mouseWheel(gxWorld* world, int zDelta, int x, int y, int flag);
        DllExport void engine_mouseMove(gxWorld* world, int x, int y, int flag);
        DllExport void engine_setMetaFolder(gxWorld* world, const char* metaFolder);
        DllExport gxTexture* engine_loadTextureFromFile(gxWorld* world, gxMaterial* material, const char* filename);
        DllExport bool engine_removeObject3d(gxWorld* world, object3d* obj);
        */

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void engine_test_function_for_mono();

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void engine_init(int nWorldToCreate);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr engine_getWorld(int index);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void engine_update(IntPtr world, float dt);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void engine_resize(IntPtr world, float x, float y, float cx, float cy);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void engine_render(IntPtr world);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void engine_renderSingleObject(IntPtr world, IntPtr obj);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr engine_loadAndAppendFBX(IntPtr world, string filename);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern IntPtr engine_appendObject3dToRoot(IntPtr world, IntPtr obj);


        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern IntPtr engine_loadFBX(IntPtr world, string filename);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void engine_mouseLButtonDown(IntPtr world, int x, int y, int flag);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void engine_mouseLButtonUp(IntPtr world, int x, int y, int flag);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void engine_mouseRButtonDown(IntPtr world, int x, int y, int flag);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void engine_mouseRButtonUp(IntPtr world, int x, int y, int flag);


        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void engine_mouseWheel(IntPtr world, int zDelta, int x, int y, int flag);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void engine_mouseMove(IntPtr world, int x, int y, int flag);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void engine_setMetaFolder(IntPtr world, string metaFolder);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern IntPtr engine_loadTextureFromFile(IntPtr world, IntPtr material, string filename);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool engine_removeObject3d(IntPtr world, IntPtr obj);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool engine_destroyObject3d(IntPtr world, IntPtr obj);



        public static void Main(string[] args)
        {
            //for testing engine_init(1);
        }

        public static void monogear_engine_test_function_for_mono()
        {
            Console.WriteLine("monogear_engine_test_function_for_mono called from C#");

            engine_test_function_for_mono();
        }


        /// <summary>
        /// MONOGEAR GAME LOOP
        /// </summary>

        object3d m_pRootObject3d;
        public void mono_game_start()
        {
            m_pRootObject3d = object3d.create("D:\\MYPROJECTS\\GEAR_PROJECTS\\test1\\Assets\\Level_1.FBX");
            object3d.destroy(m_pRootObject3d.find("BarTable"));
        }

        public void mono_game_run(float dt)
        {
        }
    }
}
