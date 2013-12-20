#include "fbxImporter.h"
#include "../util/gxUtil.h"
#include "../util/Crc32.h"


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

FbxMatrix fbxImporter::getFBXGeometryTransform(FbxNode &fbxNode)
{

	FbxVector4 t = fbxNode.GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 r = fbxNode.GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 s = fbxNode.GetGeometricScaling(FbxNode::eSourcePivot);
	FbxMatrix fbxGeometry(t, r, s);	
	return fbxGeometry;
}

object3d* fbxImporter::loadMyFBX(const char *filePath, std::vector<gxMaterial*>* materialList, std::vector<gxAnimationSet*>* animationSetList)
{
	object3d* return_object3d=NULL;
	int crc=0;
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
      
		crc = Crc32::Calc((unsigned char*)filePath);
#if defined(_WIN32)
		memset(m_cszNormalizedFilePath, 0, sizeof(m_cszNormalizedFilePath));
		// normalize the file path because FBX can't handle relative paths.
		GetFullPathNameA(filePath, 1024, m_cszNormalizedFilePath, 0);
		if(*m_cszNormalizedFilePath)
		{
			filePath = m_cszNormalizedFilePath;
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
			return_object3d=importFBXScene(filePath, *fbxManager, *fbxScene, materialList, animationSetList, crc);
		}
    }
    
    if(fbxImporter)      fbxImporter->Destroy();
    if(fbxScene)         fbxScene->Destroy();
    if(fbxManager)       fbxManager->Destroy();

	return return_object3d;
}

object3d* fbxImporter::importFBXScene(const char* filePath, FbxManager &fbxManager, FbxScene &fbxScene, std::vector<gxMaterial*>* materialList, std::vector<gxAnimationSet*>* animationSetList, int fileCRC)
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
		object3d_root_object->setFileCRC(fileCRC);
		object3d_root_object->setName(gxUtil::getFileNameFromPath(filePath));

		stBoneList boneList;
		int boneindex=0;
		m_iPrivateBoneIterator=0;
		pushAllNodes(&boneList, fbxRoot, boneindex);
		const unsigned int numChildren = fbxRoot->GetChildCount();
		for(unsigned int i=0; i<numChildren; i++)
		{
			FbxNode *fbxChildNode = fbxRoot->GetChild(i);
			assert(fbxChildNode);
			if(fbxChildNode)
			{
				importFBXNode(*fbxChildNode, object3d_root_object, materialList, fbxScene, object3d_root_object, animationSetList, &boneList);
			}
		}

		populateBonesToMeshNode(&boneList, object3d_root_object, object3d_root_object);
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

FbxNode* fbxImporter::findRoot(FbxNode *fbxNode)
{
	FbxNode *node = fbxNode;
	const char* parentName	= node->GetName();
	
	while(strcmp(parentName,"RootNode")!=0) {
		node = node->GetParent();
		parentName	= node->GetName();
	}
	return node;
}

void fbxImporter::populateBonesToMeshNode(stBoneList* boneList, object3d* obj, object3d* rootNode)
{
	if(obj->getID()==OBJECT3D_SKINNED_MESH)
	{
		gxSkinnedMesh* skinMesh = (gxSkinnedMesh*)obj;
		int index=0;
		skinMesh->clearPrivateIterator();
		skinMesh->populateBoneList(rootNode, index);
	}

	std::vector<object3d*>* childlist=obj->getChildList();
	for(std::vector<object3d*>::iterator it = childlist->begin(); it != childlist->end(); ++it)
	{
		object3d* childobj = *it;
		populateBonesToMeshNode(boneList, childobj, rootNode);
	}
}

void fbxImporter::pushAllNodes(stBoneList* boneList, FbxNode *fbxNode, int& index)
{
	boneList->bonelst.push_back(fbxNode);
	boneList->boneIndex.push_back(m_iPrivateBoneIterator);
	m_iPrivateBoneIterator++;
	const unsigned int numChildren = fbxNode->GetChildCount();
	for(unsigned int i=0; i<numChildren; i++)
	{
		FbxNode *fbxChildNode = fbxNode->GetChild(i);
		pushAllNodes(boneList, fbxChildNode, index);
	}
}

