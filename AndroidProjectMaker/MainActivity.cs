using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace AndroidProjectMaker
{
    class MainActivity
    {
        public static void createMainActivityFile(string projectpath, string bundleidentifier/*, string projectname*/)
        {
            string[] split_bundle_identifier = bundleidentifier.Split('.');
            string filename = projectpath + "//src//" +
                split_bundle_identifier[0] + "//" +
                split_bundle_identifier[1] + "//" +
                split_bundle_identifier[2] + "//" +
                "MainActivity.java";
            StreamWriter stream = new StreamWriter(filename);

/////////////////////////////////////////////////
            stream.WriteLine("package " + bundleidentifier + ";");

            stream.WriteLine("import " + bundleidentifier + ".R;");

            stream.WriteLine("import android.os.Bundle;");
            stream.WriteLine("import android.os.Environment;");
            stream.WriteLine("import android.app.NativeActivity;");
            stream.WriteLine("import android.view.Menu;");
            stream.WriteLine("import android.view.Window;");
            stream.WriteLine("import android.view.WindowManager;");

            stream.WriteLine("public class MainActivity extends NativeActivity {");

	            stream.WriteLine("\tpublic boolean firstlaunch=true;");
                stream.WriteLine("\t@Override");
                stream.WriteLine("\tprotected void onCreate(Bundle savedInstanceState) {");
                    stream.WriteLine("\t\trequestWindowFeature(Window.FEATURE_NO_TITLE);");
                    stream.WriteLine("\t\tsuper.onCreate(savedInstanceState);");
                    //stream.WriteLine("\t\tsetContentView(R.layout.activity_main);");
                    
                    stream.WriteLine("\t\tgetWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,");
                                            stream.WriteLine("\t\tWindowManager.LayoutParams.FLAG_FULLSCREEN);");
                    stream.WriteLine("\t\tSystem.out.println(\"getExternalStorageDirectory \"+ Environment.getExternalStorageDirectory());");
                stream.WriteLine("\t}");


                stream.WriteLine("\t@Override");
                stream.WriteLine("\tpublic boolean onCreateOptionsMenu(Menu menu) {");
                    stream.WriteLine("\t\t// Inflate the menu; this adds items to the action bar if it is present.");
                    //stream.WriteLine("\t\tgetMenuInflater().inflate(R.menu.main, menu);");
                    stream.WriteLine("\t\treturn true;");
                stream.WriteLine("\t}");
    
                stream.WriteLine("\t@Override");
                stream.WriteLine("\tpublic void onPause() {");
    	            stream.WriteLine("\t\tsuper.onPause();");
            //    	if(firstlaunch==true)
            //    	{
            //    		firstlaunch=false;
            //    		return;
            //    	}
            //    	mainlib();
                stream.WriteLine("\t}");
    
                stream.WriteLine("\t@Override");
                stream.WriteLine("\tpublic void onResume() {");
    	            stream.WriteLine("\tsuper.onResume();");
            //    	if(firstlaunch==true)
            //    	{
            //    		firstlaunch=false;
            //    		return;
            //    	}
            //    	mainlib();
                stream.WriteLine("\t}");
    
                stream.WriteLine("\tpublic static native int mainlib();");
                stream.WriteLine("\t\tstatic{");
                    stream.WriteLine("\t\tSystem.loadLibrary(\"gearapp\");");
                stream.WriteLine("\t\t}");
                stream.WriteLine("\t}");
////////////////////////////////////////////////////////////
                stream.Close();
        }
    }
}
