#pragma once
//
//  objC_util.h
//  Copyright 2017 GEAR3D
//  Gear3D
//
//  Created by Arun Mudaliar.
//
//
#include <string.h>
#include <errno.h>
#include "../renderer/glincludes.h"
#include "../zlib-1.2.4/zlib.h"

class objC_util {
public:
    static const char* getPath(const char* path) {
#if defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE)
        NSString *stringFromChar = [NSString stringWithUTF8String:path];
        NSString *nsStringObject = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:stringFromChar];
        const char* cString = [nsStringObject cStringUsingEncoding:NSASCIIStringEncoding];
        
        return cString;
#else
        return path;
#endif
    }

    static const char* getDocumentPath() {
#if defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE)
        NSString* nsStringObject = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
        return [nsStringObject cStringUsingEncoding:NSASCIIStringEncoding];
#elif defined(GEAR_ANDROID)
        return NULL;
#else
        return NULL;
#endif
    }

#if defined(GEAR_ANDROID)
#ifndef GL_EXT_texture_compression_s3tc
    #define GL_EXT_texture_compression_s3tc
    #define GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
    #define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
    #define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
    #define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3
#endif
#endif
    
    static unsigned int loadTexture(FILE* fp, bool& bAlpha, unsigned int& width, unsigned int& height, unsigned int& bpp) {
        fread(&width, sizeof(int), 1, fp);
        fread(&height, sizeof(int), 1, fp);
        int format = 0;
        fread(&format, sizeof(int), 1, fp);
        
        switch (format) {
            case 1: {
                bpp = 3;
            }
                break;
            case 2: {
                bpp = 4;
                bAlpha = true;
            }
                break;
            case 3: {
                bpp = 2;
            }
                break;
            case 4:{
                bpp = 2;
                bAlpha = true;
            }
                break;
                //
            case 5: {
#if defined (LOG_DEBUG_ENGINE)
                 DEBUG_PRINT("pvrtc RGB format");
#endif 
            }
                break;
            case 6: {
#if defined (LOG_DEBUG_ENGINE)
                DEBUG_PRINT("pvrtc RGBA format");
#endif 
                bAlpha = true;
            }
                break;
            case 7: {
#if defined (LOG_DEBUG_ENGINE)
               DEBUG_PRINT("ATC RGB format");
#endif 
            }
                break;
            case 8: {
#if defined (LOG_DEBUG_ENGINE)
                DEBUG_PRINT("ATC RGBA format");
#endif 
                bAlpha = true;
            }
                break;
            case 9: {
#if defined (LOG_DEBUG_ENGINE)
                DEBUG_PRINT("DXT1 RGB format");
#endif 
            }
                break;
            case 10: {
#if defined (LOG_DEBUG_ENGINE)
                DEBUG_PRINT("DXT1 RGBA format");
#endif 
                bAlpha = true;
            }
                break;
                
            default:
                break;
        }
        
        unsigned char* buffer = NULL;
        unsigned long compressedSz = 0;                         //NOLINT
        unsigned long gl_ext_algo_compressedSz = 0;             //NOLINT
        
        fread(&compressedSz, sizeof(unsigned long), 1, fp);     //NOLINT

        if (format == 5 || format == 6 || format == 7 || format == 8 || format == 9 || format == 10) {
            fread(&gl_ext_algo_compressedSz, sizeof(unsigned long), 1, fp); //NOLINT
        }
        
        unsigned char* compressed_buffer = (unsigned char*)malloc(compressedSz);
        fread(compressed_buffer, compressedSz, 1, fp);
        
        unsigned long un_com_sz = width * height * bpp;         //NOLINT
        if (format < 5) {
            un_com_sz = width * height * bpp;
        } else {
            un_com_sz = gl_ext_algo_compressedSz;
        }
        
        buffer = (unsigned char*)malloc(un_com_sz);

        int ret = uncompress(buffer, &un_com_sz, compressed_buffer, compressedSz);
        if (ret != Z_OK) {
            free(compressed_buffer);
            free(buffer);
            compressed_buffer = NULL;
            buffer = NULL;
            return -1;
        }
        
        if (compressed_buffer) free(compressed_buffer);
        
//        }
//        else
//        {   
//            buffer=(unsigned char*)malloc(compressedSz);
//            fread(buffer, compressedSz, 1, fp);
//        }
        
        unsigned int texID = 0;
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        switch (format) {
            case 1:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
                break;
            case 2:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
                break;
#if !defined(GEAR_WINDOWS)
            case 3:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, buffer);
                break;
            case 4:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, buffer);
                break;
#endif

#ifdef GL_IMG_texture_compression_pvrtc
            case 5: {
                glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG,
                                       width, height, 0, gl_ext_algo_compressedSz, buffer);
            }
                break;
            case 6: {
                glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG,
                                       width, height, 0, gl_ext_algo_compressedSz, buffer);
            }
                break;
