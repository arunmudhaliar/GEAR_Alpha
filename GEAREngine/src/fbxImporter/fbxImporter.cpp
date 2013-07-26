#include "fbxImporter.h"
#include "../util/gxUtil.h"

#include <assert.h>

//extern "C" {
//
//static fbxImporter g_cFBXImporter;
//
//extern DllExport void myDLL_fbxImport(const char* filename)
//{
//	g_cFBXImporter.loadFBX(filename);
//}
//
//}

fbxImporter::fbxImporter()
{
}

fbxImporter::~fbxImporter()
{
}

object3d* fbxImporter::loadFBX(const char* filename)
{
	numTabs=0;

	// Change the following filename to a suitable filename value.
	const char* lFilename = filename;

	// Initialize the sdk manager. This object handles all our memory management.
	FbxManager* lSdkManager = FbxManager::Create();
  

	// Create the io settings object.
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	// Create an importer using our sdk manager.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager,"");
    
	// Use the first argument as the filename for the importer.
	if(!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) {
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetLastErrorString());
		return NULL;
	}
    
	// Create a new scene so it can be populated by the imported file.
	FbxScene* lScene = FbxScene::Create(lSdkManager,"myScene");

	// Import the contents of the file into the scene.
	lImporter->Import(lScene);

	// The file has been imported; we can get rid of the importer.
	lImporter->Destroy();
    
	// Print the nodes of the scene and their attributes recursively.
	// Note that we are not printing the root node, because it should
	// not contain any attributes.
	FbxNode* lRootNode = lScene->GetRootNode();
	object3d* object3d_root_object = NULL;
	if(lRootNode)
	{
		object3d_root_object = new object3d(0);
		object3d_root_object->setName(lRootNode->GetName());

		for(int i = 0; i < lRootNode->GetChildCount(); i++)
			PrintNode(lRootNode->GetChild(i), object3d_root_object, lSdkManager);
	}
	// Destroy the sdk manager and all other objects it was handling.
	lSdkManager->Destroy();

	return object3d_root_object;
}

FbxMatrix fbxImporter::getFBXGeometryTransform(FbxNode &fbxNode)
{

	FbxVector4 t = fbxNode.GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 r = fbxNode.GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 s = fbxNode.GetGeometricScaling(FbxNode::eSourcePivot);
	FbxMatrix fbxGeometry(t, r, s);	
	return fbxGeometry;
}

object3d* fbxImporter::loadMyFBX(const char *filePath, std::vector<gxMaterial*>* materialList)
{
	object3d* return_object3d=NULL;

	// make sure we use high precision... else FBX has a bad day.
    //fpucontrol fpu(FPUCONTROL_CHOP, FPUCONTROL_64BITS);
    
    FbxManager *fbxManager = FbxManager::Create();
    assert(fbxManager);
    
    FbxScene *fbxScene = fbxManager ? FbxScene::Create(fbxManager,"fbxScene") : 0;
    assert(fbxScene);
    
    FbxImporter *fbxImporter = fbxManager ? FbxImporter::Create(fbxManager, "") : 0;
    assert(fbxImporter);
    
    if(fbxManager && fbxScene && fbxImporter)
    {
		int fbxFileFormat = 0;
		if(!fbxManager->GetIOPluginRegistry()->DetectReaderFileFormat(filePath, fbxFileFormat))
		{
			// Unrecognizable file format. Try to fall back to native format.
			fbxFileFormat = fbxManager->GetIOPluginRegistry()->GetNativeReaderFormat();
		}
		//fbxImporter->SetFileFormat(fbxFileFormat);
      
#if defined(_WIN32)
		// normalize the file path because FBX can't handle relative paths.
		char normalizedFilePath[1024]={0};
		GetFullPathNameA(filePath, 1024, normalizedFilePath, 0);
		if(*normalizedFilePath)
		{
			filePath = normalizedFilePath;
		}
#endif
      
		bool importOk = fbxImporter->Initialize(filePath);
		assert(importOk);
		if(importOk)
		{
			importOk = fbxImporter->Import(fbxScene);
			assert(importOk);
		}
		if(importOk)
		{
			return_object3d=importFBXScene(*fbxManager, *fbxScene, materialList);
		}
    }
    
    if(fbxImporter)      fbxImporter->Destroy();
    if(fbxScene)         fbxScene->Destroy();
    if(fbxManager)       fbxManager->Destroy();

	return return_object3d;
}

