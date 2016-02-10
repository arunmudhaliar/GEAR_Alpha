#include "AssetImporter.h"
#include "EditorApp.h"
#include "../../GEAREngine/src/util/gxFile.h"
#include "../../GEAREngine/src/lpng151/png.h"
#include "../../GEAREngine/src/tga/Tga.h"
#include "../../GEAREngine/src/util/gxCrc32.h"
#include "../../GEAREngine/src/fbxImporter/fbxImporter.h"
#include "../../GEAREngine/src/util/gxUtil.cpp"

#include <sys/types.h>
#include <assert.h>
#include<dirent.h>
#ifdef _WIN32
    #include <direct.h>
#endif
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "../../FreeImage/FreeImage.h"

AssetImporter::AssetImporter()
{
    assetImportDlg=NULL;
}

AssetImporter::~AssetImporter()
{
}

bool AssetImporter::importAssets(const char* assetsfolder, geAssetImportDlg* assetImportDlg)
{
    this->assetImportDlg=assetImportDlg;
    
	//if metaData folder doesn't exist create it
	char temp_buffer[FILENAME_MAX];
	sprintf(temp_buffer, "%s/MetaData", EditorGEARApp::getProjectHomeDirectory());
#ifdef _WIN32
	if(MKDIR(temp_buffer)==0)
#else
    if(MKDIR(temp_buffer, 0777)==0)
#endif
	{
		//created a new metaDirectory
	}

	noOfAssetsToProcess=0;
	traverseAndCountAssetDirectory(EditorGEARApp::getProjectHomeDirectory());

	noOfAssetsToProcess=0;
	traverseAssetDirectory(EditorGEARApp::getProjectHomeDirectory());

    assetImportDlg->setBuffer("Complete");

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
					bool bImage=false;
					bool bFBX=false;

					if(util::GE_IS_EXTENSION(buffer, ".png") || util::GE_IS_EXTENSION(buffer, ".PNG") ||
						util::GE_IS_EXTENSION(buffer, ".tga") || util::GE_IS_EXTENSION(buffer, ".TGA") ||
						util::GE_IS_EXTENSION(buffer, ".bmp") || util::GE_IS_EXTENSION(buffer, ".BMP") ||
						util::GE_IS_EXTENSION(buffer, ".ico") || util::GE_IS_EXTENSION(buffer, ".ICO") ||
						util::GE_IS_EXTENSION(buffer, ".jpeg") || util::GE_IS_EXTENSION(buffer, ".JPEG") ||
						util::GE_IS_EXTENSION(buffer, ".pcx") || util::GE_IS_EXTENSION(buffer, ".PCX") ||
						util::GE_IS_EXTENSION(buffer, ".tif") || util::GE_IS_EXTENSION(buffer, ".TIF") ||
						util::GE_IS_EXTENSION(buffer, ".psd") || util::GE_IS_EXTENSION(buffer, ".PSD") ||
						util::GE_IS_EXTENSION(buffer, ".gif") || util::GE_IS_EXTENSION(buffer, ".GIF") ||
						util::GE_IS_EXTENSION(buffer, ".hdr") || util::GE_IS_EXTENSION(buffer, ".HDR")
						)
					{
						bImage=true;
					}

					if(util::GE_IS_EXTENSION(buffer, ".fbx") || util::GE_IS_EXTENSION(buffer, ".FBX"))
					{
						bFBX=true;
					}

					if(bImage || bFBX)
					{
						noOfAssetsToProcess++;
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
					bool bImage=false;
//					bool bTGA=false;
					bool bFBX=false;

					if(util::GE_IS_EXTENSION(buffer, ".png") || util::GE_IS_EXTENSION(buffer, ".PNG") ||
						util::GE_IS_EXTENSION(buffer, ".tga") || util::GE_IS_EXTENSION(buffer, ".TGA") ||
						util::GE_IS_EXTENSION(buffer, ".bmp") || util::GE_IS_EXTENSION(buffer, ".BMP") ||
						util::GE_IS_EXTENSION(buffer, ".ico") || util::GE_IS_EXTENSION(buffer, ".ICO") ||
						util::GE_IS_EXTENSION(buffer, ".jpeg") || util::GE_IS_EXTENSION(buffer, ".JPEG") ||
						util::GE_IS_EXTENSION(buffer, ".pcx") || util::GE_IS_EXTENSION(buffer, ".PCX") ||
						util::GE_IS_EXTENSION(buffer, ".tif") || util::GE_IS_EXTENSION(buffer, ".TIF") ||
						util::GE_IS_EXTENSION(buffer, ".psd") || util::GE_IS_EXTENSION(buffer, ".PSD") ||
						util::GE_IS_EXTENSION(buffer, ".gif") || util::GE_IS_EXTENSION(buffer, ".GIF") ||
						util::GE_IS_EXTENSION(buffer, ".hdr") || util::GE_IS_EXTENSION(buffer, ".HDR")
						)
					{
						bImage=true;
					}

					if(util::GE_IS_EXTENSION(buffer, ".fbx") || util::GE_IS_EXTENSION(buffer, ".FBX"))
					{
						bFBX=true;
					}

					if(bImage || bFBX)
					{
						bool bCreateMetaFile=false;
						struct stat fst;
						memset(&fst, 0, sizeof(fst));
						if(stat(buffer, &fst)==0) 
						{
//							const char* relativepath=AssetImporter::relativePathFromProjectHomeDirectory_AssetFolder(buffer);
							int crc32=gxCrc32::Calc((unsigned char*)AssetImporter::relativePathFromProjectHomeDirectory_AssetFolder(buffer));
							char crcFileName[512];
							sprintf(crcFileName, "%s/%s/%x", EditorGEARApp::getProjectHomeDirectory(), "MetaData", crc32);

							stMetaHeader metaHeader;
							memset(&metaHeader, 0, sizeof(metaHeader));
							gxFile metaFile;
							if(metaFile.OpenFile(crcFileName))
							{
								metaFile.ReadBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));
								metaFile.CloseFile();

								if(metaHeader.lastmodified!=fst.st_mtime || 
									((bImage) && metaHeader.filetype!=eMetaTexture2D) ||
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

                            assetImportDlg->setBuffer(buffer);

							if(bCreateMetaFile)
							{
								bool bWriteMetaInfo=false;
#if __APPLE__   //some how freeimage fail to load png files
                                if(util::GE_IS_EXTENSION(buffer, ".png") || util::GE_IS_EXTENSION(buffer, ".PNG"))
                                {
                                    if(bImage && import_png_to_metadata(buffer, crcFileName, fst))
                                    {
                                        bWriteMetaInfo=true;
                                    }
                                }
                                else
                                {
                                    if(bImage && import_using_freeImageLib(buffer, crcFileName, fst))
                                    {
                                        bWriteMetaInfo=true;
                                    }
                                }
#else
								if(bImage && import_using_freeImageLib(buffer, crcFileName, fst))
								{
									bWriteMetaInfo=true;
								}
#endif
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

						noOfAssetsToProcess++;
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

void AssetImporter::readMetaHeader(stMetaHeader& metaHeader, gxFile& metaFile, struct stat& fst)
{
	memset(&metaHeader, 0, sizeof(metaHeader));
	metaFile.ReadBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));
	metaFile.CloseFile();

	fst.st_atime = metaHeader.lastaccessed;
	fst.st_mtime = metaHeader.lastmodified;
	fst.st_ctime = metaHeader.lastchanged;
}

bool AssetImporter::readMetaHeader(int crc, stMetaHeader& metaHeader, struct stat& fst)
{
	char crcFileName[512];
	sprintf(crcFileName, "%s/%s/%x", EditorGEARApp::getProjectHomeDirectory(), "MetaData", crc);

	memset(&metaHeader, 0, sizeof(metaHeader));
	gxFile metaFile;
	if(metaFile.OpenFile(crcFileName))
	{
		metaFile.ReadBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));
		metaFile.CloseFile();

		fst.st_atime = metaHeader.lastaccessed;
		fst.st_mtime = metaHeader.lastmodified;
		fst.st_ctime = metaHeader.lastchanged;

		return true;
	}

	return false;
}

void AssetImporter::writeMetaHeader(stMetaHeader& metaHeader, gxFile& metaFile)
{
	metaFile.WriteBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));
}