void fbxImporter::importFBXNode(FbxNode &fbxNode, object3d* parent_obj_node, std::vector<gxMaterial*>* materialList, FbxScene &fbxScene, object3d* rootObject3d, std::vector<gxAnimationSet*>* animationSetList, stBoneList* boneList)
{
	FbxMatrix transform;
	FbxMatrix fbxLocalPosition = fbxNode.EvaluateLocalTransform();
	FbxMatrix fbxGeometryOffset = getFBXGeometryTransform(fbxNode);
	//FbxMatrix fbxLocalOffPosition = fbxLocalPosition * fbxGeometryOffset;

	FbxMatrix local_tm = fbxLocalPosition;

	object3d* temp_parent_obj=parent_obj_node;
	FbxMesh *fbxMesh = fbxNode.GetMesh();
	//FbxSkeleton* fbxSkeleton = fbxNode.GetSkeleton();

	if(fbxMesh)
	{
		//links
		int nCluster=0;
		for(int mm=0;mm<fbxMesh->GetDeformerCount(FbxDeformer::eSkin);mm++)
		{
			FbxSkin* skin=(FbxSkin*)fbxMesh->GetDeformer(mm, FbxDeformer::eSkin);
			nCluster+=skin->GetClusterCount();
		}
		//

		if(nCluster)
		{
			FbxSkin*		skinDeformer = (FbxSkin *)fbxMesh->GetDeformer(0, FbxDeformer::eSkin);
			FbxSkin::EType	skinningType = skinDeformer->GetSkinningType();

			if(skinningType == FbxSkin::eLinear || skinningType == FbxSkin::eRigid || skinningType == FbxSkin::eDualQuaternion)
			{
				int vc=fbxMesh->GetControlPointsCount();
				stBoneInfluence* boneInfluenceList = new stBoneInfluence[vc];

				FbxCluster* clusterR	= skinDeformer->GetCluster(0);
				FbxNode*	rootBone	= findRoot(clusterR->GetLink());

				int deformerCount = fbxMesh->GetDeformerCount(FbxDeformer::eSkin); // Count of skeletons used for skinning
				for ( int defInd = 0; defInd < deformerCount; defInd++ )
				{
					skinDeformer = (FbxSkin *)fbxMesh->GetDeformer(defInd, FbxDeformer::eSkin);
					int clusterCount = skinDeformer->GetClusterCount();							// Count of bones in skeleton
					for (int clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
					{
						FbxCluster* cluster = skinDeformer->GetCluster(clusterIndex);
						if (!cluster->GetLink())
							continue;

						FbxNode*	currentFbxLimb	= cluster->GetLink();
						const char*	currentLimbName = currentFbxLimb->GetName();

						int vertexIndexCount = cluster->GetControlPointIndicesCount();
						for (int k = 0; k < vertexIndexCount; ++k)
						{
							int index = cluster->GetControlPointIndices()[k];

							// Sometimes, the mesh can have less points than at the time of the skinning
							// because a smooth operator was active when skinning but has been deactivated during export.
							if (index >= fbxMesh->GetControlPointsCount())
								continue;

							double weight = cluster->GetControlPointWeights()[k];
							if (weight == 0.0)
							{
								continue;
							}

							boneInfluenceList[index].bone.push_back(currentFbxLimb);
							boneInfluenceList[index].weight.push_back((float)weight);
						}
///////////////////////////////////
					}
				}

				gxSkinnedMesh* newSkinnedMesh = new gxSkinnedMesh();
				importFBXMesh(newSkinnedMesh, *fbxMesh, fbxGeometryOffset, materialList, rootObject3d, boneInfluenceList, boneList);
				newSkinnedMesh->setName(fbxNode.GetName());
				parent_obj_node->appendChild(newSkinnedMesh);
				parent_obj_node=newSkinnedMesh;
				
				newSkinnedMesh->allocateBoneList(boneList->bonelst.size());
				delete [] boneInfluenceList;
			}
			else
			{
				//didnt find any skinned mesh, so create a dummy object
				object3d* object3d_child = new object3d(1);
				object3d_child->setName(fbxNode.GetName());
				parent_obj_node->appendChild(object3d_child);
				parent_obj_node=object3d_child;
			}
		}
		else
		{
			gxMesh* newMesh = new gxMesh();
			importFBXMesh(newMesh, *fbxMesh, fbxGeometryOffset, materialList, rootObject3d, NULL, NULL);
			newMesh->setName(fbxNode.GetName());
			parent_obj_node->appendChild(newMesh);
			parent_obj_node=newMesh;
		}
	}
	else
	{
		object3d* object3d_child = new object3d(1);
		object3d_child->setName(fbxNode.GetName());
		parent_obj_node->appendChild(object3d_child);
		parent_obj_node=object3d_child;
	}

	FbxVector4 row1 = local_tm.GetRow(0);
	FbxVector4 row2 = local_tm.GetRow(1);
	FbxVector4 row3 = local_tm.GetRow(2);
	FbxVector4 row4 = local_tm.GetRow(3);
	parent_obj_node->setXAxis(vector3f((float)row1.mData[0], (float)row1.mData[1], (float)row1.mData[2]));
	parent_obj_node->setYAxis(vector3f((float)row2.mData[0], (float)row2.mData[1], (float)row2.mData[2]));
	parent_obj_node->setZAxis(vector3f((float)row3.mData[0], (float)row3.mData[1], (float)row3.mData[2]));
	parent_obj_node->setPosition(vector3f((float)row4.mData[0], (float)row4.mData[1], (float)row4.mData[2]));

	//*parent_obj_node->getWorldMatrix() = *parent_obj_node * *temp_parent_obj->getWorldMatrix();

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
					int nAnimSet=animationController->getAnimationSetList()->size();
					gxAnimationSet* animSet=NULL;
					if(nAnimSet)
						animSet=animationController->getAnimationSetList()->at(nAnimSet-1);
					if(!animSet)
					{
						animSet = new gxAnimationSet(pAnimStack->GetName());
						animationController->appendAnimationSet(animSet);
						//animationController->setActiveAnimationSet(0);
						animationSetList->push_back(animSet);
					}

					animTrack = new gxAnimationTrack();
					animTrack->setName(fbxNode.GetName());
					animTrack->setFPS((int)lFrameRate);
					animTrack->setTotalFrames((int)nGlobalFrame);
					animTrack->allocateTrack();
					parent_obj_node->setAnimationTrack(animTrack);
					animSet->appendTrack(animTrack);
				}


				matrix4x4f* componentTrack=animTrack->getTrack();
				FbxTime tt(0);
				FbxLongLong ti=0;
				double frame_time=0;
				for(int m=0;m<nGlobalFrame;m++)
				{
					FbxAMatrix local_tm=fbxNode.EvaluateLocalTransform(tt);
					componentTrack[m].setPosition((float)local_tm.GetRow(3).mData[0], (float)local_tm.GetRow(3).mData[1], (float)local_tm.GetRow(3).mData[2]);
					componentTrack[m].setXAxis(vector3f((float)local_tm.GetRow(0).mData[0], (float)local_tm.GetRow(0).mData[1], (float)local_tm.GetRow(0).mData[2]));
					componentTrack[m].setYAxis(vector3f((float)local_tm.GetRow(1).mData[0], (float)local_tm.GetRow(1).mData[1], (float)local_tm.GetRow(1).mData[2]));
					componentTrack[m].setZAxis(vector3f((float)local_tm.GetRow(2).mData[0], (float)local_tm.GetRow(2).mData[1], (float)local_tm.GetRow(2).mData[2]));

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
			importFBXNode(*fbxChildNode, parent_obj_node, materialList, fbxScene, rootObject3d, animationSetList, boneList);
		}
	}
}