object3d* fbxImporter::importFBXScene(FbxManager &fbxManager, FbxScene &fbxScene, std::vector<gxMaterial*>* materialList)
{
	FbxNode *fbxRoot = fbxScene.GetRootNode();
	assert(fbxRoot);
	if(fbxRoot)
	{
		// convert coordinate system...
		FbxAxisSystem fbxSceneAxisSystem = fbxScene.GetGlobalSettings().GetAxisSystem();
		FbxAxisSystem fbxOurAxisSystem(FbxAxisSystem::eZAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
		//FbxAxisSystem fbxOurAxisSystem(FbxAxisSystem::eZAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
		if(fbxSceneAxisSystem != fbxOurAxisSystem)
		{
			fbxOurAxisSystem.ConvertScene(&fbxScene);
		}
      
		//// convert units...
		//const float unitsPerMeter = 1.0f; // "The equivalent number of centimeters in the new system unit"
		//FbxSystemUnit fbxSceneSystemUnit = fbxScene.GetGlobalSettings().GetSystemUnit();
		//if(fbxSceneSystemUnit.GetScaleFactor() != unitsPerMeter)
		//{
		//	FbxSystemUnit fbxOurSystemUnit(unitsPerMeter);
		//	fbxOurSystemUnit.ConvertScene(&fbxScene);
		//}
      
		// convert everything into triangles...
		FbxGeometryConverter fbxConverter(&fbxManager);
		triangulateFBXRecursive(fbxConverter, *fbxRoot);
      
		// recurse over the scene nodes and import them as needed...
		object3d* object3d_root_object = new object3d(0);
		object3d_root_object->setName(fbxRoot->GetName());

		const unsigned int numChildren = fbxRoot->GetChildCount();
		for(unsigned int i=0; i<numChildren; i++)
		{
			FbxNode *fbxChildNode = fbxRoot->GetChild(i);
			assert(fbxChildNode);
			if(fbxChildNode)
			{
				importFBXNode(*fbxChildNode, object3d_root_object, materialList, fbxScene, object3d_root_object);
			}
		}
		//importFBXNode(*fbxRoot, object3d_root_object);


		//int numStacks = fbxScene.GetSrcObjectCount(FBX_TYPE(FbxAnimStack));
		//for(int x=0;x<numStacks;x++)
		//{
		//	FbxAnimStack* pAnimStack = FbxCast<FbxAnimStack>(fbxScene.GetSrcObject(FBX_TYPE(FbxAnimStack), x));
		//	int numAnimLayers = pAnimStack->GetMemberCount(FBX_TYPE(FbxAnimLayer));
		//	for(int y=0;y<numAnimLayers;y++)
		//	{
		//		FbxAnimLayer* lAnimLayer = pAnimStack->GetMember(FBX_TYPE(FbxAnimLayer), y);
		//	}
		//}

		return object3d_root_object;
	}

	return NULL;
}

static FbxMatrix FBXToXen(FbxMatrix &fbx)
{
	const double *matrix = (const double*)fbx;
	FbxMatrix t;
	t.SetRow(0, FbxVector4((float)matrix[0], (float)matrix[1], (float)matrix[2], (float)matrix[3]));
	t.SetRow(1, FbxVector4((float)matrix[4], (float)matrix[5], (float)matrix[6], (float)matrix[7]));
	t.SetRow(2, FbxVector4((float)matrix[8], (float)matrix[9], (float)matrix[10], (float)matrix[11]));
	t.SetRow(3, FbxVector4((float)matrix[12], (float)matrix[13], (float)matrix[14], (float)matrix[15]));

	//t.y.x = (float)matrix[4];  t.y.y = (float)matrix[5];  t.y.z = (float)matrix[6];
	//t.z.x = (float)matrix[8];  t.z.y = (float)matrix[9];  t.z.z = (float)matrix[10];
	//t.w.x = (float)matrix[12]; t.w.y = (float)matrix[13]; t.w.z = (float)matrix[14];
	return t;
}

FbxAMatrix CalculateGlobalTransform(FbxNode* pNode) 
{
    FbxAMatrix lTranlationM, lScalingM, lScalingPivotM, lScalingOffsetM, lRotationOffsetM, lRotationPivotM, \
                lPreRotationM, lRotationM, lPostRotationM, lTransform;

    FbxAMatrix lParentGX, lGlobalT, lGlobalRS;

	if(!pNode)
	{
		lTransform.SetIdentity();
		return lTransform;
	}

    // Construct translation matrix
    FbxVector4 lTranslation = pNode->LclTranslation.Get();
    lTranlationM.SetT(lTranslation);

    // Construct rotation matrices
    FbxVector4 lRotation = pNode->LclRotation.Get();
    FbxVector4 lPreRotation = pNode->PreRotation.Get();
    FbxVector4 lPostRotation = pNode->PostRotation.Get();
    lRotationM.SetR(lRotation);
    lPreRotationM.SetR(lPreRotation);
    lPostRotationM.SetR(lPostRotation);

    // Construct scaling matrix
    FbxVector4 lScaling = pNode->LclScaling.Get();
    lScalingM.SetS(lScaling);

    // Construct offset and pivot matrices
    FbxVector4 lScalingOffset = pNode->ScalingOffset.Get();
    FbxVector4 lScalingPivot = pNode->ScalingPivot.Get();
    FbxVector4 lRotationOffset = pNode->RotationOffset.Get();
    FbxVector4 lRotationPivot = pNode->RotationPivot.Get();
    lScalingOffsetM.SetT(lScalingOffset);
    lScalingPivotM.SetT(lScalingPivot);
    lRotationOffsetM.SetT(lRotationOffset);
    lRotationPivotM.SetT(lRotationPivot);

    // Calculate the global transform matrix of the parent node
    FbxNode* lParentNode = pNode->GetParent();
    if(lParentNode)
	{
        lParentGX = CalculateGlobalTransform(lParentNode);
	}
	else
	{
		lParentGX.SetIdentity();
	}

    //Construct Global Rotation
    FbxAMatrix lLRM, lParentGRM;
    FbxVector4 lParentGR = lParentGX.GetR();
    lParentGRM.SetR(lParentGR);
    lLRM = lPreRotationM * lRotationM * lPostRotationM;

    //Construct Global Shear*Scaling
    //FBX SDK does not support shear, to patch this, we use:
    //Shear*Scaling = RotationMatrix.Inverse * TranslationMatrix.Inverse * WholeTranformMatrix
    FbxAMatrix lLSM, lParentGSM, lParentGRSM, lParentTM;
    FbxVector4 lParentGT = lParentGX.GetT();
    lParentTM.SetT(lParentGT);
    lParentGRSM = lParentTM.Inverse() * lParentGX;
    lParentGSM = lParentGRM.Inverse() * lParentGRSM;
    lLSM = lScalingM;

    //Do not consider translation now
    FbxTransform::EInheritType lInheritType = pNode->InheritType.Get();
    if(lInheritType == FbxTransform::eInheritRrSs)
    {
        lGlobalRS = lParentGRM * lLRM * lParentGSM * lLSM;
    }
    else if(lInheritType == FbxTransform::eInheritRSrs)
    {
        lGlobalRS = lParentGRM * lParentGSM * lLRM * lLSM;
    }
    else if(lInheritType == FbxTransform::eInheritRrs)
    {
		FbxAMatrix lParentLSM;
		FbxVector4 lParentLS = pNode->LclScaling.Get();
		lParentLSM.SetS(lParentLS);

		FbxAMatrix lParentGSM_noLocal = lParentGSM * lParentLSM.Inverse();
        lGlobalRS = lParentGRM * lLRM * lParentGSM_noLocal * lLSM;
    }
    else
    {
        FBXSDK_printf("error, unknown inherit type! \n");
    }

    // Construct translation matrix
    // Calculate the local transform matrix
    lTransform = lTranlationM * lRotationOffsetM * lRotationPivotM * lPreRotationM * lRotationM * lPostRotationM * lRotationPivotM.Inverse()\
        * lScalingOffsetM * lScalingPivotM * lScalingM * lScalingPivotM.Inverse();
    FbxVector4 lLocalTWithAllPivotAndOffsetInfo = lTransform.GetT();
    // Calculate global translation vector according to: 
    // GlobalTranslation = ParentGlobalTransform * LocalTranslationWithPivotAndOffsetInfo
    FbxVector4 lGlobalTranslation = lParentGX.MultT(lLocalTWithAllPivotAndOffsetInfo);
    lGlobalT.SetT(lGlobalTranslation);

    //Construct the whole global transform
    lTransform = lGlobalT * lGlobalRS;

    return lTransform;
}

void fbxImporter::importFBXNode(FbxNode &fbxNode, object3d* parent_obj_node, std::vector<gxMaterial*>* materialList, FbxScene &fbxScene, object3d* rootObject3d)
{
	//kTime fbxTime;
	//KFbxXMatrix fbxGlobalPosition    = fbxNode.GetGlobalFromCurrentTake(fbxTime);
	//KFbxXMatrix fbxGeometryOffset    = getFBXGeometryTransform(fbxNode);
	//KFbxXMatrix fbxGlobalOffPosition = fbxGlobalPosition * fbxGeometryOffset;
	//mat3x4 transform = FBXToXen(fbxGlobalOffPosition);

	FbxMatrix transform;
	FbxMatrix fbxGlobalPosition = fbxNode.EvaluateLocalTransform();
	FbxMatrix fbxGeometryOffset = getFBXGeometryTransform(fbxNode);
	FbxMatrix fbxGlobalOffPosition = fbxGlobalPosition * fbxGeometryOffset;
    transform = FBXToXen(fbxGlobalOffPosition);

	FbxAMatrix localTM = fbxNode.EvaluateLocalTransform();
	
	FbxAMatrix globla_tm = fbxNode.EvaluateLocalTransform();
	globla_tm=fbxNode.EvaluateLocalTransform();

	object3d* temp_parent_obj=parent_obj_node;

	// import node...
	FbxMesh *fbxMesh = fbxNode.GetMesh();
	//FbxSkeleton* fbxSkeleton = fbxNode.GetSkeleton();

	if(fbxMesh)
	{
		gxMesh* newMesh = importFBXMesh(*fbxMesh, globla_tm, materialList);
		newMesh->setName(fbxNode.GetName());
		parent_obj_node->appendChild(newMesh);
		parent_obj_node=newMesh;
	}
	else
	{
		object3d* object3d_child = new object3d(1);
		object3d_child->setName(fbxNode.GetName());
		parent_obj_node->appendChild(object3d_child);
		parent_obj_node=object3d_child;
	}

	FbxVector4 row1 = globla_tm.GetRow(0);
	FbxVector4 row2 = globla_tm.GetRow(1);
	FbxVector4 row3 = globla_tm.GetRow(2);
	FbxVector4 row4 = globla_tm.GetRow(3);
	parent_obj_node->setXAxis(vector3f(row1.mData[0], row1.mData[1], row1.mData[2]));
	parent_obj_node->setYAxis(vector3f(row2.mData[0], row2.mData[1], row2.mData[2]));
	parent_obj_node->setZAxis(vector3f(row3.mData[0], row3.mData[1], row3.mData[2]));
	parent_obj_node->setPosition(vector3f(row4.mData[0], row4.mData[1], row4.mData[2]));

	//*parent_obj_node->getWorldMatrix() = *parent_obj_node * *temp_parent_obj->getWorldMatrix();

	//links
	if(fbxMesh)
	{
		for(int mm=0;mm<fbxMesh->GetDeformerCount(FbxDeformer::eSkin);mm++)
		{
			FbxSkin* skin=(FbxSkin*)fbxMesh->GetDeformer(mm, FbxDeformer::eSkin);
			for(int cc=0;cc<skin->GetClusterCount();cc++)
			{
				FbxCluster* cluster = skin->GetCluster(cc);
				FbxNode* linkNode=cluster->GetLink();
				const char* linknodename=linkNode->GetName();
				linknodename=linknodename;
			}
		}
	}
	//

	//animation
	FbxGlobalSettings& lTimeSettings = fbxScene.GetGlobalSettings();
	FbxTime::EMode lTimeMode = lTimeSettings.GetTimeMode();
	FbxTimeSpan pTimeSpan;
	lTimeSettings.GetTimelineDefaultTimeSpan(pTimeSpan);
	FbxLongLong nGlobalFrame=pTimeSpan.GetStop().GetFrameCount();
	double lFrameRate = FbxTime::GetFrameRate(lTimeMode);
	double oneFrameTime_inSec = 1.0f/lFrameRate;
	/*
	Since an animation curve is a function, on a given animation curve, only one key per time is allowed.
	The keys are sorted in time order. They can be accessed by their index on the curve, from 0 to FbxAnimCurve::KeyGetCount-1.
	The time unit in FBX (FbxTime) is 1/46186158000 of one second.
	*/
	//FbxLongLong fc=FbxTime::GetFrame(true);

	int numStacks = fbxScene.GetSrcObjectCount(FBX_TYPE(FbxAnimStack));
	gxAnimation* animationController=NULL;
	gxAnimationTrack* animTrack=NULL;

	for(int x=0;x<numStacks;x++)
	{
		FbxAnimStack* pAnimStack = FbxCast<FbxAnimStack>(fbxScene.GetSrcObject(FBX_TYPE(FbxAnimStack), x));
		int numAnimLayers = pAnimStack->GetMemberCount(FBX_TYPE(FbxAnimLayer));
		for(int y=0;y<numAnimLayers;y++)
		{
			FbxAnimLayer* lAnimLayer = pAnimStack->GetMember(FBX_TYPE(FbxAnimLayer), y);
			FbxAnimCurve* lAnimCurve_translationX = fbxNode.LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			FbxAnimCurve* lAnimCurve_translationY = fbxNode.LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			FbxAnimCurve* lAnimCurve_translationZ = fbxNode.LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);

			FbxAnimCurve* lAnimCurve_rotationX = fbxNode.LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			FbxAnimCurve* lAnimCurve_rotationY = fbxNode.LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			FbxAnimCurve* lAnimCurve_rotationZ = fbxNode.LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);

			FbxAnimCurve* lAnimCurve_scalingX = fbxNode.LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			FbxAnimCurve* lAnimCurve_scalingY = fbxNode.LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			FbxAnimCurve* lAnimCurve_scalingZ = fbxNode.LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);

			if(lAnimCurve_translationX || lAnimCurve_translationY || lAnimCurve_translationZ ||
				lAnimCurve_rotationX || lAnimCurve_rotationY || lAnimCurve_rotationZ ||
				lAnimCurve_scalingX || lAnimCurve_scalingY || lAnimCurve_scalingZ)
			{
				animationController=rootObject3d->createAnimationController();	//wont create new if there is already an animatiion controller exists
				if(animTrack==NULL)
				{
					gxAnimationSet* animSet = new gxAnimationSet();
					animTrack = new gxAnimationTrack();
					animTrack->setFPS((int)lFrameRate);
					animTrack->setTotalFrames((int)nGlobalFrame);
					animTrack->allocateTrack();
					parent_obj_node->setAnimationTrack(animTrack);
					animSet->appendTrack(animTrack);
					animationController->appendAnimationSet(animSet);
					animationController->setActiveAnimationSet(0);
				}


				matrix4x4f* componentTrack=animTrack->getTrack();
				FbxTime tt(0);
				FbxLongLong ti=0;
				double frame_time=0;
				for(int m=0;m<nGlobalFrame;m++)
				{
					FbxAMatrix local_tm=fbxNode.EvaluateLocalTransform(tt);
					componentTrack[m].setPosition(local_tm.GetRow(3).mData[0], local_tm.GetRow(3).mData[1], local_tm.GetRow(3).mData[2]);
					componentTrack[m].setXAxis(vector3f(local_tm.GetRow(0).mData[0], local_tm.GetRow(0).mData[1], local_tm.GetRow(0).mData[2]));
					componentTrack[m].setYAxis(vector3f(local_tm.GetRow(1).mData[0], local_tm.GetRow(1).mData[1], local_tm.GetRow(1).mData[2]));
					componentTrack[m].setZAxis(vector3f(local_tm.GetRow(2).mData[0], local_tm.GetRow(2).mData[1], local_tm.GetRow(2).mData[2]));

					frame_time+=oneFrameTime_inSec;
					ti=(frame_time*46186158000);
					tt.Set(ti);
				}
			}
		}//for(int y=0;y<numAnimLayers;y++)
	}//for(int x=0;x<numStacks;x++)
	//

	// import child nodes...
	const unsigned int numChildren = fbxNode.GetChildCount();
	for(unsigned int i=0; i<numChildren; i++)
	{
		FbxNode *fbxChildNode = fbxNode.GetChild(i);
		assert(fbxChildNode);
		if(fbxChildNode)
		{
			importFBXNode(*fbxChildNode, parent_obj_node, materialList, fbxScene, rootObject3d);
		}
	}
}

