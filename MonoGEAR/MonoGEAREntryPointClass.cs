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
        static extern int engine_test_function_for_mono();

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void engine_init(int nWorldToCreate);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr engine_getWorld(int index);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void engine_update(IntPtr world, float dt);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void engine_resize(IntPtr world, float x, float y, float cx, float cy, float nearplane, float farplane);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void engine_render(IntPtr world, IntPtr light);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void engine_renderSingleObject(IntPtr world, IntPtr obj, IntPtr light);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr engine_loadAndAppendFBX(IntPtr world, string filename);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern IntPtr engine_appendObject3dToRoot(IntPtr world, IntPtr obj);


        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern IntPtr engine_loadFBX(IntPtr world, string filename, string projecthomedirectory);

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
        static extern IntPtr engine_loadTextureFromFile(IntPtr world, IntPtr material, string filename, int submap);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool engine_removeObject3d(IntPtr world, IntPtr obj);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool engine_destroyObject3d(IntPtr world, IntPtr obj);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr engine_createEmptyObject3d(IntPtr parentObj, string name);


        public static void Main(string[] args)
        {
            //for testing engine_init(1);
            string str = Path.GetFullPath("C:/MYPROJECTS/GEAR_PROJECTS\\animation/Assets/csharp/test.txt");
            str=str.Replace("\\", "/");
            str = str;
        }

        public static int monogear_engine_test_function_for_mono()
        {
            Console.WriteLine("monogear_engine_test_function_for_mono called from C#");

            return engine_test_function_for_mono();
        }


        /// <summary>
        /// MONOGEAR GAME LOOP
        /// </summary>

        object3d m_pRootObject3d;
        public void mono_game_start()
        {
            /*
            object3d zombie_bodymesh = object3d.create(Resource.getResourcePath("for_arun/zombie_bodymesh.FBX"));
            object3d zombie_idle = object3d.create(Resource.getResourcePath("for_arun/zombie@idle.FBX"));
            object3d zombie_run = object3d.create(Resource.getResourcePath("for_arun/zombie@run.FBX"));
            object3d shotgun_set0_attack01 = object3d.create(Resource.getResourcePath("for_arun/zombies/animations/medium/weapon/shotgun/shotgun@set0_attack01.FBX"));
            object3d shotgun_set0_attack02 = object3d.create(Resource.getResourcePath("for_arun/zombies/animations/medium/weapon/shotgun/shotgun@set0_attack02.FBX"));
            //shotgun@attack_run.FBX

            //D:\MYPROJECTS\GEAR_PROJECTS\test1\Assets\for_arun\zombies\characters\medium\range_weapons
            object3d shotgun = object3d.create(Resource.getResourcePath("for_arun/zombies/characters/medium/range_weapons/shotgun.FBX")).find("mesh_l_weapon");
            object3d chainsaw = object3d.create(Resource.getResourcePath("for_arun/zombies/characters/medium/melee_weapons/chainsaw.FBX")).find("mesh_r_weapon");

            zombie_bodymesh.createAnimationController();
            zombie_bodymesh.animation.appendAnimationSet(zombie_idle.animation.getAnimationSet(0));
            zombie_bodymesh.animation.appendAnimationSet(zombie_run.animation.getAnimationSet(0));
            zombie_bodymesh.animation.appendAnimationSet(shotgun_set0_attack01.animation.getAnimationSet(0));
            zombie_bodymesh.animation.appendAnimationSet(shotgun_set0_attack02.animation.getAnimationSet(0));

            object3d.destroy(zombie_idle);
            object3d.destroy(zombie_run);
            object3d.destroy(shotgun_set0_attack01);
            object3d.destroy(shotgun_set0_attack02);

            object3d empty = new object3d("hello world");
            object3d empty2 = new object3d(empty, "hello world child");
            //zombie_bodymesh.updateLocalPositionf(100, 0, 0);

            shotgun.parent = zombie_bodymesh.find("bone_l_forearm");
            shotgun.copyTranfrom(zombie_bodymesh.find("bone_l_forearm"));

            chainsaw.parent = zombie_bodymesh.find("bone_r_forearm");
            chainsaw.copyTranfrom(zombie_bodymesh.find("bone_r_forearm"));

            zombie_bodymesh.parent = empty2;
            //Console.WriteLine("find bone location = " + shotgun.name);
            //zombie_bodymesh.rotateLocalZf(10);
            m_pRootObject3d = zombie_bodymesh;
            //m_pRootObject3d.rotateLocalZf(10);

            //Console.WriteLine(zombie_idle.animation.getAnimationSetCount() + " count");
            //Console.WriteLine(zombie_bodymesh.name + "created");

            //animbuilder
            //Stream stream = new MemoryStream(asset.bytes);
            FileStream stream = new FileStream(Resource.getResourcePath("for_arun/zombies/animations/medium/weapon/shotgun/medium_weapon_shotgun.anb.txt"), FileMode.Open, FileAccess.Read);
            BinaryReader reader = new BinaryReader(stream);

            stAnimationPackage package = new stAnimationPackage();
            package.readANBFile(reader);
            reader.Close();
             * */
        }

        public void mono_game_run(float dt)
        {
        }

        public bool mono_game_onkeydown(int charValue, int flag)
        {
            /*
            if (charValue == 37)
            {
                Console.WriteLine("animset= " + m_pRootObject3d.applyAnimationSetRecursive(0).name);
                m_pRootObject3d.animation.play(0);
            }
            else if (charValue == 38)
            {
                Console.WriteLine("animset= " + m_pRootObject3d.applyAnimationSetRecursive(1).name);
                m_pRootObject3d.animation.play(1);
            }
            else if (charValue == 39)
            {
                Console.WriteLine("animset= " + m_pRootObject3d.applyAnimationSetRecursive(2).name);
                m_pRootObject3d.animation.play(2);
            }
            else if (charValue == 40)
            {
                Console.WriteLine("animset= " + m_pRootObject3d.applyAnimationSetRecursive(3).name);
                m_pRootObject3d.animation.play(3);
            }
            Console.WriteLine("keydown " + charValue);
             * */
            return true;
        }

        public bool mono_game_onkeyup(int charValue, int flag)
        {
            //Console.WriteLine("keyup " + charValue);
            return true;
        }

    }
}
