using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;
using System.IO;

namespace AndroidProjectMaker
{
    class AndroidManifest
    {
        public static void createAndroidManifestFile(string projectpath, string bundleidentifier, string projectname)
        {
            StreamWriter stream = new StreamWriter(projectpath + "//AndroidManifest.xml");

            stream.WriteLine("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
            stream.WriteLine("<manifest xmlns:android=\"http://schemas.android.com/apk/res/android\"");
            stream.WriteLine("package=\"" + bundleidentifier + "\"");
            stream.WriteLine("android:versionCode=\"1\"");
            stream.WriteLine("android:versionName=\"1.0\" >");

            stream.WriteLine("<uses-sdk");
            stream.WriteLine("android:minSdkVersion=\"14\"");
            stream.WriteLine("android:targetSdkVersion=\"18\" />");

            stream.WriteLine("<uses-feature android:required=\"true\" android:glEsVersion=\"0x00020000\"/>");
            stream.WriteLine("<uses-permission android:name=\"android.permission.INTERNET\" />");
            stream.WriteLine("<uses-permission android:name=\"android.permission.READ_PHONE_STATE\" />");
            stream.WriteLine("<uses-permission android:name=\"android.permission.READ_EXTERNAL_STORAGE\" />");
            stream.WriteLine("<uses-permission android:name=\"android.permission.WRITE_EXTERNAL_STORAGE\" />");
    
            stream.WriteLine("<application");
            stream.WriteLine("android:allowBackup=\"true\"");
            //stream.WriteLine("android:icon=\"@drawable/ic_launcher\"");
            stream.WriteLine("android:label=\"@string/app_name\" >");
            //stream.WriteLine("android:theme=\"@style/AppTheme\" >	<!-- android:debuggable=\"true\"-->");
            //<!-- 
            //<activity
            //android:name="com.example.test.MainActivity"
            //android:label="@string/app_name" >
            //<intent-filter>
            //<action android:name="android.intent.action.MAIN" />

            //<category android:name="android.intent.category.LAUNCHER" />
            //</intent-filter>
            //</activity>
            //-->
        
            stream.WriteLine("<!-- Our activity is the built-in NativeActivity framework class.");
            stream.WriteLine("This will take care of integrating with our NDK code. -->");
            stream.WriteLine("<activity android:name=\"" + bundleidentifier + ".MainActivity\"");
            stream.WriteLine("android:label=\"@string/app_name\"");
            stream.WriteLine("android:configChanges=\"orientation|keyboardHidden\"");
            stream.WriteLine("android:screenOrientation=\"landscape\">");
            stream.WriteLine("<!-- Tell NativeActivity the name of or .so -->");
            stream.WriteLine("<meta-data android:name=\"android.app.lib_name\"");
            stream.WriteLine("android:value=\"gearapp\" />");
            stream.WriteLine("<intent-filter>");
            stream.WriteLine("<action android:name=\"android.intent.action.MAIN\" />");
            stream.WriteLine("<category android:name=\"android.intent.category.LAUNCHER\" />");
            stream.WriteLine("</intent-filter>");
            stream.WriteLine("</activity>");
        
            stream.WriteLine("</application>");

            stream.WriteLine("</manifest>");




            stream.Close();
        }
    }
}