gxMesh* fbxImporter::importFBXMesh(FbxMesh &fbxMesh, const FbxMatrix &transform, std::vector<gxMaterial*>* materialList)
{
	gxMesh* newMesh = new gxMesh();
	float* vertexBuffer = newMesh->allocateVertexBuffer(fbxMesh.GetPolygonCount());
	float* normalBuffer = newMesh->allocateNormalBuffer(fbxMesh.GetPolygonCount());
	gxTriInfo* triInfoArray = NULL;

	struct stTriData
	{
		~stTriData()
		{
			arrayList.clear();
		}

		std::vector<int> arrayList;
	};

	stTriData*  vTriList = NULL;

	int nMaterialCount=fbxMesh.GetNode()->GetMaterialCount();
	if(nMaterialCount)
	{
		triInfoArray = newMesh->allocateTriInfoArray(nMaterialCount);
		vTriList = new stTriData[nMaterialCount];
	}
	else
	{
		triInfoArray = newMesh->allocateTriInfoArray(1);
		vTriList = new stTriData[1];
	}

	FbxLayerElementArrayTemplate<int> *fbxMaterialIndices = 0;
    fbxMesh.GetMaterialIndices(&fbxMaterialIndices);

	const unsigned int fbxNumMaterialIndices = fbxMaterialIndices ? (unsigned int)fbxMaterialIndices->GetCount() : 0;

	fbxMesh.ComputeBBox();
	FbxDouble3 bbmin=fbxMesh.BBoxMin;
	FbxDouble3 bbmax=fbxMesh.BBoxMax;
	newMesh->getAABB().set(vector3f(bbmin.mData[0], bbmin.mData[1], bbmin.mData[2]), vector3f(bbmax.mData[0], bbmax.mData[1], bbmax.mData[2]));

	//UV sets
	//fbxMesh.GetTextureUVCount();
	//FbxArray<FbxLayerElement::EType> arrayUV=fbxMesh.GetAllChannelUV(0);
	//fbxMesh.GetTextureUVIndex(
	FbxStringList uvSetNames;
	fbxMesh.GetUVSetNames(uvSetNames);
	gxUV* uvChannels=newMesh->allocateUVChannels(uvSetNames.GetCount(), fbxMesh.GetPolygonCount());
	//char* uvsetname=uvSetNames.GetStringAt(0);
	//fbxMesh.GetPolygonVertexUV(
	//

	for(int x=0;x<fbxMesh.GetPolygonCount();x++)
	{
		int vertexIndices[]={
			x*3+0,
			x*3+1,
			x*3+2
		};


		int fbxMaterialIndex = 0;
		if(fbxMaterialIndices)
			fbxMaterialIndex = fbxMaterialIndices->GetAt(x<fbxNumMaterialIndices ? x : 0);

		if(nMaterialCount)
		{
			FbxSurfaceMaterial* surfaceMaterial=fbxMesh.GetNode()->GetMaterial(fbxMaterialIndex);
			gxMaterial* material=triInfoArray[fbxMaterialIndex].getMaterial();
			if(material==NULL)
			{
				//check if the material name already exists in our list or not
				for(std::vector<gxMaterial*>::iterator it = materialList->begin(); it != materialList->end(); ++it)
				{
					gxMaterial* material_in_list = *it;
					if(strcmp(material_in_list->getMaterialName(), surfaceMaterial->GetName())==0)
					{
						triInfoArray[fbxMaterialIndex].setMaterial(material_in_list);
						material=material_in_list;
						break;
					}
				}

				if(material==NULL)
				{
					//create a new material and add to list and assign to the tri list
					material = new gxMaterial();
					material->setMaterialName(surfaceMaterial->GetName());

					FbxSurfaceLambert* lambert=(FbxSurfaceLambert*)surfaceMaterial;
					FbxDouble3 diffuse=lambert->Diffuse.Get();
					FbxDouble3 ambient=lambert->Ambient.Get();
					material->setDiffuseClr(vector4f(diffuse.mData[0], diffuse.mData[1], diffuse.mData[2], 1.0f));
					material->setAmbientClr(vector4f(ambient.mData[0], ambient.mData[1], ambient.mData[2], 1.0f));
					material->setSpecularClr(vector4f(0.2f, 0.2f, 0.2f, 1.0f));

					FbxProperty diffuseProp = surfaceMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
					if(diffuseProp.IsValid())
					{
						int numTextures = diffuseProp.GetSrcObjectCount(FbxFileTexture::ClassId);
						for (int iTexture = 0; iTexture < numTextures; iTexture++)
						{
							FbxFileTexture* texture = FbxCast<FbxFileTexture>(
								diffuseProp.GetSrcObject(FbxTexture::ClassId, iTexture)); 

							if( !texture ) continue;

							material->setTextureName(gxUtil::getFileNameFromPath(texture->GetFileName()));
						}
					}

					triInfoArray[fbxMaterialIndex].setMaterial(material);
					materialList->push_back(material);
				}
			}
		}

		vTriList[fbxMaterialIndex].arrayList.push_back(vertexIndices[0]);
		vTriList[fbxMaterialIndex].arrayList.push_back(vertexIndices[1]);
		vTriList[fbxMaterialIndex].arrayList.push_back(vertexIndices[2]);
			
		for(int y=0;y<fbxMesh.GetPolygonSize(x);y++)
		{
			int vertexIndex=fbxMesh.GetPolygonVertex(x, y);
			FbxVector4 vertex=fbxMesh.GetControlPointAt(vertexIndex);
			//vertex=transform.MultNormalize(vertex);
			FbxVector4 normal;
			fbxMesh.GetPolygonVertexNormal(x, y, normal);
			//normal=transform.MultNormalize(normal);
			normal.Normalize();
			//vertex
			vertexBuffer[vertexIndices[y]*3+0]=(float)vertex.mData[0];
			vertexBuffer[vertexIndices[y]*3+1]=(float)vertex.mData[1];
			vertexBuffer[vertexIndices[y]*3+2]=(float)vertex.mData[2];

			normalBuffer[vertexIndices[y]*3+0]=(float)normal.mData[0];
			normalBuffer[vertexIndices[y]*3+1]=(float)normal.mData[1];
			normalBuffer[vertexIndices[y]*3+2]=(float)normal.mData[2];

			//uv sets
			for(int m=0;m<uvSetNames.GetCount();m++)
			{
				FbxVector2 uv;
				fbxMesh.GetPolygonVertexUV(x, y, uvSetNames.GetStringAt(m), uv);
				uvChannels[m].m_pszfGLTexCoordList[vertexIndices[y]*2+0]=(float)uv.mData[0];
				uvChannels[m].m_pszfGLTexCoordList[vertexIndices[y]*2+1]=1.0f-(float)uv.mData[1];
			}
		}
	}

	int nCount=(nMaterialCount>0)?nMaterialCount:1;

	if(nMaterialCount==0)
	{
		//create and assign new material
		triInfoArray[0].setMaterial(createNewMaterial());
	}

	for(int m=0;m<nCount;m++)
	{
		int* triIndexPtr=triInfoArray[m].allocateMemory(vTriList[m].arrayList.size());

		for(int n=0;n<vTriList[m].arrayList.size();n++)
		{
			triIndexPtr[n]=vTriList[m].arrayList[n];
		}
	}

	GX_DELETE_ARY(vTriList);

	return newMesh;
}

