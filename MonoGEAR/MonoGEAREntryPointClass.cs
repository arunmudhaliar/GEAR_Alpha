using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;

namespace MonoGEAR
{
    public class MonoGEAREntryPointClass
    {
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
            object3d zombie_bodymesh = object3d.create("D:\\MYPROJECTS\\GEAR_PROJECTS\\test1\\Assets\\for_arun\\zombie_bodymesh.FBX");
            object3d zombie_idle = object3d.create("D:\\MYPROJECTS\\GEAR_PROJECTS\\test1\\Assets\\for_arun\\zombie@idle.FBX");
            object3d zombie_run = object3d.create("D:\\MYPROJECTS\\GEAR_PROJECTS\\test1\\Assets\\for_arun\\zombie@run.FBX");
            object3d shotgun_set0_attack01 = object3d.create("D:\\MYPROJECTS\\GEAR_PROJECTS\\test1\\Assets\\for_arun\\zombies\\animations\\medium\\weapon\\shotgun\\shotgun@set0_attack01.FBX");
            object3d shotgun_set0_attack02 = object3d.create("D:\\MYPROJECTS\\GEAR_PROJECTS\\test1\\Assets\\for_arun\\zombies\\animations\\medium\\weapon\\shotgun\\shotgun@set0_attack02.FBX");
            //shotgun@attack_run.FBX

            zombie_bodymesh.createAnimationController();
            zombie_bodymesh.animation.appendAnimationSet(zombie_idle.animation.getAnimationSet(0));
            zombie_bodymesh.animation.appendAnimationSet(zombie_run.animation.getAnimationSet(0));
            zombie_bodymesh.animation.appendAnimationSet(shotgun_set0_attack01.animation.getAnimationSet(0));
            zombie_bodymesh.animation.appendAnimationSet(shotgun_set0_attack02.animation.getAnimationSet(0));
            //zombie_bodymesh.applyAnimationSetRecursive(0);
            //zombie_bodymesh.animation.play(0);

            zombie_bodymesh.updateLocalPositionf(0, 0, 0);
            //zombie_bodymesh.rotateLocalZf(10);
            m_pRootObject3d = zombie_bodymesh;
            //m_pRootObject3d.rotateLocalZf(10);

            Console.WriteLine(zombie_idle.animation.getAnimationSetCount() + " count");
            Console.WriteLine(zombie_bodymesh.name + "created");

            //animbuilder
            //Stream stream = new MemoryStream(asset.bytes);
            FileStream stream = new FileStream("D:\\MYPROJECTS\\GEAR_PROJECTS\\test1\\Assets\\for_arun\\zombies\\animations\\medium\\weapon\\shotgun\\medium_weapon_shotgun.anb.txt", FileMode.Open, FileAccess.Read);
            BinaryReader reader = new BinaryReader(stream);

            stAnimationPackage package = new stAnimationPackage();
            package.readANBFile(reader);
            reader.Close();

        }

        public void mono_game_run(float dt)
        {
            //m_pRootObject3d.rotateLocalZf(10);
        }

        public bool mono_game_onkeydown(int charValue, int flag)
        {
            if (charValue == 37)
            {
                m_pRootObject3d.applyAnimationSetRecursive(0);
                m_pRootObject3d.animation.play(0);
            }
            else if (charValue == 38)
            {
                m_pRootObject3d.applyAnimationSetRecursive(1);
                m_pRootObject3d.animation.play(1);
            }
            else if (charValue == 39)
            {
                m_pRootObject3d.applyAnimationSetRecursive(2);
                m_pRootObject3d.animation.play(2);
            }
            else if (charValue == 40)
            {
                m_pRootObject3d.applyAnimationSetRecursive(3);
                m_pRootObject3d.animation.play(3);
            }
            Console.WriteLine("keydown " + charValue);
            return true;
        }

        public bool mono_game_onkeyup(int charValue, int flag)
        {
            Console.WriteLine("keyup " + charValue);
            return true;
        }

    }
}
