using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace CSharpProjectMaker
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 2)
            {
                Console.WriteLine("Usage: CSharpProjectMaker <app name> <working directory>");
                return;
            }

            Console.WriteLine("CSharpProjectMaker Version 1.0.0");

            Guid guid = Guid.NewGuid();
            create_sln(args[0], args[1], guid);
            create_csproj(args[0], args[1], guid);
        }

        static void create_sln(string slnfilename, string workingdir, Guid guid)
        {
            StreamWriter stream = new StreamWriter(workingdir+"//"+slnfilename+".sln");

            stream.WriteLine();
            stream.WriteLine("Microsoft Visual Studio Solution File, Format Version 11.00");
            stream.WriteLine("# Visual Studio 2010");
            stream.WriteLine("Project(\"{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}\") = \"" + slnfilename + "\", \"" + slnfilename + ".csproj\", \"{" + guid + "}\"");
            stream.WriteLine("EndProject");
            stream.WriteLine("Global");
            stream.WriteLine("\tGlobalSection(SolutionConfigurationPlatforms) = preSolution");
            stream.WriteLine("\t\tDebug|x86 = Debug|x86");
            stream.WriteLine("\t\tRelease|x86 = Release|x86");
            stream.WriteLine("\tEndGlobalSection");
            stream.WriteLine("\tGlobalSection(ProjectConfigurationPlatforms) = postSolution");
            stream.WriteLine("\t\t{" + guid + "}.Debug|x86.ActiveCfg = Debug|x86");
            stream.WriteLine("\t\t{" + guid + "}.Debug|x86.Build.0 = Debug|x86");
            stream.WriteLine("\t\t{" + guid + "}.Release|x86.ActiveCfg = Release|x86");
            stream.WriteLine("\t\t{" + guid + "}.Release|x86.Build.0 = Release|x86");
            stream.WriteLine("\tEndGlobalSection");
            stream.WriteLine("\tGlobalSection(MonoDevelopProperties) = preSolution");
            stream.WriteLine("\t\tStartupItem = " + slnfilename + ".csproj");
            stream.WriteLine("\tEndGlobalSection");
            stream.WriteLine("EndGlobal");
            stream.Close();
        }

        static void create_csproj(string cs_projfilename, string workingdir, Guid guid)
        {
            StreamWriter stream = new StreamWriter(workingdir + "//" + cs_projfilename + ".csproj");

            stream.WriteLine("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
            stream.WriteLine("<Project DefaultTargets=\"Build\" ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">");

            stream.WriteLine("\t<PropertyGroup>");
            stream.WriteLine("\t\t<Configuration Condition=\" '$(Configuration)' == '' \">Debug</Configuration>");
            stream.WriteLine("\t\t<Platform Condition=\" '$(Platform)' == '' \">x86</Platform>");
            stream.WriteLine("\t\t<ProductVersion>10.0.0</ProductVersion>");
            stream.WriteLine("\t\t<SchemaVersion>2.0</SchemaVersion>");
            stream.WriteLine("\t\t<ProjectGuid>{" + guid + "}</ProjectGuid>");
            stream.WriteLine("\t\t<OutputType>Exe</OutputType>");
            stream.WriteLine("\t\t<RootNamespace>MonoGEAR</RootNamespace>");
            stream.WriteLine("\t\t<AssemblyName>" + cs_projfilename + "</AssemblyName>");
            stream.WriteLine("\t\t<TargetFrameworkVersion>v4.0</TargetFrameworkVersion>");
            stream.WriteLine("\t</PropertyGroup>");

            stream.WriteLine("\t<PropertyGroup Condition=\" '$(Configuration)|$(Platform)' == 'Debug|x86' \">");
            stream.WriteLine("\t\t<DebugSymbols>True</DebugSymbols>");
            stream.WriteLine("\t\t<DebugType>full</DebugType>");
            stream.WriteLine("\t\t<Optimize>False</Optimize>");
            stream.WriteLine("\t\t<OutputPath>bin\\Debug</OutputPath>");
            stream.WriteLine("\t\t<DefineConstants>DEBUG;</DefineConstants>");
            stream.WriteLine("\t\t<ErrorReport>prompt</ErrorReport>");
            stream.WriteLine("\t\t<WarningLevel>4</WarningLevel>");
            stream.WriteLine("\t\t<PlatformTarget>x86</PlatformTarget>");
            stream.WriteLine("\t\t<Externalconsole>True</Externalconsole>");
            stream.WriteLine("\t</PropertyGroup>");

            stream.WriteLine("\t<PropertyGroup Condition=\" '$(Configuration)|$(Platform)' == 'Release|x86' \">");
            stream.WriteLine("\t\t<DebugType>none</DebugType>");
            stream.WriteLine("\t\t<Optimize>True</Optimize>");
            stream.WriteLine("\t\t<OutputPath>bin\\Release</OutputPath>");
            stream.WriteLine("\t\t<ErrorReport>prompt</ErrorReport>");
            stream.WriteLine("\t\t<WarningLevel>4</WarningLevel>");
            stream.WriteLine("\t\t<PlatformTarget>x86</PlatformTarget>");
            stream.WriteLine("\t\t<Externalconsole>True</Externalconsole>");
            stream.WriteLine("\t</PropertyGroup>");

            stream.WriteLine("\t<ItemGroup>");
            stream.WriteLine("\t\t<Reference Include=\"System\" />");
            stream.WriteLine("\t\t<Reference Include=\"System.Core\" />");
            stream.WriteLine("\t\t<Reference Include=\"System.Xml\" />");
            stream.WriteLine("\t\t<Reference Include=\"System.Xml.Linq\" />");

            stream.WriteLine("\t</ItemGroup>");
            stream.WriteLine("\t<ItemGroup>");

            traverseFolderFor_CS_Files(workingdir, workingdir, stream);

            stream.WriteLine("\t</ItemGroup>");
            stream.WriteLine("\t<Import Project=\"$(MSBuildBinPath)\\Microsoft.CSharp.targets\" />");
            stream.WriteLine("</Project>");

            stream.Close();


        }

        static void traverseFolderFor_CS_Files(string path, string workingdirectory, StreamWriter stream)
        {
            //search for bitmap files
            string[] files = Directory.GetFiles(path, "*.cs");
            foreach (string file in files)
            {
                string relativefilepath=file.Replace(workingdirectory+"\\", "");
                stream.WriteLine("\t\t<Compile Include=\"" + relativefilepath + "\" />");
            }

            //search for folders
            string[] dirs = Directory.GetDirectories(path, "*.*", SearchOption.TopDirectoryOnly);
            foreach (string dir in dirs)
            {
                traverseFolderFor_CS_Files(dir, workingdirectory, stream);
            }

        }
    }
}
