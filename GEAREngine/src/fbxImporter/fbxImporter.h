#ifndef FBXIMPORTER_H
#define FBXIMPORTER_H

#define FBXSDK_NEW_API
#define FBXSDK_SHARED

#include <fbxsdk.h>
#include "../core/object3d.h"
#include "../core/gxMesh.h"
#include "../core/gxSkinnedMesh.h"

//extern "C" {
//DECLSPEC void myDLL_fbxImport(const char* filename);
//}

class DECLSPEC fbxImporter
{
public:
	fbxImporter();
	~fbxImporter();

	//object3d* loadFBX(const char* filename);

	struct stBoneInfluence
	{
		std::vector<FbxNode*> bone;
		std::vector<float>	weight;
	};

	struct stBoneList
	{
		std::vector<FbxNode*> bonelst;
		std::vector<int> boneIndex;
	};
	//int numTabs; 
	//void PrintTabs();
	//void PrintNode(FbxNode* pNode, object3d* parent_obj_node, FbxManager* fbxManager);

	FbxMatrix getFBXGeometryTransform(FbxNode &fbxNode);

	object3d* loadMyFBX(const char *filePath, std::vector<gxMaterial*>* materialList, std::vector<gxAnimationSet*>* animationSetList, const char* projecthomedirectory);
	object3d* importFBXScene(const char* filePath, FbxManager &fbxManager, FbxScene &fbxScene, std::vector<gxMaterial*>* materialList, std::vector<gxAnimationSet*>* animationSetList, int fileCRC);
	void importFBXNode(FbxNode &fbxNode, object3d* parent_obj_node, std::vector<gxMaterial*>* materialList, FbxScene &fbxScene, object3d* rootObject3d, std::vector<gxAnimationSet*>* animationSetList, stBoneList* boneList);
	gxMesh* importFBXMesh(gxMesh* newMesh, FbxMesh &fbxMesh, const FbxMatrix &geometryOffset, std::vector<gxMaterial*>* materialList, object3d* rootObject3d, stBoneInfluence* boneInfluenceList, stBoneList* boneList);

	void createTangentBuffer(gxMesh* newMesh, FbxMesh &fbxMesh, const FbxMatrix &geometryOffset, char* uvname);

	FbxNode* findRoot(FbxNode *fbxNode);
	void pushAllNodes(stBoneList* boneList, FbxNode *fbxNode, int& index);
	void populateBonesToMeshNode(stBoneList* boneList, object3d* obj, object3d* rootNode);

	void triangulateFBXRecursive(FbxGeometryConverter &fbxConverter, FbxNode &fbxNode);
	//void PrintAttribute(FbxNodeAttribute* pAttribute);

	//FbxString GetAttributeTypeName(FbxNodeAttribute::EType type);

private:
	char m_cszNormalizedFilePath[1024];
	int m_iPrivateBoneIterator;
};

#endif