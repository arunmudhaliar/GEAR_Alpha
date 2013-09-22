#include "AssetImporter.h"
#include "EditorApp.h"
#include "../../GEAREngine/src/util/gxFile.h"
#include "../../GEAREngine/src/lpng151/png.h"
#include "../../GEAREngine/src/tga/Tga.h"
#include "../../GEAREngine/src/core/gxMetaStructures.h"
#include "../../GEAREngine/src/util/Crc32.h"
#include "../../GEAREngine/src/fbxImporter/fbxImporter.h"
#include "../../GEAREngine/src/util/gxUtil.cpp"

#include <sys/types.h>
#include <assert.h>
#include <dirent.h>
#include <direct.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <CommCtrl.h>

AssetImporter::AssetImporter()
{
}

AssetImporter::~AssetImporter()
{
}

bool AssetImporter::importAssets(const char* assetsfolder, HWND hWndDlg, int progressBarID, int statictextID)
{
	//if metaData folder doesn't exist create it
	char temp_buffer[1024];
	sprintf(temp_buffer, "%s/MetaData", EditorApp::getProjectHomeDirectory());
	if(_mkdir(temp_buffer)==0)
	{
		//created a new metaDirectory
	}

	m_hWndProgress=hWndDlg;
	m_iProgressBarID=progressBarID;
	m_iStatictextID=statictextID;
	m_nAssetsToProcess=0;
	traverseAndCountAssetDirectory(EditorApp::getProjectHomeDirectory());
	SendDlgItemMessage(hWndDlg, m_iProgressBarID, PBM_SETRANGE, 0, MAKELPARAM(0, m_nAssetsToProcess));
	m_nAssetsToProcess=0;
	traverseAssetDirectory(EditorApp::getProjectHomeDirectory());
	SendDlgItemMessage(m_hWndProgress, m_iStatictextID, WM_SETTEXT, 0, (LPARAM)"Complete");
	return true;
}

