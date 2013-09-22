#ifndef ASSETIMPORTER_H
#define ASSETIMPORTER_H

#include <sys/stat.h>
#include "../../GEAREngine/src/core/gxMaterial.h"

class AssetImporter
{
public:
	AssetImporter();
	~AssetImporter();

	bool importAssets(const char* assetsfolder, HWND hWndDlg, int progressBarID, int statictextID);


	static int import_material_to_metadata(const char* fbx_file_name, gxMaterial* material);

private:
	int traverseAndCountAssetDirectory(const char *dirname);
	int traverseAssetDirectory(const char *dirname);
	int import_png_to_metadata(const char* png_file_name, const char* crcFileName, struct stat srcStat);
	int import_tga_to_metadata(const char* tga_file_name, const char* crcFileName, struct stat srcStat);
	int import_fbx_to_metadata(const char* fbx_file_name, const char* crcFileName, struct stat srcStat);

	int m_nAssetsToProcess;
	int m_iProgressBarID;
	int m_iStatictextID;
	HWND m_hWndProgress;
};

#endif