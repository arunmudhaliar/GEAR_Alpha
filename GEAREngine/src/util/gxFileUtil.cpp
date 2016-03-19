#include "gxFileUtil.h"
#include "../core/gxDebug.h"
#include <sys/stat.h>
#include <sys/syslimits.h>
#include <dirent.h>

const gxFileUtil& gxFileUtil::getInstance()
{
    static gxFileUtil instance;
    return instance;
}

gxFileUtil::gxFileUtil()
{
}

bool gxFileUtil::createDirectory(std::string path) const
{
#ifdef _WIN32
    if(MKDIR(path.c_str())==0)
#else
        if(MKDIR(path.c_str(), 0777)==0)
#endif
        {
            return true;
        }
    
    DEBUG_PRINT("Error creating directory %s", path.c_str());
    return false;
}

bool gxFileUtil::isDirecoryExist(std::string path) const
{
    struct stat sb;
    if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        return true;
    }
    
    return false;
}

bool gxFileUtil::isFileExist(std::string path) const
{
    struct stat sb;
    if (stat(path.c_str(), &sb) == 0 && S_ISREG(sb.st_mode))
    {
        return true;
    }
    
    return false;
}

int gxFileUtil::delete_directory_recursive(const char *dirname) const
{
    DIR *dir;
    char buffer[PATH_MAX + 2];
    char *p = buffer;
    const char *src;
    char *end = &buffer[PATH_MAX];
    int ok;
    
    /* Copy directory name to buffer */
    src = dirname;
    while (p < end  &&  *src != '\0') {
        *p++ = *src++;
    }
    *p = '\0';
    
    /* Open directory stream */
    dir = opendir (dirname);
    if (dir != NULL) {
        struct dirent *ent;
        
        /* Print all files and directories within the directory */
        while ((ent = readdir (dir)) != NULL) {
            char *q = p;
            char c;
            
            /* Get final character of directory name */
            if (buffer < q) {
                c = q[-1];
            } else {
                c = ':';
            }
            
            /* Append directory separator if not already there */
            if (c != ':'  &&  c != '/'  &&  c != '\\') {
                *q++ = '/';
            }
            
            /* Append file name */
            src = ent->d_name;
            while (q < end  &&  *src != '\0') {
                *q++ = *src++;
            }
            *q = '\0';
            
            /* Decide what to do with the directory entry */
            switch (ent->d_type) {
                case DT_REG:
                {
                    chmod(buffer, 777);
                    int status = remove(buffer);
                    if(status!=0)
                    {
                        printf("Errror deleting file %s\n", buffer);
                        return status;
                    }
                }
                    break;
                    
                case DT_DIR:
                {
                    /* Scan sub-directory recursively */
                    if (strcmp (ent->d_name, ".") != 0  &&  strcmp (ent->d_name, "..") != 0)
                    {
                        int ret_val = delete_directory_recursive(buffer);
                        if(ret_val!=0)
                        {
                            printf("Error deleting folder %s\n", buffer);
                            return ret_val;
                        }
                        
                        int status = rmdir(buffer);
                        if(status!=0)
                            return status;
                    }
                }
                    break;
                    
                default:
                    /* Do not device entries */
                    /*NOP*/;
            }
            
        }
        
        closedir (dir);
        ok = 0;
        
    } else {
        /* Could not open directory */
        printf ("Cannot open directory %s\n", dirname);
        ok = 1;
    }
    
    return ok;
}

bool gxFileUtil::deleteDirectory(const std::string& path) const
{
    int status = delete_directory_recursive(path.c_str());
    if(status==0)
    {
        status = rmdir(path.c_str());
        if(status!=0)
            printf("Error deleting directory %s\n", path.c_str());
    }
    return status==0;
}