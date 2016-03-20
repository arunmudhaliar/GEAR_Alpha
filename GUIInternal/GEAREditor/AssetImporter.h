#pragma once

#include <sys/stat.h>
#include "../../GEAREngine/src/core/gxMaterial.h"
#include "../../GEAREngine/src/core/object3d.h"
#include "../../GEAREngine/src/core/gxMetaStructures.h"

#include "secondryViews/geAssetImportDlg.h"

class AssetImporter
{
public:
	AssetImporter();
	~AssetImporter();

    bool importAssets(const char* assetsfolder, geAssetImportDlg* assetImportDlg);
	static int import_material_to_metadata(const char* fbx_file_name, gxMaterial* material);

	//save to crc
	static bool saveMaterialToMetaData(const char* crcFileName, gxMaterial* material, struct stat& fst);
	static bool saveObject3DToMetaData(const char* crcFileName, object3d* obj3d, struct stat& fst);

	static bool readMetaHeader(int crc, stMetaHeader& metaHeader, struct stat& fst);
	static void readMetaHeader(stMetaHeader& metaHeader, gxFile& metaFile, struct stat& fst);
	static void writeMetaHeader(stMetaHeader& metaHeader, gxFile& metaFile);

	static int calcCRC32(unsigned char* data);
	static const char* relativePathFromProjectHomeDirectory_AssetFolder(const std::string& path);
private:
	int traverseAndCountAssetDirectory(const char *dirname);
	int traverseAssetDirectory(const char *dirname);
	int import_png_to_metadata(const char* png_file_name, const char* crcFileName, struct stat srcStat);
	int import_tga_to_metadata(const char* tga_file_name, const char* crcFileName, struct stat srcStat);
	int import_fbx_to_metadata(const char* fbx_file_name, const char* crcFileName, struct stat srcStat);
	bool import_using_freeImageLib(const char* filename, const char* crcFileName, struct stat srcStat);

	int noOfAssetsToProcess;
#if !defined(GEAR_APPLE)
	HWND hWndProgress;
    int progressBarID;
    int staticTextID;
#endif
    geAssetImportDlg* assetImportDlg;
};