int AssetImporter::traverseAndCountAssetDirectory(const char *dirname)
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
					bool bPNG=false;
					bool bTGA=false;
					bool bFBX=false;

					if(util::GE_IS_EXTENSION(buffer, ".png") || util::GE_IS_EXTENSION(buffer, ".PNG"))
					{
						bPNG=true;
					}

					if(util::GE_IS_EXTENSION(buffer, ".tga") || util::GE_IS_EXTENSION(buffer, ".TGA"))
					{
						bTGA=true;
					}
					if(util::GE_IS_EXTENSION(buffer, ".fbx") || util::GE_IS_EXTENSION(buffer, ".FBX"))
					{
						bFBX=true;
					}

					if(bPNG || bTGA || bFBX)
					{
						m_nAssetsToProcess++;
					}
				}
                break;

            case DT_DIR:
                /* Scan sub-directory recursively */
                if (strcmp (ent->d_name, ".") != 0  &&  strcmp (ent->d_name, "..") != 0)
				{
                    traverseAndCountAssetDirectory(buffer);
                }
                break;

            default:
                /* Do not device entries */
                /*NOP*/;
            }

        }

        closedir (dir);
        ok = 1;

    } else {
        /* Could not open directory */
        printf ("Cannot open directory %s\n", dirname);
        ok = 0;
    }

    return ok;
}
int AssetImporter::traverseAssetDirectory(const char *dirname)
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
					bool bPNG=false;
					bool bTGA=false;
					bool bFBX=false;

					if(util::GE_IS_EXTENSION(buffer, ".png") || util::GE_IS_EXTENSION(buffer, ".PNG"))
					{
						bPNG=true;
					}

					if(util::GE_IS_EXTENSION(buffer, ".tga") || util::GE_IS_EXTENSION(buffer, ".TGA"))
					{
						bTGA=true;
					}
					if(util::GE_IS_EXTENSION(buffer, ".fbx") || util::GE_IS_EXTENSION(buffer, ".FBX"))
					{
						bFBX=true;
					}

					if(bPNG || bTGA || bFBX)
					{
						bool bCreateMetaFile=false;
						struct stat fst;
						memset(&fst, 0, sizeof(fst));
						if(stat(buffer, &fst)==0) 
						{
							int crc32=Crc32::Calc((unsigned char*)buffer);
							char crcFileName[512];
							sprintf(crcFileName, "%s/%s/%x", EditorApp::getProjectHomeDirectory(), "MetaData", crc32);

							stMetaHeader metaHeader;
							memset(&metaHeader, 0, sizeof(metaHeader));
							gxFile metaFile;
							if(metaFile.OpenFile(crcFileName))
							{
								metaFile.ReadBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));
								metaFile.CloseFile();

								if(metaHeader.lastmodified!=fst.st_mtime || 
									((bPNG || bTGA) && metaHeader.filetype!=eMetaTexture2D) ||
									((bFBX) && metaHeader.filetype!=eMeta3DFile)
									)
								{
									//source file has some modifications so update meta file
									bCreateMetaFile=true;
								}
							}
							else
							{
								//there is no meta file associated with this source file. so create a new meta file
								bCreateMetaFile=true;
							}

							SendDlgItemMessage(m_hWndProgress, m_iStatictextID, WM_SETTEXT, 0, (LPARAM)buffer);

							if(bCreateMetaFile)
							{
								bool bWriteMetaInfo=false;
								if(bPNG && import_png_to_metadata(buffer, crcFileName, fst))
								{
									bWriteMetaInfo=true;
								}

								if(bTGA && import_tga_to_metadata(buffer, crcFileName, fst))
								{
									bWriteMetaInfo=true;
								}

								if(bFBX && import_fbx_to_metadata(buffer, crcFileName, fst))
								{
									bWriteMetaInfo=true;
								}

								if(bWriteMetaInfo)
								{
									//create the meta-info file
									gxFile metaInfoFile;
									char metaInfoFileName[512];
									sprintf(metaInfoFileName, "%s.meta", buffer);
									if(metaInfoFile.OpenFile(metaInfoFileName, gxFile::FILE_w))
									{
										metaInfoFile.Write(crc32);
										metaInfoFile.CloseFile();
									}
								}
							}
							else
							{
								//check for .meta file
								//create the meta-info file
								gxFile metaInfoFile;
								char metaInfoFileName[512];
								sprintf(metaInfoFileName, "%s.meta", buffer);
								if(!metaInfoFile.OpenFile(metaInfoFileName))
								{
									if(metaInfoFile.OpenFile(metaInfoFileName, gxFile::FILE_w))
									{
										metaInfoFile.Write(crc32);
										metaInfoFile.CloseFile();
									}
								}
								else
								{
									metaInfoFile.CloseFile();
								}
							}
						}

						m_nAssetsToProcess++;
						SendDlgItemMessage(m_hWndProgress, m_iProgressBarID, PBM_SETPOS, m_nAssetsToProcess, 0);
					}
				}
                break;

            case DT_DIR:
                /* Scan sub-directory recursively */
                if (strcmp (ent->d_name, ".") != 0  &&  strcmp (ent->d_name, "..") != 0)
				{
                    traverseAssetDirectory(buffer);
                }
                break;

            default:
                /* Do not device entries */
                /*NOP*/;
            }

        }

        closedir (dir);
        ok = 1;

    } else {
        /* Could not open directory */
        printf ("Cannot open directory %s\n", dirname);
        ok = 0;
    }

    return ok;
}

int AssetImporter::import_fbx_to_metadata(const char* fbx_file_name, const char* crcFileName, struct stat srcStat)
{
	std::vector<gxMaterial*> materialList;
	std::vector<gxAnimationSet*> animationSetList;

	fbxImporter importer;
	object3d* obj3d=importer.loadMyFBX(fbx_file_name, &materialList, &animationSetList);

	for(std::vector<gxMaterial*>::iterator it = materialList.begin(); it != materialList.end(); ++it)
	{
		gxMaterial* material = *it;
		import_material_to_metadata(fbx_file_name, material);
	}

	//
	gxFile file_meta;
	if(file_meta.OpenFile(crcFileName, gxFile::FILE_w))
	{
		stMetaHeader metaHeader;
		metaHeader.filetype=eMeta3DFile;
		metaHeader.lastaccessed = srcStat.st_atime;
		metaHeader.lastmodified = srcStat.st_mtime;
		metaHeader.lastchanged = srcStat.st_ctime;

		//write the meta header
		file_meta.WriteBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));
		obj3d->write(file_meta);
		//write the texture header
		file_meta.CloseFile();
	}
	//

	materialList.clear();
	animationSetList.clear();

	GX_DELETE(obj3d);

	return 1;
}

