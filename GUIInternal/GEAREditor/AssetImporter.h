#ifndef ASSETIMPORTER_H
#define ASSETIMPORTER_H

#include <sys/stat.h>

class AssetImporter
{
public:
	AssetImporter();
	~AssetImporter();

	bool importAssets(const char* assetsfolder);


private:
	int traverseAssetDirectory(const char *dirname);
	int import_png_to_metadata(const char* png_file_name, const char* crcFileName, struct stat srcStat);
	int import_tga_to_metadata(const char* tga_file_name, const char* crcFileName, struct stat srcStat);
};

#endif