gxMaterial* fbxImporter::createNewMaterial()
{
	gxMaterial* material = new gxMaterial();
	material->setMaterialName("New Material");

	material->setDiffuseClr(vector4f(0.7f, 0.7f, 0.7f, 1.0f));
	material->setAmbientClr(vector4f(0.7f, 0.7f, 0.7f, 1.0f));
	material->setSpecularClr(vector4f(0.2f, 0.2f, 0.2f, 1.0f));

	return material;
}

void fbxImporter::PrintTabs()
{
	for(int i = 0; i < numTabs; i++)
		printf("\t");
}

void fbxImporter::triangulateFBXRecursive(FbxGeometryConverter &fbxConverter, FbxNode &fbxNode)
{
	FbxNodeAttribute *fbxNodeAttribute = fbxNode.GetNodeAttribute();
	if(fbxNodeAttribute)
	{
		FbxNodeAttribute::EType fbxNodeAttributeType = fbxNodeAttribute->GetAttributeType();
		switch(fbxNodeAttributeType)
		{
			case FbxNodeAttribute::eNurbsSurface:
			{
				fbxConverter.ConvertNurbsSurfaceToNurbsInPlace(&fbxNode);
				fbxConverter.TriangulateInPlace(&fbxNode);
			break;
			}
			case FbxNodeAttribute::eNurbs:
			case FbxNodeAttribute::ePatch:
			case FbxNodeAttribute::eMesh:
				fbxConverter.TriangulateInPlace(&fbxNode);
			break;
		}
	}
    
	const unsigned int numChildren = fbxNode.GetChildCount();
	for(unsigned int i=0; i<numChildren; i++)
	{
		FbxNode *fbxChildNode = fbxNode.GetChild(i);
		assert(fbxChildNode);
		if(fbxChildNode)
		{
			triangulateFBXRecursive(fbxConverter, *fbxChildNode);
		}
	}
}

