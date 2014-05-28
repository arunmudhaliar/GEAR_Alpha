#ifndef ASSETUSERDATA_H
#define ASSETUSERDATA_H

class assetUserData
{
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
			STRCPY(assetAbsolutePath, path);
		assetObjectPtr=ptr;
	}
	
	~assetUserData()
	{
		assetObjectPtr=NULL;
	}

	const char* getAssetAbsolutePath()	{	return assetAbsolutePath;	}
	ASSETUSERDATA_TYPE getAssetType()	{	return type;				}
	void* getAssetObjectPtr()			{	return assetObjectPtr;		}
	void setAssetObjectPtr(void* ptr, ASSETUSERDATA_TYPE typeID)	{	assetObjectPtr= ptr; type=typeID;	}

private:
	char assetAbsolutePath[1024];
	ASSETUSERDATA_TYPE type;
	void* assetObjectPtr;
};

#endif