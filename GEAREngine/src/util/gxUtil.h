#pragma once

#include <string>
#include "../core/vector3.h"

class DECLSPEC gxUtil
{
public:
	static const char* getFolderPathFromFileName(const char* filename)
	{
		GX_STRCPY(g_directoryPath, filename);
		for(int x=(int)strlen(g_directoryPath)-1;x>=0;x--)
		{
			if(g_directoryPath[x]=='\\' || g_directoryPath[x]=='/')
			{
				g_directoryPath[x]='\0';
				break;
			}
		}

		return g_directoryPath;
	}

	static const char* getFileNameFromPath(const char* path)
	{
		for(int x=(int)strlen(path)-1;x>=0;x--)
		{
			if(path[x]=='\\' || path[x]=='/')
			{
				strcpy(g_directoryPath, &path[x+1]);
				return g_directoryPath;
			}
		}

		return path;
	}

	static bool lineSegmentAndPlaneIntersection(vector3f p1, vector3f p2, vector3f planeNormal, vector3f pointOnPlane, float& u)
	{
		vector3f p3_p1(pointOnPlane-p1);
		vector3f p2_p1(p2-p1);

		float n = planeNormal.dot(p3_p1);
		float d = planeNormal.dot(p2_p1);

		if(abs(d)<0.00000001f)
			return false;		//line parallel to plane

		u=n/d;
		return true;
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

	static bool GX_IS_EXTENSION(const char* path, const char* ext)
	{
		const char* extension_begin=strrchr(path, '.');
		if(extension_begin==NULL) return NULL;
		return (strcmp(extension_begin, ext)==0);
	}

	// Note: This function returns a pointer to a substring of the original string.
	// If the given string was allocated dynamically, the caller must not overwrite
	// that pointer with the returned value, since the original pointer must be
	// deallocated using the same allocator with which it was allocated.  The return
	// value must NOT be deallocated using free() etc.
	static char* trimwhitespace(char *str)
	{
		char *end;

		// Trim leading space
		while(isspace(*str)) str++;

		if(*str == 0)  // All spaces?
		return str;

		// Trim trailing space
		end = str + strlen(str) - 1;
		while(end > str && isspace(*end)) end--;

		// Write new null terminator
		*(end+1) = 0;

		return str;
	}

	static bool isSubString(const char* str, const char* substr)
	{
		const char* fond_str=STRCHR_nocase(str, substr[0]);
		if(fond_str==NULL) return false;

		bool bFound=false;

		while(!bFound)
		{
			int sub_strlen=(int)strlen(substr);
			int str_len=(int)strlen(fond_str);
			if(sub_strlen>str_len) return false;

			bFound=true;
			for(int x=0;x<sub_strlen; x++)
			{
				if(tolower(substr[x])!=tolower(*fond_str))
				{
					bFound=false;
					break;
				}
				fond_str++;
			}

			if(bFound)
				return bFound;

			fond_str=STRCHR_nocase(fond_str, substr[0]);
			if(fond_str==NULL) return false;
		}

		return false;
	}

private:

	static const char* STRCHR_nocase(const char* str, char value)
	{
		for(int x=0;x<strlen(str);x++)
		{
			if(tolower(str[x])==tolower(value))
				return &str[x];
		}

		return NULL;
	}

	static char g_directoryPath[512];
	static char g_filePath[512];
};