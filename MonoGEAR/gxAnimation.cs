using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace MonoGEAR
{
    public class gxAnimation
    {
        //DllExport gxAnimationSet* gxAnimation_play(gxAnimation* animation, int animSetIndex);
        //DllExport void gxAnimation_stop(gxAnimation* animation);
        //DllExport void gxAnimation_pause(gxAnimation* animation);
        //DllExport void gxAnimation_resume(gxAnimation* animation);
        //DllExport void gxAnimation_rewind(gxAnimation* animation);
        //DllExport void gxAnimation_rewindAll(gxAnimation* animation);
        //DllExport bool gxAnimation_isPlaying(gxAnimation* animation, int animSetIndex);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr gxAnimation_play(IntPtr animation, int animSetIndex);
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void gxAnimation_stop(IntPtr animation);
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void gxAnimation_pause(IntPtr animation);
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void gxAnimation_resume(IntPtr animation);
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void gxAnimation_rewind(IntPtr animation);
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void gxAnimation_rewindAll(IntPtr animation);
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool gxAnimation_isPlaying(IntPtr animation);

        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr gxAnimation_getAnimationSet(IntPtr animation, int index);
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int gxAnimation_getAnimSetCount(IntPtr animation);
        [DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void gxAnimation_appendAnimationSet(IntPtr animation, IntPtr animationSet);

        public gxAnimation(IntPtr ptr)
        {
            animationHandle_cpp = ptr;
        }

        public void play(int animSetIndex)
        {
            gxAnimation_play(animationHandle_cpp, animSetIndex);
        }

        public void stop()
        {
            gxAnimation_stop(animationHandle_cpp);
        }

        public void pause()
        {
            gxAnimation_pause(animationHandle_cpp);
        }

        public void resume()
        {
            gxAnimation_resume(animationHandle_cpp);
        }

        public void rewind()
        {
            gxAnimation_rewind(animationHandle_cpp);
        }

        public void rewindAll()
        {
            gxAnimation_rewindAll(animationHandle_cpp);
        }

        public bool isPlaying()
        {
            return gxAnimation_isPlaying(animationHandle_cpp);
        }

        public int getAnimationSetCount()
        {
            return gxAnimation_getAnimSetCount(animationHandle_cpp);
        }

        public gxAnimationSet getAnimationSet(int index)
        {
            return new gxAnimationSet(gxAnimation_getAnimationSet(animationHandle_cpp, index));
        }

        public void appendAnimationSet(gxAnimationSet animationSet)
        {
            gxAnimation_appendAnimationSet(animationHandle_cpp, animationSet.getHandle());
        }

		IntPtr animationHandle_cpp;
    }
}