gxMesh* fbxImporter::importFBXMesh(gxMesh* newMesh, FbxMesh &fbxMesh, const FbxMatrix &geometryOffset, std::vector<gxMaterial*>* materialList, object3d* rootObject3d, stBoneInfluence* boneInfluenceList, stBoneList* boneList)
{
	float* vertexBuffer = newMesh->allocateVertexBuffer(fbxMesh.GetPolygonCount());
	float* normalBuffer = newMesh->allocateNormalBuffer(fbxMesh.GetPolygonCount());
	gxTriInfo* triInfoArray = NULL;

	//Max no of bones per vertex
	int nMaxBonePerVert=0;
	int* boneIndexBuffer=NULL;
	float* weightBuffer=NULL;

	if(boneInfluenceList)
	{
		int nData=0;
		for(int x=0;x<fbxMesh.GetControlPointsCount();x++)
		{
			nData+=boneInfluenceList[x].bone.size();
			if((int)boneInfluenceList[x].bone.size()>nMaxBonePerVert)
				nMaxBonePerVert=boneInfluenceList[x].bone.size();
		}
		nMaxBonePerVert=4;	//hack
		//allocation for influence bones and weights
		boneIndexBuffer=((gxSkinnedMesh*)newMesh)->allocateBoneIndexBuffer(fbxMesh.GetPolygonCount(), nMaxBonePerVert);
		weightBuffer=((gxSkinnedMesh*)newMesh)->allocateWeightBuffer(fbxMesh.GetPolygonCount(), nMaxBonePerVert);
	}
	//

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
	FbxDouble3 bbmin=geometryOffset.MultNormalize(FbxVector4(fbxMesh.BBoxMin));
	FbxDouble3 bbmax=geometryOffset.MultNormalize(FbxVector4(fbxMesh.BBoxMax));
	newMesh->getOOBB().set(vector3f((float)bbmin.mData[0], (float)bbmin.mData[1], (float)bbmin.mData[2]), vector3f((float)bbmax.mData[0], (float)bbmax.mData[1], (float)bbmax.mData[2]));

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
						material_in_list->appendDependency(rootObject3d->getFileCRC());
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
					material->setDiffuseClr(vector4f((float)diffuse.mData[0], (float)diffuse.mData[1], (float)diffuse.mData[2], 1.0f));
					material->setAmbientClr(vector4f((float)ambient.mData[0], (float)ambient.mData[1], (float)ambient.mData[2], 1.0f));
					material->setSpecularClr(vector4f(0.2f, 0.2f, 0.2f, 1.0f));

#if 0	//moved the maps to surface shader. so we don't need this anymore
					FbxProperty diffuseProp = surfaceMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
					if(diffuseProp.IsValid())
					{
						int numTextures = diffuseProp.GetSrcObjectCount(FbxFileTexture::ClassId);
						for (int iTexture = 0; iTexture < numTextures; iTexture++)
						{
							FbxFileTexture* texture = FbxCast<FbxFileTexture>(
								diffuseProp.GetSrcObject(FbxTexture::ClassId, iTexture)); 

							if( !texture ) continue;
							
							stTextureMap* texmap = new stTextureMap();
							texmap->texturename.assign(gxUtil::getFileNameFromPath(texture->GetFileName()));
							material->appendTextureMap(texmap);
						}
					}
					else
					{
						stTextureMap* texmap = new stTextureMap();
						texmap->texturename.assign("no-map");
						material->appendTextureMap(texmap);
					}
#endif
					material->setMainShaderName("Diffuse");
					triInfoArray[fbxMaterialIndex].setMaterial(material);
					material->appendDependency(rootObject3d->getFileCRC());
					materialList->push_back(material);
				}
			}
		}

		vTriList[fbxMaterialIndex].arrayList.push_back(vertexIndices[0]);
		vTriList[fbxMaterialIndex].arrayList.push_back(vertexIndices[1]);
		vTriList[fbxMaterialIndex].arrayList.push_back(vertexIndices[2]);
			
		////http://stackoverflow.com/questions/13566608/loading-skinning-information-from-fbx
		//FbxAMatrix globalTM=fbxMesh.GetNode()->EvaluateGlobalTransform(0);
		//FbxAMatrix globalTM_inverse=globalTM.Inverse();
		//FbxAMatrix localTM=fbxMesh.GetNode()->EvaluateLocalTransform(0);
		//FbxAMatrix pivot = localTM*globalTM_inverse;

		for(int y=0;y<fbxMesh.GetPolygonSize(x);y++)
		{
			int vertexIndex=fbxMesh.GetPolygonVertex(x, y);

			FbxVector4 vertex=fbxMesh.GetControlPointAt(vertexIndex);
			//FbxVector4 v = pivot.MultT(vertex);
			//vertex=globalTM.MultT(v);
			vertex=geometryOffset.MultNormalize(vertex);

			FbxVector4 normal;
			fbxMesh.GetPolygonVertexNormal(x, y, normal);
			normal=geometryOffset.MultNormalize(normal);
			//normal=transform.MultNormalize(normal);
			normal.Normalize();
			//vertex
			vertexBuffer[vertexIndices[y]*3+0]=(float)vertex.mData[0];
			vertexBuffer[vertexIndices[y]*3+1]=(float)vertex.mData[1];
			vertexBuffer[vertexIndices[y]*3+2]=(float)vertex.mData[2];

			normalBuffer[vertexIndices[y]*3+0]=(float)normal.mData[0];
			normalBuffer[vertexIndices[y]*3+1]=(float)normal.mData[1];
			normalBuffer[vertexIndices[y]*3+2]=(float)normal.mData[2];

			//bone influence
			if(boneInfluenceList)
			{
				int nReminder = nMaxBonePerVert-boneInfluenceList[vertexIndex].bone.size();

				for(int bb=0;bb<(int)boneInfluenceList[vertexIndex].bone.size();bb++)
				{
					FbxNode* influencedBone=boneInfluenceList[vertexIndex].bone[bb];
					float weight=boneInfluenceList[vertexIndex].weight[bb];

					int si=0;
					for(si=0;si<(int)boneList->bonelst.size();si++)
					{
						if(boneList->bonelst[si]==influencedBone)
							break;
					}
					boneIndexBuffer[vertexIndices[y]*nMaxBonePerVert+bb]=si;
					weightBuffer[vertexIndices[y]*nMaxBonePerVert+bb]=weight;
				}
				if(nReminder>0)
				{
					for(int bb=nMaxBonePerVert-nReminder;bb<nMaxBonePerVert;bb++)
					{
						boneIndexBuffer[vertexIndices[y]*nMaxBonePerVert+bb]=boneIndexBuffer[vertexIndices[y]*nMaxBonePerVert+bb];
					}
				}
			}

			//uv sets
			for(int m=0;m<uvSetNames.GetCount();m++)
			{
				FbxVector2 uv;
				bool pUnmapped;
				fbxMesh.GetPolygonVertexUV(x, y, uvSetNames.GetStringAt(m), uv, pUnmapped);
				uvChannels[m].m_pszfGLTexCoordList[vertexIndices[y]*2+0]=(float)uv.mData[0];
				uvChannels[m].m_pszfGLTexCoordList[vertexIndices[y]*2+1]=1.0f-(float)uv.mData[1];
			}
		}
	}

	//create the tangent buffer
	if(uvSetNames.GetCount())
	{
		createTangentBuffer(newMesh, fbxMesh, geometryOffset, uvSetNames.GetStringAt(0));
	}

	int nCount=(nMaterialCount>0)?nMaterialCount:1;

	if(nMaterialCount==0)
	{
		//create and assign new material
		triInfoArray[0].setMaterial(gxMaterial::createNewMaterial());
		triInfoArray[0].getMaterial()->appendDependency(rootObject3d->getFileCRC());
	}

	for(int m=0;m<nCount;m++)
	{
		if(vTriList[m].arrayList.size()==0)
			continue;

		int* triIndexPtr=triInfoArray[m].allocateMemory(vTriList[m].arrayList.size());
		for(int n=0;n<(int)vTriList[m].arrayList.size();n++)
		{
			triIndexPtr[n]=vTriList[m].arrayList[n];
		}
	}

	GX_DELETE_ARY(vTriList);

	return newMesh;
}

