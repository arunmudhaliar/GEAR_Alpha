using System.Collections;
using System.IO;

public class ZBGlobal {
	
	public struct stZombieResourceInfo
	{
		public string zombieFileName;
		public int zombieType;
		
		public stZombieResourceInfo(string filename, int type)
		{
			zombieFileName=filename;
			zombieType=type;
		}
	}
	
    public static void writeString(string str, BinaryWriter wr)
    {
        wr.Write(str.Length);
        for (int x = 0; x < str.Length; x++)
        {
            wr.Write((byte)str[x]);
        }
    }

    public static string readString(BinaryReader rd)
    {
        int length=rd.ReadInt32();
        char[] buffer = new char[length];
        for (int x = 0; x < length; x++)
        {
            buffer[x]=rd.ReadChar();
        }
        return new string(buffer);
    }
	
	public static int SCENE_EMPTY				= 100;
	public static int SCENE_SPLASH				= 200;
	public static int SCENE_MAIN_MENU			= 300;
	public static int SCENE_ZOMBIE_SELECTION	= 400;
	public static int SCENE_ARENA				= 500;

    //arun_check_for_mono public static ZBCamera	g_ZBCamera			= null;
    //arun_check_for_mono public static GameObject g_MainCamera		= null;
    //arun_check_for_mono public static GameObject g_GUICamera		= null;
	
//	public static string[] g_ZombieFileList =
//	{
//		"zombies/zombie0/zombie_00",
//		"",
//		"zombies/zombie2/zombie_00_body_withouthands",
//		"zombies/zombie3/zombie@mesh",
//	};
	
	public static stZombieResourceInfo[] g_ZombieResourceInfoList =
	{
		new stZombieResourceInfo("zombie_bodymesh", 1)
	};
	
	public static string[] g_MediumZombieMeleeWeaponResourceInfoList=
	{
		"chainsaw",
	};

	public static string[] g_MediumZombieLongRangeWeaponResourceInfoList=
	{
		"shotgun",
	};
}
