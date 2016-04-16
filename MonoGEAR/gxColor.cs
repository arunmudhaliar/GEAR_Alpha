using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MonoGEAR
{
    public class gxColor
    {
        public float r, g, b, a;

		public static gxColor RED
		{
			get
			{
				return new gxColor (1.0f, 0.0f, 0.0f, 1.0f);
			}
		}

		public static gxColor GREEN
		{
			get
			{
				return new gxColor (0.0f, 1.0f, 0.0f, 1.0f);
			}
		}

		public static gxColor BLUE
		{
			get
			{
				return new gxColor (0.0f, 0.0f, 1.0f, 1.0f);
			}
		}

		public static gxColor WHITE
		{
			get
			{
				return new gxColor (1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		public static gxColor BLACK
		{
			get
			{
				return new gxColor (0.0f, 0.0f, 0.0f, 1.0f);
			}
		}

		public static gxColor GREY
		{
			get
			{
				return new gxColor (0.5f, 0.5f, 0.5f, 1.0f);
			}
		}


		public gxColor() { r = g = b = a =0; }
		public gxColor(float red, float green, float blue, float alpha) { r = red; g = green; b = blue; a = alpha; }
		public gxColor(gxColor v) { r = v.r; g = v.g; b = v.b; a = v.a;	}
		public gxColor(float[] array)	{
			r = array [0];
			g = array [1];
			b = array [2];
			a = array [3];
		}

		public void set(float red, float green, float blue, float alpha) { r = red; g = green; b = blue; a = alpha;	}

		public float[] toArray()
		{
			return new float[] { r, g, b, a };
		}
    }
}