void fbxImporter::createTangentBuffer(gxMesh* newMesh, FbxMesh &fbxMesh, const FbxMatrix &geometryOffset, char* uvname)
{
	int vertexCount=fbxMesh.GetControlPointsCount();
	vector3f *tan1 = new vector3f[vertexCount * 2];
	FbxVector4 *normalbuffer = new FbxVector4[vertexCount];
	FbxVector4 *tangentbuffer = new FbxVector4[vertexCount];
	int *normalbufferCount = new int[vertexCount];
    vector3f *tan2 = tan1 + vertexCount;
    ZeroMemory(tan1, vertexCount * sizeof(vector3f) * 2);
    ZeroMemory(normalbuffer, vertexCount * sizeof(FbxVector4));
    ZeroMemory(normalbufferCount, vertexCount * sizeof(int));
    ZeroMemory(tangentbuffer, vertexCount * sizeof(FbxVector4));

	for (long x = 0; x < fbxMesh.GetPolygonCount(); x++)
    {
		int vertexIndex0=fbxMesh.GetPolygonVertex(x, 0);
		int vertexIndex1=fbxMesh.GetPolygonVertex(x, 1);
		int vertexIndex2=fbxMesh.GetPolygonVertex(x, 2);

		FbxVector4 v1=fbxMesh.GetControlPointAt(vertexIndex0);
		v1=geometryOffset.MultNormalize(v1);
		FbxVector4 v2=fbxMesh.GetControlPointAt(vertexIndex1);
		v2=geometryOffset.MultNormalize(v2);
		FbxVector4 v3=fbxMesh.GetControlPointAt(vertexIndex2);
		v3=geometryOffset.MultNormalize(v3);

		FbxVector4 n1;
		fbxMesh.GetPolygonVertexNormal(x, 0, n1);
		n1=geometryOffset.MultNormalize(n1);
		n1.Normalize();
		normalbuffer[vertexIndex0]+=n1;
		normalbufferCount[vertexIndex0]++;

		FbxVector4 n2;
		fbxMesh.GetPolygonVertexNormal(x, 1, n2);
		n2=geometryOffset.MultNormalize(n2);
		n2.Normalize();
		normalbuffer[vertexIndex1]+=n2;
		normalbufferCount[vertexIndex1]++;

		FbxVector4 n3;
		fbxMesh.GetPolygonVertexNormal(x, 2, n3);
		n3=geometryOffset.MultNormalize(n3);
		n3.Normalize();
		normalbuffer[vertexIndex2]+=n3;
		normalbufferCount[vertexIndex2]++;

		bool pUnmapped;
		FbxVector2 w1;
		fbxMesh.GetPolygonVertexUV(x, 0, uvname, w1, pUnmapped);
		w1.mData[1]=1.0f-(float)w1.mData[1];
		FbxVector2 w2;
		fbxMesh.GetPolygonVertexUV(x, 1, uvname, w2, pUnmapped);
		w2.mData[1]=1.0f-(float)w2.mData[1];
		FbxVector2 w3;
		fbxMesh.GetPolygonVertexUV(x, 2, uvname, w3, pUnmapped);
		w3.mData[1]=1.0f-(float)w3.mData[1];

		float x1 = v2.mData[0] - v1.mData[0];
		float x2 = v3.mData[0] - v1.mData[0];
		float y1 = v2.mData[1] - v1.mData[1];
		float y2 = v3.mData[1] - v1.mData[1];
		float z1 = v2.mData[2] - v1.mData[2];
		float z2 = v3.mData[2] - v1.mData[2];
        
		float s1 = w2.mData[0] - w1.mData[0];
		float s2 = w3.mData[0] - w1.mData[0];
		float t1 = w2.mData[1] - w1.mData[1];
		float t2 = w3.mData[1] - w1.mData[1];
        
		float r = 1.0F / (s1 * t2 - s2 * t1);
		vector3f sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
				(t2 * z1 - t1 * z2) * r);
		vector3f tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
				(s1 * z2 - s2 * z1) * r);

		tan1[vertexIndex0] += sdir;
		tan1[vertexIndex1] += sdir;
		tan1[vertexIndex2] += sdir;
        
		tan2[vertexIndex0] += tdir;
		tan2[vertexIndex1] += tdir;
		tan2[vertexIndex2] += tdir;
	}

	for (long a = 0; a < vertexCount; a++)
    {
        FbxVector4& temp = normalbuffer[a]/normalbufferCount[a];
		temp.Normalize();
		vector3f n(temp.mData[0], temp.mData[1], temp.mData[2]);
        vector3f& t = tan1[a];
        
        // Gram-Schmidt orthogonalize
		vector3f ortho_val(t - n * n.dot(t));
		ortho_val.normalize();

		tangentbuffer[a].mData[0] = ortho_val.x;
		tangentbuffer[a].mData[1] = ortho_val.y;
		tangentbuffer[a].mData[2] = ortho_val.z;
        
        // Calculate handedness
		tangentbuffer[a].mData[3] = (((n.cross(t)).dot(tan2[a])) < 0.0F) ? -1.0F : 1.0F;
    }

	float* meshtangentBuffer = newMesh->allocateTangentBuffer(fbxMesh.GetPolygonCount());
	for(int x=0;x<fbxMesh.GetPolygonCount();x++)
	{
		int vertexIndices[]={
			x*3+0,
			x*3+1,
			x*3+2
		};

		for(int y=0;y<fbxMesh.GetPolygonSize(x);y++)
		{
			int vertexIndex=fbxMesh.GetPolygonVertex(x, y);

			FbxVector4 t=tangentbuffer[vertexIndex];

			meshtangentBuffer[vertexIndices[y]*4+0]=(float)t.mData[0];
			meshtangentBuffer[vertexIndices[y]*4+1]=(float)t.mData[1];
			meshtangentBuffer[vertexIndices[y]*4+2]=(float)t.mData[2];
			meshtangentBuffer[vertexIndices[y]*4+3]=(float)t.mData[3];
		}
	}


	delete [] tan1;
	delete [] normalbuffer;
	delete [] tangentbuffer;
	delete [] normalbufferCount;

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

/*
void fbxImporter::PrintTabs()
{
	for(int i = 0; i < numTabs; i++)
		printf("\t");
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


void fbxImporter::PrintAttribute(FbxNodeAttribute* pAttribute)
{
	if(!pAttribute) return;
 
	FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
	FbxString attrName = pAttribute->GetName();
	PrintTabs();
	// Note: to retrieve the chararcter array of a FbxString, use its Buffer() method.
	printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}


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
*/