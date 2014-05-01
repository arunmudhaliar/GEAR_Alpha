using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading;

namespace AndroidProjectMaker
{
    class Program
    {
        static int Main(string[] args)
        {
            //string strCmdText;
            //strCmdText = "mkdir c://TempAndroid";
            //System.Diagnostics.Process.Start("CMD.exe", strCmdText);

            string app_bundle_identifier = "com.gear.gearapp";
            string app_name = "gearApp";
            string rootDirectory = ".";
            DirectoryInfo dirinfo = Directory.CreateDirectory(rootDirectory+"//TempAndroid");

            string android_sdk_root = Environment.GetEnvironmentVariable("ANDROID_ROOT");
            if (android_sdk_root == null)
                return -99;     //ANDROID_ROOT variable not set

            string command_buffer;
			command_buffer = android_sdk_root+"//tools//android.bat ";
			command_buffer += "create project --target 2 ";
            command_buffer += "--name " + app_name + " ";
            command_buffer += "--path " + rootDirectory + "//TempAndroid ";
            command_buffer += "--activity MainActivity ";
			command_buffer += "--package "+app_bundle_identifier;

            ExecuteCommandSync(command_buffer);

            AndroidManifest.createAndroidManifestFile(rootDirectory + "//TempAndroid", app_bundle_identifier, app_name);
            MainActivity.createMainActivityFile(rootDirectory + "//TempAndroid", app_bundle_identifier);
            /*
            //project update
            //android update project --name <project_name> --target <target_ID> --path <path_to_your_project>
            command_buffer = android_sdk_root + "//tools//android.bat ";
            command_buffer += "update project --name " + app_name + " ";
            command_buffer += "--target 2 ";
            command_buffer += "--path " + rootDirectory + "//TempAndroid ";
            ExecuteCommandSync(command_buffer);
            */

            //NDK BUILD
            string ndk_home = Environment.GetEnvironmentVariable("NDK_HOME");
            if (ndk_home == null)
                return -97;     //NDK_HOME variable not set

            command_buffer = ndk_home + "\\ndk-build --directory="+rootDirectory + "//TempAndroid";
            ExecuteCommandSync(command_buffer);

            //ant build
            string ant_home = Environment.GetEnvironmentVariable("ANT_HOME");
            if (ant_home == null)
                return -98;     //ANT_HOME variable not set

            /*
            //version override
            #if DEBUG
            command_buffer=ant_home+"\\ant -DVersion.Code=100 -DVersion.Name=1.0.0 set-version-using-commandline-args debug -buildfile "+rootDirectory+"//TempAndroid//build.xml";
            #else
            command_buffer=ant_home+"\\ant -DVersion.Code=100 -DVersion.Name=1.0.0 set-version-using-commandline-args release -buildfile "+rootDirectory+"//TempAndroid//build.xml";
            #endif
             * */
#if DEBUG
            command_buffer = ant_home + "\\ant debug -buildfile " + rootDirectory + "//TempAndroid//build.xml";
#else
            command_buffer=ant_home+"\\ant release -buildfile "+rootDirectory+"//TempAndroid//build.xml";
#endif
            ExecuteCommandSync(command_buffer);


            //Pushing to device
			#if DEBUG
			command_buffer=android_sdk_root+"//platform-tools//adb.exe -d install -r "+rootDirectory + "//TempAndroid//bin//"+app_name+"-debug.apk";
			#else
			command_buffer=android_sdk_root+"//platform-tools//adb.exe -d install -r "+rootDirectory + "//TempAndroid//bin//"+app_name+"-release-unsigned.apk", responsebuffer);
			#endif
            ExecuteCommandSync(command_buffer);

			//Executing on device
            command_buffer = android_sdk_root + "//platform-tools//adb.exe shell am start -n " + app_bundle_identifier + "/" + app_bundle_identifier + ".MainActivity";
            ExecuteCommandSync(command_buffer);
            return 0;
        }

        /// <span class="code-SummaryComment"><summary></span>
        /// Executes a shell command synchronously.
        /// <span class="code-SummaryComment"></summary></span>
        /// <span class="code-SummaryComment"><param name="command">string command</param></span>
        /// <span class="code-SummaryComment"><returns>string, as output of the command.</returns></span>
        public static void ExecuteCommandSync(object command)
        {
            try
            {
                // create the ProcessStartInfo using "cmd" as the program to be run,
                // and "/c " as the parameters.
                // Incidentally, /c tells cmd that we want it to execute the command that follows,
                // and then exit.
                System.Diagnostics.ProcessStartInfo procStartInfo =
                    new System.Diagnostics.ProcessStartInfo("cmd", "/c " + command);

                // The following commands are needed to redirect the standard output.
                // This means that it will be redirected to the Process.StandardOutput StreamReader.
                procStartInfo.RedirectStandardOutput = true;
                procStartInfo.UseShellExecute = false;
                // Do not create the black window.
                procStartInfo.CreateNoWindow = true;
                // Now we create a process, assign its ProcessStartInfo and start it
                System.Diagnostics.Process proc = new System.Diagnostics.Process();
                proc.StartInfo = procStartInfo;
                proc.Start();
                // Get the output into a string
                string result = proc.StandardOutput.ReadToEnd();
                // Display the command output.
                Console.WriteLine(result);
            }
            catch (Exception objException)
            {
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
