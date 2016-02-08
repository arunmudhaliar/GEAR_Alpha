#pragma once

class assetUserData
{
private:
	assetUserData()
	{
		assetObjectPtr=NULL;
	}
public:

	enum ASSETUSERDATA_TYPE
	{
		ASSET_ONLY_PATH,
		ASSET_MESH_OBJECT,
		ASSET_TEXTURE
	};

	assetUserData(ASSETUSERDATA_TYPE typeID, const char* path, void* ptr)
	{
		type=typeID;
		if(path)
			STRCPY(assetPath, path);
		assetObjectPtr=ptr;
	}
	
	~assetUserData()
	{
		assetObjectPtr=NULL;
	}

	const char* getAssetPath()	{	return assetPath;	}
	ASSETUSERDATA_TYPE getAssetType()	{	return type;				}
	void* getAssetObjectPtr()			{	return assetObjectPtr;		}
	void setAssetObjectPtr(void* ptr, ASSETUSERDATA_TYPE typeID)	{	assetObjectPtr= ptr; type=typeID;	}

private:
	char assetPath[FILENAME_MAX];
	ASSETUSERDATA_TYPE type;
	void* assetObjectPtr;
};