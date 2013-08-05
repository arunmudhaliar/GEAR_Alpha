#ifndef FBXIMPORTER_H
#define FBXIMPORTER_H

#define FBXSDK_NEW_API
#define FBXSDK_SHARED

#include <fbxsdk.h>
#include "../core/object3d.h"
#include "../core/gxMesh.h"

//extern "C" {
//DllExport void myDLL_fbxImport(const char* filename);
//}

class DllExport fbxImporter
{
public:
	fbxImporter();
	~fbxImporter();

	//object3d* loadFBX(const char* filename);


	//int numTabs; 
	//void PrintTabs();
	//void PrintNode(FbxNode* pNode, object3d* parent_obj_node, FbxManager* fbxManager);

	FbxMatrix getFBXGeometryTransform(FbxNode &fbxNode);

	object3d* loadMyFBX(const char *filePath, std::vector<gxMaterial*>* materialList, std::vector<gxAnimationSet*>* animationSetList);
	object3d* importFBXScene(FbxManager &fbxManager, FbxScene &fbxScene, std::vector<gxMaterial*>* materialList, std::vector<gxAnimationSet*>* animationSetList, int fileCRC);
	void importFBXNode(FbxNode &fbxNode, object3d* parent_obj_node, std::vector<gxMaterial*>* materialList, FbxScene &fbxScene, object3d* rootObject3d, std::vector<gxAnimationSet*>* animationSetList);
	gxMesh* importFBXMesh(FbxMesh &fbxMesh, const FbxMatrix &geometryOffset, std::vector<gxMaterial*>* materialList, object3d* rootObject3d);

	void triangulateFBXRecursive(FbxGeometryConverter &fbxConverter, FbxNode &fbxNode);
	//void PrintAttribute(FbxNodeAttribute* pAttribute);

	//FbxString GetAttributeTypeName(FbxNodeAttribute::EType type);

private:
	char m_cszNormalizedFilePath[1024];
};

#endif