int AssetImporter::import_fbx_to_metadata(const char* fbx_file_name, const char* crcFileName, struct stat srcStat)
{
	std::vector<gxMaterial*> materialList;
	std::vector<gxAnimationSet*> animationSetList;
	fbxImporter importer;
    
	object3d* obj3d=importer.loadMyFBX(fbx_file_name, &materialList, &animationSetList, EditorGEARApp::getProjectHomeDirectory());

	for(std::vector<gxMaterial*>::iterator it = materialList.begin(); it != materialList.end(); ++it)
	{
		gxMaterial* material = *it;
		import_material_to_metadata(fbx_file_name, material);
	}

	saveObject3DToMetaData(crcFileName, obj3d, srcStat);

	materialList.clear();
	animationSetList.clear();

	GX_DELETE(obj3d);

	return 1;
}

bool AssetImporter::saveObject3DToMetaData(const char* crcFileName, object3d* obj3d, struct stat& fst)
{
	gxFile file_meta;
	if(file_meta.OpenFile(crcFileName, gxFile::FILE_w))
	{
		stMetaHeader metaHeader;
		metaHeader.filetype=eMeta3DFile;
		metaHeader.lastaccessed = fst.st_atime;
		metaHeader.lastmodified = fst.st_mtime;
		metaHeader.lastchanged = fst.st_ctime;

		//write the meta header
		file_meta.WriteBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));
		obj3d->write(file_meta);
		//write the texture header
		file_meta.CloseFile();
		return true;
	}

	return false;
}

