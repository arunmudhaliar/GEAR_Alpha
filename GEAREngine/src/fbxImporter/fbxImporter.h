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

class fbxImporter
{
public:
	fbxImporter();

	~fbxImporter();

	object3d* loadFBX(const char* filename);


	/* Tab character ("\t") counter */
	int numTabs; 

	/**
	 * Print the required number of tabs.
	 */
	void PrintTabs();

	/**
	* Print a node, its attributes, and all its children recursively.
	*/
	void PrintNode(FbxNode* pNode, object3d* parent_obj_node, FbxManager* fbxManager);

	FbxMatrix getFBXGeometryTransform(FbxNode &fbxNode);

	object3d* loadMyFBX(const char *filePath, std::vector<gxMaterial*>* materialList);
	object3d* importFBXScene(FbxManager &fbxManager, FbxScene &fbxScene, std::vector<gxMaterial*>* materialList);
	void importFBXNode(FbxNode &fbxNode, object3d* parent_obj_node, std::vector<gxMaterial*>* materialList, FbxScene &fbxScene);
	gxMesh* importFBXMesh(FbxMesh &fbxMesh, const FbxMatrix &transform, std::vector<gxMaterial*>* materialList);

	void triangulateFBXRecursive(FbxGeometryConverter &fbxConverter, FbxNode &fbxNode);
	void PrintAttribute(FbxNodeAttribute* pAttribute);

	/**
	* Return a string-based representation based on the attribute type.
	*/
	FbxString GetAttributeTypeName(FbxNodeAttribute::EType type);

private:
	gxMaterial* createNewMaterial();
};

#endif