using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MonoGEAR
{
    public class gxLight : object3d
    {
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
    }
}