int AssetImporter::import_material_to_metadata(const char* fbx_file_name, gxMaterial* material)
{
	bool bCreateMetaFile=false;
	struct stat fst;
	char buffer[1024];
	char temp_buffer[1024];
	GX_STRCPY(temp_buffer, gxUtil::getFolderPathFromFileName(fbx_file_name));
	sprintf(buffer, "%s/%s.mat", temp_buffer, material->getMaterialName());
	int crc32=Crc32::Calc((unsigned char*)buffer);
	gxFile materialFile;
	if(materialFile.OpenFile(buffer))
	{
		materialFile.CloseFile();
	}
	else
	{
		materialFile.OpenFile(buffer, gxFile::FILE_w);
		materialFile.Write(crc32);
		materialFile.CloseFile();
	}

	memset(&fst, 0, sizeof(fst));
	if(stat(buffer, &fst)==0) 
	{
		char crcFileName[512];
		sprintf(crcFileName, "%s/%s/%x", EditorApp::getProjectHomeDirectory(), "MetaData", crc32);

		stMetaHeader metaHeader;
		memset(&metaHeader, 0, sizeof(metaHeader));
		gxFile metaFile;
		if(metaFile.OpenFile(crcFileName))
		{
			metaFile.ReadBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));
			metaFile.CloseFile();

			if(metaHeader.lastmodified!=fst.st_mtime || (metaHeader.filetype!=eMetaMaterial))
			{
				//source file has some modifications so update meta file
				bCreateMetaFile=true;
			}
		}
		else
		{
			//there is no meta file associated with this source file. so create a new meta file
			bCreateMetaFile=true;
		}

		if(bCreateMetaFile)
		{
			gxFile file_meta;
			if(file_meta.OpenFile(crcFileName, gxFile::FILE_w))
			{
				stMetaHeader metaHeader;
				metaHeader.filetype=eMetaMaterial;
				metaHeader.lastaccessed = fst.st_atime;
				metaHeader.lastmodified = fst.st_mtime;
				metaHeader.lastchanged = fst.st_ctime;

				//write the meta header
				file_meta.WriteBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));

				//write the material header
				material->setFileCRC(crc32);
				material->write(file_meta);
				file_meta.CloseFile();
			}
		}
	}

	return 1;
}

int AssetImporter::import_tga_to_metadata(const char* tga_file_name, const char* crcFileName, struct stat srcStat)
{
	tgaTexture texture;
	memset(&texture, 0, sizeof(texture));
	if(LoadTGA(&texture, tga_file_name))
	{
		gxFile file_meta;
		if(!file_meta.OpenFile(crcFileName, gxFile::FILE_w))
		{
			GE_DELETE_ARY(texture.imageData);
			return 0;
		}

		stMetaHeader metaHeader;
		metaHeader.filetype=eMetaTexture2D;
		metaHeader.lastaccessed = srcStat.st_atime;
		metaHeader.lastmodified = srcStat.st_mtime;
		metaHeader.lastchanged = srcStat.st_ctime;

		//write the meta header
		file_meta.WriteBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));

		//write the texture header
		file_meta.Write(texture.width);
		file_meta.Write(texture.height);
		int bpp=3;
		if(texture.type==1)
		{
			file_meta.Write(eTexture2D_888);
			bpp=3;
		}
		else if(texture.type==2)
		{
			file_meta.Write(eTexture2D_8888);
			bpp=4;
		}
		else
			file_meta.Write(eTexture2D_Unknown);
		file_meta.WriteBuffer(texture.imageData, texture.width*texture.height*bpp);

		file_meta.CloseFile();

		GE_DELETE_ARY(texture.imageData);
		return 1;
	}

	return 0;
}

