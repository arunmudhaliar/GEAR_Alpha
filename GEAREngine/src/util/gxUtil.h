#ifndef GXUTIL_H
#define GXUTIL_H

#include <string.h>
#include "../core/vector3.h"

class gxUtil
{
public:
	static const char* getFolderPathFromFileName(const char* filename)
	{
		char* directorPathPtr=NULL;
		strcpy(g_directoryPath, filename);
		char* last_slash=NULL;
		char* fs=g_directoryPath;
		char* bs=g_directoryPath;
		fs=strrchr(fs, '\\');
		bs=strrchr(bs, '/');
		int fs_len=strlen(g_directoryPath);
		if(fs)
		{
			fs_len=strlen(fs);
		}
		int bs_len=strlen(g_directoryPath);
		if(bs)
		{
			bs_len=strlen(bs);
		}

		if(fs_len<bs_len)
		{
			fs[0]='\0';
			last_slash=&fs[0];
		}
		else
		{
			bs[0]='\0';
			last_slash=&bs[0];
		}

		if(last_slash)
		{
			//strcpy(last_slash, "");
			//last_slash[0]='\0';
			directorPathPtr=g_directoryPath;
		}

		return directorPathPtr;
	}

	static const char* getFileNameFromPath(const char* path)
	{
		strcpy(g_filePath, path);
		const char* onlyfinename=strrchr(g_filePath, '\\');
		if(onlyfinename==NULL)
		{
			onlyfinename=strrchr(g_filePath, '/');
		}
		if(onlyfinename==NULL)
			onlyfinename=g_filePath;
		else
			onlyfinename=&onlyfinename[1];

		return onlyfinename;
	}

	static bool twoLineSegmentIntersection(vector3f p1, vector3f p2, vector3f p3, vector3f p4, vector3f& pa, vector3f& pb, float& ua, float& ub)
	{
		//http://paulbourke.net/geometry/pointlineplane/
	   vector3f p13,p43,p21;
	   float d1343,d4321,d1321,d4343,d2121;
	   float numer,denom;

	   p13.x = p1.x - p3.x;
	   p13.y = p1.y - p3.y;
	   p13.z = p1.z - p3.z;
	   p43.x = p4.x - p3.x;
	   p43.y = p4.y - p3.y;
	   p43.z = p4.z - p3.z;
	   if (ABS(p43.x) < 0.00000001f && ABS(p43.y) < 0.00000001f && ABS(p43.z) < 0.00000001f)
		  return false;
	   p21.x = p2.x - p1.x;
	   p21.y = p2.y - p1.y;
	   p21.z = p2.z - p1.z;
	   if (ABS(p21.x) < 0.00000001f && ABS(p21.y) < 0.00000001f && ABS(p21.z) < 0.00000001f)
		  return false;

	   d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
	   d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
	   d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
	   d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
	   d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;

	   denom = d2121 * d4343 - d4321 * d4321;
	   if (ABS(denom) < 0.00000001f)
		  return false;
	   numer = d1343 * d4321 - d1321 * d4343;

	   ua = numer / denom;
	   ub = (d1343 + d4321 * (ua)) / d4343;

	   pa.x = p1.x + ua * p21.x;
	   pa.y = p1.y + ua * p21.y;
	   pa.z = p1.z + ua * p21.z;
	   pb.x = p3.x + ub * p43.x;
	   pb.y = p3.y + ub * p43.y;
	   pb.z = p3.z + ub * p43.z;

		return true;
	}
private:
	static char g_directoryPath[512];
	static char g_filePath[512];
};

#endif