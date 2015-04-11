using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading;
using System.Windows.Forms;

namespace AndroidProjectMaker
{
    class Program
    {
        static int g_iExitCode = 0;
        static mainform g_cGUI_mainform;

        static int Main(string[] args)
        {
            //string strCmdText;
            //strCmdText = "mkdir c://TempAndroid";
            //System.Diagnostics.Process.Start("CMD.exe", strCmdText);
            string gearprojectDirectory = args[1];
            string os_platform = args[2];
            string platform_command_prefix = "";

            string cmd_android = "android.bat";
            string cmd_adb = "adb.exe";

            if (os_platform == "win32")
            {
                cmd_android = "android.bat";
                cmd_adb = "adb.exe";
            }
            else if (os_platform == "macos")
            {
                cmd_android = "android";
                cmd_adb = "adb";
            }


            if (os_platform == "win32")
                platform_command_prefix = "";
            else if (os_platform == "macos")
                platform_command_prefix = "sh ";

            g_cGUI_mainform = new mainform();
            g_cGUI_mainform.Show();
            g_cGUI_mainform.setMessage("Checking Environment Variables");

            string app_bundle_identifier = "com.gear.gearapp";
            string app_name = "gearApp";
            string rootDirectory = args[0];
            if(Directory.Exists(rootDirectory + "//TempAndroid"))
                Directory.Delete(rootDirectory + "//TempAndroid", true);
            DirectoryInfo dirinfo = Directory.CreateDirectory(rootDirectory+"//TempAndroid");

            string android_sdk_root = Environment.GetEnvironmentVariable("ANDROID_ROOT");
            if (android_sdk_root == null)
                return -99;     //ANDROID_ROOT variable not set


            g_cGUI_mainform.setMessage("Creating Android Project");
            string command_buffer="";
            command_buffer = platform_command_prefix + android_sdk_root + "//tools//" + cmd_android;
            command_buffer += " create project --target 2 ";
            command_buffer += "--name " + app_name + " ";
            command_buffer += "--path " + rootDirectory + "//TempAndroid ";
            command_buffer += "--activity MainActivity ";
            command_buffer += "--package " + app_bundle_identifier;

            ExecuteCommandSync(command_buffer);
            if (g_iExitCode != 0)
            {
                g_cGUI_mainform.setMessage("Error creating android project");
                return -98;
            }

            AndroidManifest.createAndroidManifestFile(rootDirectory + "//TempAndroid", app_bundle_identifier, app_name);
            MainActivity.createMainActivityFile(rootDirectory + "//TempAndroid", app_bundle_identifier);

            //Directory.CreateDirectory(rootDirectory + "//TempAndroid//libs//armeabi//");

            //copy the required files
            //File.Copy(rootDirectory + "//..//AndroidApp//gearApp//libs//armeabi//libgearapp.so",
            //    rootDirectory + "//TempAndroid//libs//armeabi//libgearapp.so", true);
            //File.Copy(rootDirectory + "//..//AndroidApp//gearApp//libs//armeabi//libGEAREngine.dll.so",
            //    rootDirectory + "//TempAndroid//libs//armeabi//libGEAREngine.dll.so", true);

            Directory.CreateDirectory(rootDirectory + "//TempAndroid//assets");
            Directory.CreateDirectory(rootDirectory + "//TempAndroid//jni");

            //jni folder
            foreach (string newPath in Directory.GetFiles(rootDirectory + "//jni_source", "*.*",
            SearchOption.AllDirectories))
                File.Copy(newPath, newPath.Replace(rootDirectory + "//jni_source", rootDirectory + "//TempAndroid//jni"), true);

#if COPY_META_TO_ASSET
            //Copy all the files & Replaces any files with the same name
            foreach (string newPath in Directory.GetFiles(gearprojectDirectory + "//MetaData", "*.*",
                SearchOption.AllDirectories))
                File.Copy(newPath, newPath.Replace(gearprojectDirectory + "//MetaData", rootDirectory + "//TempAndroid//assets"), true);
#endif
            /*
            //project update
            //android update project --name <project_name> --target <target_ID> --path <path_to_your_project>
            command_buffer = android_sdk_root + "//tools//android.bat ";
            command_buffer += "update project --name " + app_name + " ";
            command_buffer += "--target 2 ";
            command_buffer += "--path " + rootDirectory + "//TempAndroid ";
            ExecuteCommandSync(command_buffer);
            */

            g_cGUI_mainform.setMessage("Compiling...0");
            //NDK BUILD
            string ndk_home = Environment.GetEnvironmentVariable("NDK_HOME");
            if (ndk_home == null)
                return -97;     //NDK_HOME variable not set

            command_buffer = platform_command_prefix + ndk_home + "\\ndk-build --directory=" + rootDirectory + "//TempAndroid";
            
            ExecuteCommandSync(command_buffer);
            if (g_iExitCode != 0)
            {
                g_cGUI_mainform.setMessage("Error in Compilation 1");
                return -89;
            }

            g_cGUI_mainform.setMessage("Compiling...1");
            //ant build
            string ant_home = Environment.GetEnvironmentVariable("ANT_HOME");
            if (ant_home == null)
                return -96;     //ANT_HOME variable not set

            /*
            //version override
            #if DEBUG
            command_buffer=ant_home+"\\ant -DVersion.Code=100 -DVersion.Name=1.0.0 set-version-using-commandline-args debug -buildfile "+rootDirectory+"//TempAndroid//build.xml";
            #else
            command_buffer=ant_home+"\\ant -DVersion.Code=100 -DVersion.Name=1.0.0 set-version-using-commandline-args release -buildfile "+rootDirectory+"//TempAndroid//build.xml";
            #endif
             * */
#if DEBUG
            command_buffer = platform_command_prefix + ant_home + "\\ant debug -buildfile " + rootDirectory + "//TempAndroid//build.xml";
#else
            command_buffer = platform_command_prefix + ant_home+"\\ant release -buildfile "+rootDirectory+"//TempAndroid//build.xml";
#endif
            ExecuteCommandSync(command_buffer);
            if (g_iExitCode != 0)
            {
                g_cGUI_mainform.setMessage("Error in Compilation 2");
                return -88;
            }

#if !DEBUG
            g_cGUI_mainform.setMessage("JarSigner");
            command_buffer = "jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 -keystore " + rootDirectory + "//android_keystore//debug.keystore  -storepass android " + rootDirectory + "//TempAndroid//bin//" + app_name + "-release-unsigned.apk androiddebugkey";
            ExecuteCommandSync(command_buffer);
            if (g_iExitCode != 0)
            {
                g_cGUI_mainform.setMessage("Error while signing the apk");
                return -87;
            }
#endif

            g_cGUI_mainform.setMessage("Pushing files to device");
            //Pushing files to device
            string[] necessaryfiles = { "out.exe", "MonoGEAR.dll" };
            foreach (string file_str in necessaryfiles)
            {
#if DEBUG
                command_buffer = android_sdk_root + "//platform-tools//" + cmd_adb +" -d push " + rootDirectory + "//Debug//" + file_str + " //sdcard//gear//" + file_str;
#else
                command_buffer = android_sdk_root + "//platform-tools//" + cmd_adb +" -d push " + rootDirectory + "//Release//" + file_str + " //sdcard//gear//" + file_str;
#endif
                ExecuteCommandSync(command_buffer);
                if (g_iExitCode != 0)
                {
                    ExecuteCommandSync("adb kill-server");
                    g_cGUI_mainform.setMessage("USB device error while pushing files to device.");
                    return -98;
                }
            }

            //Copy all the metadata files & Replaces any files with the same name
            foreach (string newPath in Directory.GetFiles(gearprojectDirectory + "//MetaData", "*.*", SearchOption.AllDirectories))
            {
                command_buffer = android_sdk_root + "//platform-tools//" + cmd_adb + " -d push " + newPath + " //sdcard//gear//MetaData//" + Path.GetFileName(newPath);
                ExecuteCommandSync(command_buffer);
                if (g_iExitCode != 0)
                {
                    ExecuteCommandSync("adb kill-server");
                    g_cGUI_mainform.setMessage("USB device error while pushing meta data files to device.");
                    return -99;
                }
            }

            //Copy all the scenes files & Replaces any files with the same name
            foreach (string newPath in Directory.GetFiles(gearprojectDirectory + "//Assets", "*.gearscene", SearchOption.AllDirectories))
            {
                string relativePath = newPath;
                relativePath=relativePath.Replace(gearprojectDirectory + "//Assets", "");
                relativePath=relativePath.Replace("\\", "//");

                command_buffer = android_sdk_root + "//platform-tools//" + cmd_adb + " -d push " + newPath + " //sdcard//gear//scenes" + relativePath;
                ExecuteCommandSync(command_buffer);
                if (g_iExitCode != 0)
                {
                    ExecuteCommandSync("adb kill-server");
                    g_cGUI_mainform.setMessage("USB device error while pushing scenes files to device.");
                    return -99;
                }
            }

            //current scene
            if (File.Exists(gearprojectDirectory + "//ProjectSettings//currentscene"))
            {
                command_buffer = android_sdk_root + "//platform-tools//" + cmd_adb + " -d push " + gearprojectDirectory + "//ProjectSettings//currentscene" + " //sdcard//gear//currentscene";
                ExecuteCommandSync(command_buffer);
                if (g_iExitCode != 0)
                {
                    ExecuteCommandSync("adb kill-server");
                    g_cGUI_mainform.setMessage("USB device error while pushing current scenes file to device.");
                    return -99;
                }
            }

            g_cGUI_mainform.setMessage("Pushing to device");
            //Pushing to device
#if DEBUG
            command_buffer = android_sdk_root + "//platform-tools//" + cmd_adb + " -d install -r " + rootDirectory + "//TempAndroid//bin//" + app_name + "-debug.apk";
#else
            command_buffer=android_sdk_root+"//platform-tools//" + cmd_adb + " -d install -r "+rootDirectory + "//TempAndroid//bin//"+app_name+"-release-unsigned.apk";
#endif
            ExecuteCommandSync(command_buffer);
            if (g_iExitCode != 0)
            {
                ExecuteCommandSync("adb kill-server");
                g_cGUI_mainform.setMessage("USB device error while pushing apk to device.");
                return -86;
            }
            ExecuteCommandSync("adb kill-server");

            g_cGUI_mainform.setMessage("Executing on device");
            //Executing on device
            command_buffer = android_sdk_root + "//platform-tools//" + cmd_adb + " shell am start -n " + app_bundle_identifier + "/" + app_bundle_identifier + ".MainActivity";
            ExecuteCommandSync(command_buffer);
            if (g_iExitCode != 0)
            {
                ExecuteCommandSync("adb kill-server");
                g_cGUI_mainform.setMessage("Error opening the application on device.");
                return -85;
            }
            ExecuteCommandSync("adb kill-server");

            return 0;
        }