#endif

#ifdef GL_AMD_compressed_ATC_texture
            case 7: {
                glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ATC_RGB_AMD,
                                       width, height, 0, gl_ext_algo_compressedSz, buffer);
            }
                break;
            case 8: {
                glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD,
                                       width, height, 0, gl_ext_algo_compressedSz, buffer);
                // GL_ATC_RGBA_EXPLICIT_ALPHA_AMD
                // GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD
            }
                break;
#endif

#ifdef GL_EXT_texture_compression_s3tc
            case 9: {
                glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_S3TC_DXT1_EXT,
                                       width, height, 0, gl_ext_algo_compressedSz, buffer);
            }
                break;
            case 10: {
                glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
                                       width, height, 0, gl_ext_algo_compressedSz, buffer);
            }
                break;
#endif

            default:
            {
#if defined (LOG_DEBUG_ENGINE)
                 DEBUG_PRINT("unsupported format");
#endif 
            }
               
                break;
        }
        
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
        
        if (buffer) free(buffer);
        
        return texID;
    }
    
    static unsigned int loadTexture2(const char* filepath, bool& bAlpha,
                                     unsigned int& width, unsigned int& height, unsigned int& bpp) {
        // replace .png to .tzx
        char out_filename[256];
        strcpy_s(out_filename, sizeof(out_filename), filepath);
#if !defined(GEAR_WINDOWS)
        char* dot_p = strrchr(out_filename, '.');
        strcpy(dot_p, ".tx");   //NOLINT
#endif
        //

        FILE* fp = fopen(getPath(out_filename), "rb");
        
        if (!fp) {
#if defined (LOG_DEBUG_ENGINE)
            DEBUG_PRINT("Error opening file - %s, %s", out_filename, strerror(errno));
#endif
            return 0;
        }
        
        unsigned int retval = loadTexture(fp, bAlpha, width, height, bpp);
        if (retval > 0) {
#if defined (LOG_DEBUG_ENGINE)
            DEBUG_PRINT("texture loaded - %s, w=%d, h=%d, bpp=%d", filepath, width, height, bpp);
#endif
        } else if (retval == -1) {
#if defined (LOG_DEBUG_ENGINE)
            DEBUG_PRINT("texture uncompression error - %s", filepath);
#endif
        } else {
#if defined (LOG_DEBUG_ENGINE)
            DEBUG_PRINT("texture loading error - %s, return value=%d", filepath, retval);
#endif
        }

        fclose(fp);
        
        return retval;
    }
    
#if 0
    static unsigned int loadTexture2(stFD* fd, bool& bAlpha,
                                     unsigned int& width, unsigned int& height, unsigned int& bpp) {
        if (fd == NULL) {
            DEBUG_PRINT("fd is NULL");
            return 0;
        }

        FILE* fp = fdopen(fd->fd, "rb");
        if (!fp) {
#if defined (LOG_DEBUG_ENGINE)
            DEBUG_PRINT("Error opening file - %s, %s", fd->filename, strerror(errno));
#endif
            return 0;
        } else {
#if defined (LOG_DEBUG_ENGINE)
           DEBUG_PRINT("loading %s frm offset %d, length %d", fd->filename, fd->offset, fd->length);
#endif
        }
        fseek(fp, fd->offset, SEEK_SET);
        unsigned int retval = loadTexture(fp, bAlpha, width, height, bpp);
        
        if (retval > 0) {
#if defined (LOG_DEBUG_ENGINE)
           DEBUG_PRINT("texture loaded - %s, w=%d, h=%d, bpp=%d", fd->filename, width, height, bpp);
#endif
        } else if (retval == -1) {
#if defined (LOG_DEBUG_ENGINE)
            DEBUG_PRINT("texture uncompression error - %s", fd->filename);
#endif
        } else {
#if defined (LOG_DEBUG_ENGINE)
            DEBUG_PRINT("texture loading error - %s, return value=%d", fd->filename, retval);
#endif
        }

// #if !defined(GEAR_ANDROID)
        fclose(fp);
// #endif
        
        return retval;
    }
#endif

    static unsigned int loadBuffer(unsigned char* buffer, bool bAlpha,
                                   unsigned int width, unsigned int height, unsigned int bpp) {
        unsigned int texID = 0;
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        switch (bpp) {
            case 16:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, buffer);
                break;
            case 32:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
                break;
            default:
                break;
        }
        
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glDisable(GL_TEXTURE_2D);
        
#if defined (LOG_DEBUG_ENGINE)
        DEBUG_PRINT("texture loaded from buffer - w=%d, h=%d", width, height);
#endif
        return texID;
    }
};
