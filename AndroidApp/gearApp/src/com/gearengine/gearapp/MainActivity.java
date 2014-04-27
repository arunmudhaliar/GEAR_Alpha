package com.gearengine.gearapp;

import com.gearengine.gearapp.R;

import android.os.Bundle;
import android.os.Environment;
import android.app.NativeActivity;
import android.view.Menu;

public class MainActivity extends NativeActivity {

	public boolean firstlaunch=true;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        System.out.println("getExternalStorageDirectory "+ Environment.getExternalStorageDirectory());
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
    @Override
    public void onPause() {
    	super.onPause();
//    	if(firstlaunch==true)
//    	{
//    		firstlaunch=false;
//    		return;
//    	}
//    	mainlib();
    }
    
    @Override
    public void onResume() {
    	super.onResume();
//    	if(firstlaunch==true)
//    	{
//    		firstlaunch=false;
//    		return;
//    	}
//    	mainlib();
    }
    
    public static native int mainlib();
    static{
        System.loadLibrary("gearapp");
    }
}
