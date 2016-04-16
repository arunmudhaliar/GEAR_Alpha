using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace MonoGEAR
{
	public class gxLight : monoScript
    {
		[DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void gxLight_setLightType(IntPtr light, gxLight.ELIGHT_TYPE eType);

		[DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern gxLight.ELIGHT_TYPE gxLight_getLightType(IntPtr light);
		[DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void gxLight_setDiffuseColor(IntPtr light, float[] clr);
		[DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void gxLight_setAmbientColor(IntPtr light, float[] clr);
		[DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void gxLight_setSpecularColor(IntPtr light, float[] clr);
		[DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void gxLight_getDiffuseColor(IntPtr light, [In, Out] float[] clr);
		[DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void gxLight_getAmbientColor(IntPtr light, [In, Out] float[] clr);
		[DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void gxLight_getSpecularColor(IntPtr light, [In, Out] float[] clr);


		[DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void gxLight_setConstantAttenuation(IntPtr light, float value);
		[DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void gxLight_setLinearAttenuation(IntPtr light, float value);
		[DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void gxLight_setQuadraticAttenuation(IntPtr light, float value);
		[DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern float gxLight_getConstantAttenuation(IntPtr light);
		[DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern float gxLight_getLinearAttenuation(IntPtr light);
		[DllImport("GEAREngine.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern float gxLight_getQuadraticAttenuation(IntPtr light);


        public enum ELIGHT_TYPE
        {
            LIGHT_POINT,
            LIGHT_DIRECTIONAL,
        };

		private gxLight():
		base()
        {
			Console.WriteLine ("Default Constructor for gxLight Called");
        }

		protected gxLight(string name, ELIGHT_TYPE eType) :
            base()
        {
			IntPtr script = MonoGEAREntryPointClass.engine_createLight (MonoGEAREntryPointClass.engine_getWorld (0), name, (int)eType);
			setHandle (script);
			Console.WriteLine ("Paramatrised Constructor for gxLight Called");
        }

		public static gxLight createLight(string name, ELIGHT_TYPE eType)
        {
			gxLight light = new gxLight(name, eType);
			return light;
        }


		protected override void initMonoScript()
		{
		}

		protected override void update()
		{
		}

		public ELIGHT_TYPE lightType
		{
			get
			{
				return gxLight_getLightType(scriptInstanceHandle_cpp);
			}
			set
			{
				gxLight_setLightType (scriptInstanceHandle_cpp, value);
			}
		}

		public vector4 diffuseColor
		{
			get
			{
				float[] array = new float[4];
				gxLight_getDiffuseColor (scriptInstanceHandle_cpp, array);
				return new vector4 (array);
			}
			set
			{
				gxLight_setDiffuseColor (scriptInstanceHandle_cpp, value.toArray());
			}
		}

		public vector4 specularColor
		{
			get
			{
				float[] array = new float[4];
				gxLight_getSpecularColor (scriptInstanceHandle_cpp, array);
				return new vector4 (array);
			}
			set
			{
				gxLight_setSpecularColor (scriptInstanceHandle_cpp, value.toArray());
			}
		}

		public vector4 ambientColor
		{
			get
			{
				float[] array = new float[4];
				gxLight_getAmbientColor (scriptInstanceHandle_cpp, array);
				return new vector4 (array);
			}
			set
			{
				gxLight_setAmbientColor (scriptInstanceHandle_cpp, value.toArray());
			}
		}

		public float constantAttenuation
		{
			get
			{
				return gxLight_getConstantAttenuation (scriptInstanceHandle_cpp);
			}
			set
			{
				gxLight_setConstantAttenuation (scriptInstanceHandle_cpp, value);
			}
		}

		public float linearAttenuation
		{
			get
			{
				return gxLight_getLinearAttenuation (scriptInstanceHandle_cpp);
			}
			set
			{
				gxLight_setLinearAttenuation (scriptInstanceHandle_cpp, value);
			}
		}

		public float quadraticAttenuation
		{
			get
			{
				return gxLight_getQuadraticAttenuation (scriptInstanceHandle_cpp);
			}
			set
			{
				gxLight_setQuadraticAttenuation (scriptInstanceHandle_cpp, value);
			}
		}
    }
}
