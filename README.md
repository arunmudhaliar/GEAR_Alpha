v0.0.1

References
----------
http://www.ntu.edu.sg/home/ehchua/programming/opengl/CG_BasicsTheory.html
http://renderingwonders.wordpress.com/tag/glsl-2/

http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/#Basic_shadowmap

GI
http://www.kevinbeason.com/smallpt/

//harware skinning
http://www.antongerdelan.net/opengl/

Created dev Branch
Set dev to main branch

//stack trace on crash
http://oroboro.com/stack-trace-on-crash/

//STL EXPORT
http://support.microsoft.com/kb/168958/en-us

//GLSL
http://www.shaderific.com/glsl-functions/

//Texture Atlas Generator
https://github.com/lukaszdk/texture-atlas-generator/blob/master/AtlasGenerator.java

//shadow mapping
http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/

Configuring the dev environment for MACOS
-----------------------------------------
1. Check out the dev branch from git
2. Open Terminal and go to the root folder
3. Run sh ./setup_mono_osx.sh
4. On Success it must create monoForMACOS with bin, etc, include, lib & share folders
6. Check if ./macosProj/GEARInternal/mono-prerequisite/monoinstallpath.txt file is created or not.
5. Check the monoForMACOS path is exported to ./macosProj/GEARInternal/mono-prerequisite/monoinstallpath.txt
6. Open the ./macosProj/GEARWorkSpace.xcworkspace in xcode
7. Happy coding....