void fbxImporter::PrintNode(FbxNode* pNode, object3d* parent_obj_node, FbxManager* fbxManager)
{
	object3d* object3d_child = NULL;

	if(pNode->GetMesh())
	{
		gxMesh* newMesh = new gxMesh();
		newMesh->setName(pNode->GetName());

		//
		FbxMesh* fbxmesh=pNode->GetMesh();
		FbxGeometryConverter converter( fbxManager);
		fbxmesh=converter.TriangulateMesh(fbxmesh);
		int nTriGrp=fbxmesh->GetElementPolygonGroupCount();
		int nVerts=fbxmesh->GetControlPointsCount();
		int nTris=fbxmesh->GetPolygonCount();
		int nM=fbxmesh->GetElementMaterialCount();
		//fbxmesh->Get

		for(int x=0;x<nTris;x++)
		{
			for(int y=0;y<fbxmesh->GetPolygonSize(x);y++)
			{
				int vertexIndex=fbxmesh->GetPolygonVertex(x, y);
				FbxVector4 vertex=fbxmesh->GetControlPointAt(vertexIndex);
				vertex.Length();
				//fbxmesh->GetMaterialIndices
			}
		}

		int nMC=pNode->GetMaterialCount();
		for(int x=0;x<nMC;x++)
		{
			const char* mname=pNode->GetMaterial(x)->GetName();
			x=x;
		}

		//fbxmesh->GetEle
		//
		FbxVector4* fbxVertexLst = fbxmesh->GetControlPoints();

		parent_obj_node->appendChild(newMesh);
		object3d_child=newMesh;
	}
	else
	{
		object3d_child = new object3d(1);
		object3d_child->setName(pNode->GetName());
		parent_obj_node->appendChild(object3d_child);
	}

	const char* nodeName = pNode->GetName();
	FbxDouble3 translation = pNode->LclTranslation.Get();
	FbxDouble3 rotation = pNode->LclRotation.Get();
	FbxDouble3 scaling = pNode->LclScaling.Get();

	// print the contents of the node.
	printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n", 
		nodeName, 
		translation[0], translation[1], translation[2],
		rotation[0], rotation[1], rotation[2],
		scaling[0], scaling[1], scaling[2]
		);
	numTabs++;

	// Print the node's attributes.
	for(int i = 0; i < pNode->GetNodeAttributeCount(); i++)
		PrintAttribute(pNode->GetNodeAttributeByIndex(i));

	// Recursively print the children nodes.
	for(int j = 0; j < pNode->GetChildCount(); j++)
		PrintNode(pNode->GetChild(j), object3d_child, fbxManager);

	numTabs--;
	PrintTabs();
	printf("</node>\n");
}

