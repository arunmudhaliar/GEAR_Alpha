#ifndef GXUTIL_H
#define GXUTIL_H

#include <string.h>

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
private:
	static char g_directoryPath[512];
	static char g_filePath[512];
};

#endif