        /// <span class="code-SummaryComment"><summary></span>
        /// Executes a shell command synchronously.
        /// <span class="code-SummaryComment"></summary></span>
        /// <span class="code-SummaryComment"><param name="command">string command</param></span>
        /// <span class="code-SummaryComment"><returns>string, as output of the command.</returns></span>
        public static void ExecuteCommandSync(object command)
        {
            //try
            //{
            //    // create the ProcessStartInfo using "cmd" as the program to be run,
            //    // and "/c " as the parameters.
            //    // Incidentally, /c tells cmd that we want it to execute the command that follows,
            //    // and then exit.
            //    System.Diagnostics.ProcessStartInfo procStartInfo =
            //        new System.Diagnostics.ProcessStartInfo("cmd", "/c " + command);

            //    // The following commands are needed to redirect the standard output.
            //    // This means that it will be redirected to the Process.StandardOutput StreamReader.
            //    procStartInfo.RedirectStandardOutput = true;
            //    procStartInfo.UseShellExecute = false;
            //    // Do not create the black window.
            //    procStartInfo.CreateNoWindow = true;
            //    // Now we create a process, assign its ProcessStartInfo and start it
            //    System.Diagnostics.Process proc = new System.Diagnostics.Process();
            //    proc.StartInfo = procStartInfo;
            //    proc.Start();
            //    proc.WaitForExit(10 * 1000);
            //    // Get the output into a string
            //    string result = proc.StandardOutput.ReadToEnd();
            //    // Display the command output.
            //    Console.WriteLine(result);
            //}
            //catch (Exception objException)
            //{
            //    // Log the exception
            //}
            try
            {
                using (System.Diagnostics.Process process = new System.Diagnostics.Process())
                {
                    int timeout = 3 * 60 * 1000;    //3 minutes
                    g_iExitCode = 0;

                    System.Diagnostics.ProcessStartInfo procStartInfo =
                        new System.Diagnostics.ProcessStartInfo("cmd", "/c " + command);

                    process.StartInfo = procStartInfo;
                    process.StartInfo.UseShellExecute = false;
                    process.StartInfo.RedirectStandardOutput = true;
                    process.StartInfo.RedirectStandardError = true;
                    process.StartInfo.CreateNoWindow = true;

                    StringBuilder output = new StringBuilder();
                    StringBuilder error = new StringBuilder();

                    using (AutoResetEvent outputWaitHandle = new AutoResetEvent(false))
                    using (AutoResetEvent errorWaitHandle = new AutoResetEvent(false))
                    {
                        process.OutputDataReceived += (sender, e) =>
                        {
                            if (e.Data == null)
                            {
                                outputWaitHandle.Set();
                            }
                            else
                            {
                                output.AppendLine(e.Data);
                                Console.WriteLine(e.Data);
                            }
                        };
                        process.ErrorDataReceived += (sender, e) =>
                        {
                            if (e.Data == null)
                            {
                                errorWaitHandle.Set();
                            }
                            else
                            {
                                error.AppendLine(e.Data);
                                Console.WriteLine(e.Data);
                            }
                        };

                        process.Start();

                        process.BeginOutputReadLine();
                        process.BeginErrorReadLine();

                        if (process.WaitForExit(timeout) &&
                            outputWaitHandle.WaitOne(timeout) &&
                            errorWaitHandle.WaitOne(timeout))
                        {
                            // Process completed. Check process.ExitCode here.
                        }
                        else
                        {
                            // Timed out.
                            //process.Kill();
                        }
                    }

                    g_iExitCode = process.ExitCode;
                }
            }
            catch (Exception objException)
            {
                g_iExitCode = -999;
                Console.WriteLine(objException);
                // Log the exception
            }
        }

        /// <span class="code-SummaryComment"><summary></span>
        /// Execute the command Asynchronously.
        /// <span class="code-SummaryComment"></summary></span>
        /// <span class="code-SummaryComment"><param name="command">string command.</param></span>
        public static void ExecuteCommandAsync(string command)
        {
            try
            {
                //Asynchronously start the Thread to process the Execute command request.
                Thread objThread = new Thread(new ParameterizedThreadStart(ExecuteCommandSync));
                //Make the thread as background thread.
                objThread.IsBackground = true;
                //Set the Priority of the thread.
                objThread.Priority = ThreadPriority.AboveNormal;
                //Start the thread.
                objThread.Start(command);
            }
            catch (ThreadStartException objException)
            {
                // Log the exception
            }
            catch (ThreadAbortException objException)
            {
                // Log the exception
            }
            catch (Exception objException)
            {
                // Log the exception
            }
        }
    }
}