int AssetImporter::import_material_to_metadata(const char* fbx_file_name, gxMaterial* material)
{
	bool bCreateMetaFile=false;
	struct stat fst;
	char buffer[FILENAME_MAX];
	char temp_buffer[FILENAME_MAX];
	GX_STRCPY(temp_buffer, gxUtil::getFolderPathFromFileName(fbx_file_name));
	sprintf(buffer, "%s/%s.mat", temp_buffer, material->getMaterialName());
	unsigned char* relative_path=(unsigned char*)AssetImporter::relativePathFromProjectHomeDirectory_AssetFolder(buffer);
	unsigned int crc32=gxCrc32::Calc(relative_path);
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
		sprintf(crcFileName, "%s/%s/%x", EditorGEARApp::getProjectHomeDirectory(), "MetaData", crc32);

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
			material->setAssetFileCRC(crc32, (char*)relative_path);
			saveMaterialToMetaData(crcFileName, material, fst);
		}
	}

	return 1;
}

bool AssetImporter::saveMaterialToMetaData(const char* crcFileName, gxMaterial* material, struct stat& fst)
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
		material->write(file_meta);
		file_meta.CloseFile();
		return true;
	}
	return false;
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
			file_meta.Write(eTexture2D_RGB888);
			bpp=3;
		}
		else if(texture.type==2)
		{
			file_meta.Write(eTexture2D_RGBA8888);
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
//	png_byte color_bpp;
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

//	int out_format=0;

	int rowbytes = (int)png_get_rowbytes(png_ptr, info_ptr);

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
		row_pointers[/*i*/height - 1 - i] = image_data + i * rowbytes;

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
		file_meta.Write(eTexture2D_RGB888);
		bpp=3;
	}
	else if(color_type==PNG_COLOR_TYPE_RGB_ALPHA)
	{
		file_meta.Write(eTexture2D_RGBA8888);
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

bool AssetImporter::import_using_freeImageLib(const char* filename, const char* crcFileName, struct stat srcStat)
{
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	
	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if(fif == FIF_UNKNOWN) 
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if(fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if(!dib)
		return false;

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if((bits == 0) || (width == 0) || (height == 0))
		return false;
	
	////if this texture ID is in use, unload the current texture
	//if(m_texID.find(texID) != m_texID.end())
	//	glDeleteTextures(1, &(m_texID[texID]));

	////generate an OpenGL texture ID for this texture
	//glGenTextures(1, &gl_texID);
	////store the texture ID mapping
	//m_texID[texID] = gl_texID;
	////bind to the new texture ID
	//glBindTexture(GL_TEXTURE_2D, gl_texID);
	////store the texture data for OpenGL use
	//glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,
	//	border, image_format, GL_UNSIGNED_BYTE, bits);

	//
	gxFile file_meta;
	if(!file_meta.OpenFile(crcFileName, gxFile::FILE_w))
	{
		//Free FreeImage's copy of the data
		FreeImage_Unload(dib);
		return false;
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
	int bpp=FreeImage_GetBPP(dib);
	
	switch(bpp)
	{
	case 24:
		{
			int bytesPerPixel=(bpp/8);
			for(int x=0;x<width*height;x++)
			{
				BYTE b = bits[x*bytesPerPixel+0];
				bits[x*bytesPerPixel+0] = bits[x*bytesPerPixel+2];
				bits[x*bytesPerPixel+2]=b;
			}
			file_meta.Write(eTexture2D_RGB888);
		}
		break;
	case 32:
		{
			int bytesPerPixel=(bpp/8);
			for(int x=0;x<width*height;x++)
			{
				BYTE b = bits[x*bytesPerPixel+0];
				bits[x*bytesPerPixel+0] = bits[x*bytesPerPixel+2];
				bits[x*bytesPerPixel+2]=b;
			}
			file_meta.Write(eTexture2D_RGBA8888);
		}
		break;
	default:
		file_meta.Write(eTexture2D_Unknown);
	}
	file_meta.WriteBuffer(bits, width*height*(bpp/8));

	file_meta.CloseFile();
	//

	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);

	return true;
}

int AssetImporter::calcCRC32(unsigned char* data)
{
	return gxCrc32::Calc(data);
}

const char* AssetImporter::relativePathFromProjectHomeDirectory_AssetFolder(const char* path)
{
	return &path[strlen(EditorGEARApp::getProjectHomeDirectory())+strlen("/Assets")];
}
