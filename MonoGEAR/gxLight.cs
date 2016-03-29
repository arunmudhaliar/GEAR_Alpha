using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace MonoGEAR
{
    public class gxLight : object3d
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

        public gxLight():
            base(MonoGEAREntryPointClass.engine_createLight(MonoGEAREntryPointClass.engine_getWorld(0), "PointLight", (int)ELIGHT_TYPE.LIGHT_POINT))
        {
        }

        public gxLight(string name, ELIGHT_TYPE eType) :
            base(MonoGEAREntryPointClass.engine_createLight(MonoGEAREntryPointClass.engine_getWorld(0), name, (int)eType))
        {

        }

        public static gxLight createLight(string name, ELIGHT_TYPE eType)
        {
            return new gxLight(name, eType);
        }

		public ELIGHT_TYPE lightType
		{
			get
			{
				return gxLight_getLightType(getHandle());
			}
			set
			{
				gxLight_setLightType (getHandle (), value);
			}
		}

		public vector4 diffuseColor
		{
			get
			{
				float[] array = new float[4];
				gxLight_getDiffuseColor (getHandle (), array);
				return new vector4 (array);
			}
			set
			{
				gxLight_setDiffuseColor (getHandle (), value.toArray());
			}
		}

		public vector4 specularColor
		{
			get
			{
				float[] array = new float[4];
				gxLight_getSpecularColor (getHandle (), array);
				return new vector4 (array);
			}
			set
			{
				gxLight_setSpecularColor (getHandle (), value.toArray());
			}
		}

		public vector4 ambientColor
		{
			get
			{
				float[] array = new float[4];
				gxLight_getAmbientColor (getHandle (), array);
				return new vector4 (array);
			}
			set
			{
				gxLight_setAmbientColor (getHandle (), value.toArray());
			}
		}

		public float constantAttenuation
		{
			get
			{
				return gxLight_getConstantAttenuation (getHandle ());
			}
			set
			{
				gxLight_setConstantAttenuation (getHandle (), value);
			}
		}

		public float linearAttenuation
		{
			get
			{
				return gxLight_getLinearAttenuation (getHandle ());
			}
			set
			{
				gxLight_setLinearAttenuation (getHandle (), value);
			}
		}

		public float quadraticAttenuation
		{
			get
			{
				return gxLight_getQuadraticAttenuation (getHandle ());
			}
			set
			{
				gxLight_setQuadraticAttenuation (getHandle (), value);
			}
		}
//		//ELIGHT_TYPE lightType;
//		vector4 diffuseColor;
//		vector4 specularColor;
//		vector4 ambientColor;
//		float constantAttenuation;
//		float linearAttenuation;
//		float quadraticAttenuation;
    }
}
