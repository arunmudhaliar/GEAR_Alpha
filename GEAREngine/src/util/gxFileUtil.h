#pragma once

#include <string>
#include "../core/basicIncludes.h"

#ifdef GEAR_WINDOWS
#include <direct.h>
#include <io.h>
#define MKDIR _mkdir
#define CHMOD _chmod
#else
#define MKDIR mkdir
#define CHMOD chmod
#endif

class DECLSPEC gxFileUtil
{
public:
    static const gxFileUtil& getInstance();
    
    bool createDirectory(std::string path) const;
    bool isDirecoryExist(std::string path) const;
    bool isFileExist(std::string path) const;
    bool deleteDirectory(const std::string& path) const;
    
private:
    gxFileUtil();
    int delete_directory_recursive(const char *dirname) const;
};