/**
* Print an attribute.
*/
void fbxImporter::PrintAttribute(FbxNodeAttribute* pAttribute)
{
	if(!pAttribute) return;
 
	FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
	FbxString attrName = pAttribute->GetName();
	PrintTabs();
	// Note: to retrieve the chararcter array of a FbxString, use its Buffer() method.
	printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}

/**
* Return a string-based representation based on the attribute type.
*/
FbxString fbxImporter::GetAttributeTypeName(FbxNodeAttribute::EType type)
{
	switch(type) {
		case FbxNodeAttribute::eUnknown: return "unidentified";
		case FbxNodeAttribute::eNull: return "null";
		case FbxNodeAttribute::eMarker: return "marker";
		case FbxNodeAttribute::eSkeleton: return "skeleton";
		case FbxNodeAttribute::eMesh: return "mesh";
		case FbxNodeAttribute::eNurbs: return "nurb";
		case FbxNodeAttribute::ePatch: return "patch";
		case FbxNodeAttribute::eCamera: return "camera";
		case FbxNodeAttribute::eCameraStereo:    return "stereo";
		case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
		case FbxNodeAttribute::eLight: return "light";
		case FbxNodeAttribute::eOpticalReference: return "optical reference";
		case FbxNodeAttribute::eOpticalMarker: return "marker";
		case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
		case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
		case FbxNodeAttribute::eBoundary: return "boundary";
		case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
		case FbxNodeAttribute::eShape: return "shape";
		case FbxNodeAttribute::eLODGroup: return "lodgroup";
		case FbxNodeAttribute::eSubDiv: return "subdiv";
		default: return "unknown";
	}
}