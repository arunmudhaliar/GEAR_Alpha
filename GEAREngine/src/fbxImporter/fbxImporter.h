#ifndef FBXIMPORTER_H
#define FBXIMPORTER_H

#define FBXSDK_NEW_API
#define FBXSDK_SHARED

#include <fbxsdk.h>
#include "../core/object3d.h"
#include "../core/gxMesh.h"
#include "../core/gxSkinnedMesh.h"

class DECLSPEC fbxImporter
{
public:
	fbxImporter();
	~fbxImporter();

	struct stBoneInfluence
	{
		std::vector<FbxNode*> bone;
		std::vector<FbxCluster*> bone_cluster;
		std::vector<float>	weight;
	};

	struct stBoneList
	{
		std::vector<FbxNode*> bonelst;
		std::vector<int> boneIndex;
	};

	object3d* loadMyFBX(const char *filePath, std::vector<gxMaterial*>* materialList, std::vector<gxAnimationSet*>* animationSetList, const char* projecthomedirectory);

private:

	FbxMatrix getFBXGeometryTransform(FbxNode &fbxNode);
	object3d* importFBXScene(const char* filePath, FbxManager &fbxManager, FbxScene &fbxScene, std::vector<gxMaterial*>* materialList, std::vector<gxAnimationSet*>* animationSetList, int fileCRC);
	void importFBXNode(FbxNode &fbxNode, object3d* parent_obj_node, std::vector<gxMaterial*>* materialList, FbxScene &fbxScene, object3d* rootObject3d, std::vector<gxAnimationSet*>* animationSetList, stBoneList* boneList);
	gxMesh* importFBXMesh(gxMesh* newMesh, FbxNode &fbxNode, const FbxMatrix &geometryOffset, std::vector<gxMaterial*>* materialList, object3d* rootObject3d, stBoneInfluence* boneInfluenceList, stBoneList* boneList);
	object3d* tryImportFBXSkinnedMesh(FbxNode &fbxNode, const FbxMatrix &geometryOffset, std::vector<gxMaterial*>* materialList, object3d* rootObject3d, stBoneList* boneList);
	bool tryImportAnimation(FbxNode &fbxNode, object3d* parent_obj_node, FbxScene &fbxScene, object3d* rootObject3d, std::vector<gxAnimationSet*>* animationSetList);
	int tryImportMaterial(int vertexID, int nMaterialCount, FbxLayerElementArrayTemplate<int> *fbxMaterialIndices, object3d* rootObject3d, gxTriInfo* triInfoArray, FbxMesh &fbxMesh, std::vector<int>& validMaterials, std::vector<gxMaterial*>* materialList);

	void createTangentBuffer(gxMesh* newMesh, FbxMesh &fbxMesh, const FbxMatrix &geometryOffset, char* uvname);
	FbxNode* findRoot(FbxNode *fbxNode);
	void pushAllNodes(stBoneList* boneList, FbxNode *fbxNode, int& index);
	void populateBonesToMeshNode(stBoneList* boneList, object3d* obj, object3d* rootNode);
	void triangulateFBXRecursive(FbxGeometryConverter &fbxConverter, FbxNode &fbxNode);

	char m_cszNormalizedFilePath[FILENAME_MAX];
	int m_iPrivateBoneIterator;
};

#endif