int AssetImporter::import_png_to_metadata(const char* png_file_name, const char* crcFileName, struct stat srcStat)
{
	int width, height;
	png_byte color_type;
	png_byte color_bpp;
	png_byte bit_depth;
    png_byte header[8];    // 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    FILE *fp = fopen(png_file_name, "rb");
    if (!fp)
    {
		//DEBUG_PRINT("[read_png_file] File %s could not be opened for reading", file_name);
		return 0;
	}
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8))
    {
		fclose(fp);
		//DEBUG_PRINT("[read_png_file] File %s is not recognized as a PNG file", file_name);
		return 0;
	}


    /* initialize stuff */
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
    {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		//DEBUG_PRINT("[read_png_file] png_create_read_struct failed");
		return 0;
	}

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		//DEBUG_PRINT("[read_png_file] png_create_info_struct failed");
		return 0;
	}

	//create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		fclose(fp);
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		//DEBUG_PRINT("Unable to create png end info : %s");
		return 0;
	}

    if (setjmp(png_jmpbuf(png_ptr)))
    {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		//DEBUG_PRINT("[read_png_file] Error during init_io");
		return 0;
	}

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

	//png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    png_read_info(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    //number_of_passes = png_set_interlace_handling(png_ptr);
    //png_read_update_info(png_ptr, info_ptr);

	/* extract multiple pixels with bit depths of 1, 2, and 4 from a single
	* byte into separate bytes (useful for paletted and grayscale images).
	*/
	png_set_packing(png_ptr);


	/* expand paletted colors into true RGB triplets */
	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_expand(png_ptr);

	/* expand grayscale images to the full 8 bits from 1, 2, or 4 bits/pixel */
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand(png_ptr);

	/* expand paletted or RGB images with transparency to full alpha channels
	* so the data will be available as RGBA quartets */
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_expand(png_ptr);


    png_read_update_info(png_ptr, info_ptr);

    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    ///* read file */
    if (setjmp(png_jmpbuf(png_ptr)))
    {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		//DEBUG_PRINT("[read_png_file] Error during read_image");
		return 0;
	}

	int out_format=0;

	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	// Allocate the image_data as a big block, to be given to opengl
	png_byte *image_data = new png_byte[rowbytes * height];
	if (!image_data)
	{
		fclose(fp);
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		//LOGE("Unable to allocate image_data while loading %s ", filename);
		//DEBUG_PRINT("Unable to allocate image_data while loading");
		return 0;
	}

    png_bytep* row_pointers = new  png_bytep[height];
	if  (!row_pointers)
	{
		fclose(fp);
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		delete[] image_data;
		//DEBUG_PRINT("Unable to allocate row_pointer while loading");
		return 0;
	}
	// set the individual row_pointers to point at the correct offsets of image_data
	for (int i = 0; i < height; ++i)
		row_pointers[i/*height - 1 - i*/] = image_data + i * rowbytes;

    png_read_image(png_ptr, row_pointers);

	//
	gxFile file_meta;
	if(!file_meta.OpenFile(crcFileName, gxFile::FILE_w))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		delete[] image_data;
		free(row_pointers);
		fclose(fp);
		return 0;
	}

	stMetaHeader metaHeader;
	metaHeader.filetype=eMetaTexture2D;
	metaHeader.lastaccessed = srcStat.st_atime;
	metaHeader.lastmodified = srcStat.st_mtime;
	metaHeader.lastchanged = srcStat.st_ctime;

	//write the meta header
	file_meta.WriteBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));

	//write the texture header
	file_meta.Write(width);
	file_meta.Write(height);
	int bpp=3;
	if(color_type==PNG_COLOR_TYPE_RGB)
	{
		file_meta.Write(eTexture2D_888);
		bpp=3;
	}
	else if(color_type==PNG_COLOR_TYPE_RGB_ALPHA)
	{
		file_meta.Write(eTexture2D_8888);
		bpp=4;
	}
	else
		file_meta.Write(eTexture2D_Unknown);
	file_meta.WriteBuffer(image_data, width*height*bpp);

	file_meta.CloseFile();
	//

	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	delete[] image_data;
	free(row_pointers);
    fclose(fp);

	////DEBUG_PRINT("%s : size(%d, %d), format %d, compressed size %d bytes\n", out_filename, width, height, out_format, compressedSize);
	return 1;
}