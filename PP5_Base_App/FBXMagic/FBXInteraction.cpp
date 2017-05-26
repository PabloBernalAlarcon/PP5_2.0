#include "stdafx.h"
#include "FBXInteraction.h"
#include <queue>
using namespace fbxsdk;

namespace FBXinteracts {

	FbxNode * root;
	FbxScene* lScene;
	int Functions::GimmeSomething() 
	{
		 
		return FBXSDK_INT_MAX;
	}

	/* Tab character ("\t") counter */
	int numTabs = 0;
	std::vector<float> Positions;
	std::vector<vert> becky;
	std::vector<uint32_t> Indices;
	/**
	* Print the required number of tabs.
	*/
	void /*Functions::*/PrintTabs() {
		for (int i = 0; i < numTabs; i++)
			printf("\t");
	}

	/**
	* Return a string-based representation based on the attribute type.
	*/
	FbxString /*Functions::*/GetAttributeTypeName(FbxNodeAttribute::EType type) {
		switch (type) {
		case FbxNodeAttribute::eUnknown: return "unidentified";
		case FbxNodeAttribute::eNull: return "null";
		case FbxNodeAttribute::eMarker: return "marker";
		case FbxNodeAttribute::eSkeleton: return "skeleton";
		case FbxNodeAttribute::eMesh: return "mesh";
		case FbxNodeAttribute::eNurbs: return "nurbs";
		case FbxNodeAttribute::ePatch: return "patch";
		case FbxNodeAttribute::eCamera: return "camera";
		case FbxNodeAttribute::eCameraStereo: return "stereo";
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

	/**
	* Print an attribute.
	*/
	void /*Functions::*/PrintAttribute(FbxNodeAttribute* pAttribute) {
		if (!pAttribute) return;

		FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
		FbxString attrName = pAttribute->GetName();
		PrintTabs();
		// Note: to retrieve the character array of a FbxString, use its Buffer() method.
		printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
	}
	FbxPose * BindPose;
	struct my_fbx_joint { FbxNode* node; int parent_index; };

	void LoadBone(FbxNode * pNode, FbxDouble3 StoredP, KeyFrame * currFrame, FbxTime duration) {

		vert parent;
		for (int i = 0; i < pNode->GetChildCount(); i++)
		{
			vert child;
			//FbxDouble3 translationChild = pNode->GetChild(i)->LclTranslation.Get();
			FbxDouble3 translationChild = pNode->GetChild(i)->EvaluateGlobalTransform(duration).GetT();
			parent.Position[0] = StoredP[0];
			parent.Position[1] = StoredP[1];
			parent.Position[2] = StoredP[2];
			parent.Position[3] = 1.0f;
			
			child.Position[0] = (translationChild[0] );
			child.Position[1] = (translationChild[1]);
			child.Position[2] = (translationChild[2]);
			child.Position[3] = (1.0f);
			currFrame->bones.push_back(parent);
			currFrame->bones.push_back(child);
			FbxDouble3 P;
			P[0] = translationChild[0];
			P[1] = translationChild[1];
			P[2] = translationChild[2];
			LoadBone(pNode->GetChild(i), P,currFrame,duration);
			//me, child, call func on child
		}
	}



	void LoadNode(FbxNode * pNode) {
		
		FbxMesh * leMesh = pNode->GetMesh();
		FbxGeometryElementNormal* lNormalElement = leMesh->GetElementNormal();
		std::vector<vert> tempvert;
		
	
		
		for (int i = 0; i < leMesh->GetControlPointsCount(); i++)
		{
			FbxVector4 holden = leMesh->GetControlPointAt(i);
			int lNormalIndex = 0;
			if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				lNormalIndex = i;
			}

			//reference mode is index-to-direct, get normals by the index-to-direct
			if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
				lNormalIndex = lNormalElement->GetIndexArray().GetAt(i);

			//Got normals of each vertex.
			FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
			
			vert ben; 
			ben.Position[0] = holden.mData[0];
			ben.Position[1] = holden.mData[1];
			ben.Position[2] = holden.mData[2];
			ben.Position[3] = holden.mData[3];
		//	tempvert.push_back(ben);
			becky.push_back(ben);
		}
		FbxStringList list;
		leMesh->GetUVSetNames(list);
		for (int i = 0; i < leMesh->GetPolygonCount(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Indices.push_back(leMesh->GetPolygonVertex(i, j));

				vert ron = becky[leMesh->GetPolygonVertex(i,j)];
				
				FbxVector2 uvs;
				FbxVector4 norms;
				bool map;
				if (leMesh->GetPolygonVertexUV(i, j, list.GetStringAt(0),uvs, map) )
				{
					becky[leMesh->GetPolygonVertex(i,j)].Color[0] = uvs[0];
					becky[leMesh->GetPolygonVertex(i,j)].Color[1] = uvs[1];
					becky[leMesh->GetPolygonVertex(i,j)].Color[2] = 0;
					becky[leMesh->GetPolygonVertex(i,j)].Color[3] = 1;
				}
				if (leMesh->GetPolygonVertexNormal(i,j,norms))
				{
					becky[leMesh->GetPolygonVertex(i,j)].Normals[0] = norms[0];
					becky[leMesh->GetPolygonVertex(i,j)].Normals[1] = norms[1];
					becky[leMesh->GetPolygonVertex(i,j)].Normals[2] = norms[2];
					becky[leMesh->GetPolygonVertex(i,j)].Normals[3] = norms[3];
				}
				
			}
			
		}
	}
	/**
	* Print a node, its attributes, and all its children recursively.
	*/
	void /*Functions::*/PrintNode(FbxNode* pNode) {

		
		PrintTabs();
		const char* nodeName = pNode->GetName();
		const char* attribute = pNode->GetNodeAttribute()->GetTypeName();
		const char* data = "skeleton";
		FbxDouble3 translation = pNode->LclTranslation.Get();
		FbxDouble3 rotation = pNode->LclRotation.Get();
		FbxDouble3 scaling = pNode->LclScaling.Get();
		FbxString att = GetAttributeTypeName(pNode->GetNodeAttribute()->GetAttributeType());
		/*if (Positions.size() == 0)
		{*/
		printf("fjkj %s \n", att);
			if (att == "skeleton")
			{
				printf("<node name='%s' node att='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
					nodeName, attribute,
					translation[0], translation[1], translation[2],
					rotation[0], rotation[1], rotation[2],
					scaling[0], scaling[1], scaling[2]
				);
				
					/*FbxDouble3 translationChild = pNode->LclTranslation.Get();
					Positions.push_back(translationChild[0]);
					Positions.push_back(translationChild[1]);
					Positions.push_back(translationChild[2]);
					Positions.push_back(1.0f);*/
				if (root == nullptr)
				{
					root = pNode;
					FbxAnimStack * Animstack = pNode->GetScene()->GetCurrentAnimationStack();
					fbxsdk::FbxTimeSpan  TimeSpan = Animstack->GetLocalTimeSpan();
					fbxsdk::FbxTime TimeDuration = TimeSpan.GetDuration();
					animation.lenght = TimeDuration.GetSecondDouble();
					FbxLongLong frameCount = TimeDuration.GetFrameCount(FbxTime::EMode::eFrames24);

					for (int i = 1; i <= frameCount; i++)
					{
						KeyFrame currFrame;
						TimeDuration.SetFrame(i, FbxTime::EMode::eFrames24);
						currFrame.time = TimeDuration.GetSecondDouble();
					LoadBone(pNode, pNode->EvaluateGlobalTransform(TimeDuration).GetT(),&currFrame,TimeDuration);
					animation.keys.push_back(currFrame);
					
					}


				}


			}

		//}
		
			if (att == "mesh")
			{
				// Print the contents of the node.
				LoadNode(pNode);
				
					printf("<node name='%s' node att='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
						nodeName, attribute,
						translation[0], translation[1], translation[2],
						rotation[0], rotation[1], rotation[2],
						scaling[0], scaling[1], scaling[2]
					);
			}
		
		numTabs++;

		// Print the node's attributes.
		for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
			PrintAttribute(pNode->GetNodeAttributeByIndex(i));
		
		// Recursively print the children.
		for (int j = 0; j < pNode->GetChildCount(); j++)
			PrintNode(pNode->GetChild(j));

		numTabs--;
		PrintTabs();
		printf("</node>\n");
	}
	void Functions::SetupFBX() 
	{
		//// Change the following filename to a suitable filename value.
		const char* lFilename = "Teddy_Run.fbx";

		//// Initialize the SDK manager. This object handles memory management.
		FbxManager* lSdkManager = FbxManager::Create();

		// Create the IO settings object.
		FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
		lSdkManager->SetIOSettings(ios);

		// Create an importer using the SDK manager.
		FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

		// Use the first argument as the filename for the importer.
		if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) {
			printf("Call to FbxImporter::Initialize() failed.\n");
			printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
			exit(-1);
		}

		//Create a new scene so that it can be populated by the imported file.
		lScene = FbxScene::Create(lSdkManager, "myScene");

	


		// Import the contents of the file into the scene.
		lImporter->Import(lScene);

		// The file is imported, so get rid of the importer.
		lImporter->Destroy();

	


		// Print the nodes of the scene and their attributes recursively.
		// Note that we are not printing the root node because it should
		// not contain any attributes.
		FbxNode* lRootNode = lScene->GetRootNode();
		if (lRootNode) {
			for (int i = 0; i < lRootNode->GetChildCount(); i++)
				PrintNode(lRootNode->GetChild(i));

		}
		// Destroy the SDK manager and all the other objects it was handling.
		lSdkManager->Destroy();
	}

	float Functions::getPositions(unsigned int indicesVec) {
		return Positions[indicesVec];
	}

	unsigned int Functions::getPositionsSize() {
		return Positions.size();
	}

	float Functions::getverts(unsigned int indicesVec, unsigned int indicesArr ) {
		return becky[indicesVec].Position[indicesArr];
	}

	unsigned int Functions::getvertsSize() {
		return becky.size();
	}
	std::vector<vert> Functions::getBecky() {
		return becky;
	}
	std::vector<uint32_t> Functions::getIndices() { return Indices; }
}