#include "formatImporter.h"
namespace MyLibrary
{
	Loadera::Loadera()
	{

	}

	Loadera::~Loadera()
	{
	}

	bool Loadera::readMeshFile(std::string fileName, MeshFromFile *inMesh)
	{
		
		bool fileOpened = false;
		std::ifstream customMeshFile(fileName, std::ifstream::binary);
		
		if (customMeshFile.is_open())
		{
			fileOpened = true;
			MeshHeader meshname;

			customMeshFile.read((char*)&meshname, sizeof(MeshHeader));

			inMesh->mesh_nrOfVertices = meshname.mesh_nrOfVertices;

			Vertex* vertices = new Vertex[meshname.mesh_nrOfVertices];
			customMeshFile.read((char*)vertices, meshname.mesh_nrOfVertices * sizeof(Vertex));

			VertexFromFile tempVert;

			for (int i = 0; i < meshname.mesh_nrOfVertices; i++)
			{
				tempVert.vertex_position[0] = vertices[i].vertex_position[0];
				tempVert.vertex_position[1] = vertices[i].vertex_position[1];
				tempVert.vertex_position[2] = vertices[i].vertex_position[2];

				tempVert.vertex_UVCoord[0] = vertices[i].vertex_UVCoord[0];
				tempVert.vertex_UVCoord[1] = vertices[i].vertex_UVCoord[1];
				
				tempVert.vertex_normal[0] =vertices[i].vertex_normal[0];
				tempVert.vertex_normal[1] =vertices[i].vertex_normal[1];
				tempVert.vertex_normal[2] =vertices[i].vertex_normal[2];
				inMesh->mesh_vertices.push_back(tempVert);
			}															 
			

			for (int i = 0; i < MAX_FILENAME; i++)
			{
				inMesh->mesh_meshID[i] = meshname.mesh_meshID[i];
			}
			calculateTangentsAndBitangents(inMesh);
			delete vertices;
			customMeshFile.close();
		}
		return fileOpened;
	//
	}

	bool MyLibrary::Loadera::readAnimatedMeshFile(std::string fileName, AnimatedMeshFromFile *inMesh)
	{
		std::ifstream customMeshFile(fileName, std::ifstream::binary);
		bool set = false;
		if (customMeshFile.is_open())
		{
			set = true;
			MeshHeader meshname;

			customMeshFile.read((char*)&meshname, sizeof(MeshHeader));

			inMesh->mesh_nrOfVertices = meshname.mesh_nrOfVertices;

			AnimatedVertex* vertices = new AnimatedVertex[meshname.mesh_nrOfVertices];
			customMeshFile.read((char*)vertices, meshname.mesh_nrOfVertices * sizeof(AnimatedVertex));

			AnimatedVertexFromFile tempVert;
			for (int i = 0; i < meshname.mesh_nrOfVertices; i++)
			{
				
				tempVert.vertex_position[0] = vertices[i].vertex_position[0];
				tempVert.vertex_position[1] = vertices[i].vertex_position[1];
				tempVert.vertex_position[2] = vertices[i].vertex_position[2];
				
				tempVert.vertex_UVCoord[0] = vertices[i].vertex_UVCoord[0];
				tempVert.vertex_UVCoord[1] = vertices[i].vertex_UVCoord[1];
			
				tempVert.vertex_normal[0] = vertices[i].vertex_normal[0];
				tempVert.vertex_normal[1] = vertices[i].vertex_normal[1];
				tempVert.vertex_normal[2] = vertices[i].vertex_normal[2];

				tempVert.influencing_joint[0] = vertices[i].influencing_joint[0];
				tempVert.influencing_joint[1] = vertices[i].influencing_joint[1];
				tempVert.influencing_joint[2] = vertices[i].influencing_joint[2];
				tempVert.influencing_joint[3] = vertices[i].influencing_joint[3];
			
				tempVert.joint_weights[0] = vertices[i].joint_weights[0];
				tempVert.joint_weights[1] = vertices[i].joint_weights[1];
				tempVert.joint_weights[2] = vertices[i].joint_weights[2];
				tempVert.joint_weights[3] = vertices[i].joint_weights[3];
				inMesh->mesh_vertices.push_back(tempVert);
			}


			for (int i = 0; i < MAX_FILENAME; i++)
			{
				inMesh->mesh_meshID[i] = meshname.mesh_meshID[i];
			}

			calculateTangentsAndBitangents(inMesh);

			delete vertices;
			customMeshFile.close();
		}
		return set;

		
	}

	bool Loadera::readCameraFile(std::string fileName, CamerasFromFile *inCamera)
	{
		bool fileOpened = false;
		CamerasFromFile cameraToReturn;

		std::ifstream customCameraFile(fileName, std::ifstream::binary);
		if (customCameraFile.is_open())
		{
			fileOpened = true;
			CameraHeader cameraheader;
			customCameraFile.read((char*)&cameraheader, sizeof(CameraHeader));
			Camera tempCam;


			inCamera->cam_nrOfCameras = cameraheader.camera_nrOfCameras;

			for (int i = 0; i < cameraheader.camera_nrOfCameras; i++)
			{
				customCameraFile.read((char*)&tempCam, sizeof(Camera));
				inCamera->cam_cameras.push_back(tempCam);
			}
			customCameraFile.close();
		}
		return fileOpened;
	}



	bool Loadera::readMorphAnimationFile(std::string fileName,  MorphAnimationFromFile *inMorph)
	{
		bool fileIsOpen = false;

		std::ifstream customMorphAnimationFile(fileName, std::ifstream::binary);
		if (customMorphAnimationFile.is_open())
		{
			fileIsOpen = true;
			MorphAnimationHeader morpHeader;
			MorphAnimVert tempMorpHvert;
			customMorphAnimationFile.read((char*)&morpHeader, sizeof(MorphAnimationHeader));

			inMorph->nrOfShapes = morpHeader.morph_nrOfTargets;
			for (int i = 0; i < 100; i++)
			{
				inMorph->mesh_ID[i] = morpHeader.morph_ID[i];
			}
			for (int i = 0; i < morpHeader.morph_nrOfTargets; i++)
			{
				customMorphAnimationFile.read((char*)&tempMorpHvert, sizeof(MorphAnimVert));
				inMorph->transforms.push_back(tempMorpHvert);
			}
			customMorphAnimationFile.close();
		}

		return fileIsOpen;
	}

	bool Loadera::readSkeletonFile(std::string fileName, SkeletonFromFile *inSkeleton)
	{
		bool fileOpened = false;
		
		std::ifstream customSkeletonFile(fileName, std::ifstream::binary);
		if (customSkeletonFile.is_open())
		{
			fileOpened = true;
			SkeletonHeader skeleton_header;
			customSkeletonFile.read((char*)&skeleton_header, sizeof(SkeletonHeader));

			Joint* tempJoint = new Joint[skeleton_header.skeleton_nrOfJoints];
			customSkeletonFile.read((char*)tempJoint, skeleton_header.skeleton_nrOfJoints * sizeof(Joint));
			for (int i = 0; i < skeleton_header.skeleton_nrOfJoints; i++)
			{
				inSkeleton->skeleton_joints.push_back(tempJoint[i]);
				for (int j = 0; j < MAX_FILENAME; j++)
					inSkeleton->skeleton_joints[i].joint_name[j] = tempJoint[i].joint_name[j];
				inSkeleton->skeleton_joints[i].joint_transform = tempJoint[i].joint_transform;
				inSkeleton->skeleton_joints[i].parentIndex = tempJoint[i].parentIndex;

			}
			
			inSkeleton->skeleton_nrOfJoints = skeleton_header.skeleton_nrOfJoints;
			for(int i = 0; i < MAX_FILENAME; i++)
				inSkeleton->skeletonID[i] = skeleton_header.skeletonID[i];
			delete tempJoint;
			customSkeletonFile.close();
		}

		return fileOpened;
	}

	bool Loadera::readAnimationFile(std::string fileName, int nrOfJoints, AnimationFromFile *inAnim)
	{
		bool fileIsOpen = false;

		std::ifstream customAnimationFile(fileName, std::ifstream::binary);
		AnimationHeader animation_header;
		

		if (customAnimationFile.is_open())
		{
			
			fileIsOpen = true;
			customAnimationFile.read((char*)&animation_header, sizeof(AnimationHeader));
			inAnim->nr_of_keyframes = animation_header.anim_nrOfKeys;
			Transform* keyframes = new Transform[animation_header.anim_nrOfKeys*nrOfJoints];
			customAnimationFile.read((char*)keyframes, animation_header.anim_nrOfKeys * nrOfJoints * sizeof(Transform));

			
			Transform tempKeyFrame;
			for (int i = 0; i < animation_header.anim_nrOfKeys * nrOfJoints; i++)
			{
				tempKeyFrame = keyframes[i];

				inAnim->keyframe_transformations.push_back(tempKeyFrame);
			}
			delete keyframes;
			customAnimationFile.close();
		}
		return fileIsOpen;
		
	}

	bool Loadera::readGroupFile(std::string fileName, Group *inGroup)
	{
		bool fileIsOpen = false;

		std::ifstream customGroupFile(fileName, std::ifstream::binary);
		if (customGroupFile.is_open())
		{
			fileIsOpen = true;
			GroupHeader groupheader;
			customGroupFile.read((char*)&groupheader, sizeof(GroupHeader));
			
			
			Transform tempTransform;
			customGroupFile.read((char*)&tempTransform, sizeof(Transform));
			inGroup->group_transform = tempTransform;

			groupChild tempGroupChild;
			for (int i = 0; i < groupheader.items_in_group; i++)
			{
				customGroupFile.read((char*)&tempGroupChild, sizeof(groupChild));
				inGroup->children_names.push_back(tempGroupChild);

			}
			customGroupFile.close();
		}

		return fileIsOpen;
	}



	bool Loadera::readLightFile(std::string fileName, LightFromFile *inLight)
	{
		bool fileIsOpen = false;

		std::ifstream customLightFile(fileName, std::ifstream::binary);
		if (customLightFile.is_open())
		{
			Light tempLight;

			fileIsOpen = true;

			LightHeader lightname;
			customLightFile.read((char*)&lightname, sizeof(LightHeader));

			for (int i = 0; i < lightname.light_nrofLights; i++)
			{
				customLightFile.read((char*)&tempLight, sizeof(Light));
				inLight->light_lights.push_back(tempLight);

			}

			inLight->light_nrofLights = lightname.light_nrofLights;

			customLightFile.close();
		}

		return fileIsOpen;
	}

	CustomAttributes Loadera::readCustomAttributeFile(std::string fileName)
	{
		CustomAttributes customAttribute;

		std::ifstream customAttributeFile(fileName, std::ifstream::binary);
		if (customAttributeFile.is_open())
		{
			customAttributeFile.read((char*)&customAttribute.customBool, sizeof(customAttribute.customBool));
			customAttributeFile.read((char*)&customAttribute.customFloat, sizeof(customAttribute.customFloat));
			customAttributeFile.read((char*)&customAttribute.customInt, sizeof(customAttribute.customInt));
			customAttributeFile.read((char*)&customAttribute.customEnum, sizeof(customAttribute.customEnum));
			customAttributeFile.read((char*)&customAttribute.customVector, sizeof(customAttribute.customVector));
			customAttributeFile.read((char*)&customAttribute.customString, sizeof(customAttribute.customString));		
			customAttributeFile.close();
		}

		return customAttribute;
	}

	MaterialFromFile Loadera::readMaterialFile(std::string fileName)
	{
		

		MaterialFromFile materialToReturn;

		bool fileIsOpen = false;

		std::ifstream customMaterialFile(fileName, std::ifstream::binary);
		if (customMaterialFile.is_open())
		{
			fileIsOpen = true;
			Material tempMat;
			MaterialHeader materialname;

			customMaterialFile.read((char*)&materialname, sizeof(materialname));
			materialToReturn.mat_nrOfMaterials = materialname.material_nrOfMaterials;
			materialToReturn.mat_nrOfTextures = materialname.material_nrOfTextures;

			customMaterialFile.read((char*)&tempMat,sizeof(Material));
			materialToReturn.mat_materials.push_back(tempMat);
			customMaterialFile.close();
		}

		return materialToReturn;
	}
template<class T> 
void MyLibrary::Loadera::calculateTangentsAndBitangents(T mesh)
{
	for (int i = 0; i < mesh->mesh_nrOfVertices; i += 3)
	{
		//mesh->mesh_vertices[i].vertex_position;
		float vec1[3];
		float vec2[3];

		vec1[0] = mesh->mesh_vertices[i + 1].vertex_position[0] - mesh->mesh_vertices[i].vertex_position[0];
		vec1[1] = mesh->mesh_vertices[i + 1].vertex_position[1] - mesh->mesh_vertices[i].vertex_position[1];
		vec1[2] = mesh->mesh_vertices[i + 1].vertex_position[2] - mesh->mesh_vertices[i].vertex_position[2];

		vec2[0] = mesh->mesh_vertices[i + 2].vertex_position[0] - mesh->mesh_vertices[i].vertex_position[0];
		vec2[1] = mesh->mesh_vertices[i + 2].vertex_position[1] - mesh->mesh_vertices[i].vertex_position[1];
		vec2[2] = mesh->mesh_vertices[i + 2].vertex_position[2] - mesh->mesh_vertices[i].vertex_position[2];

		float uVec1[2];
		float uVec2[2];

		uVec1[0] = mesh->mesh_vertices[i + 1].vertex_UVCoord[0] - mesh->mesh_vertices[i].vertex_UVCoord[0];
		uVec1[1] = mesh->mesh_vertices[i + 1].vertex_UVCoord[1] - mesh->mesh_vertices[i].vertex_UVCoord[1];

		uVec2[0] = mesh->mesh_vertices[i + 2].vertex_UVCoord[0] - mesh->mesh_vertices[i].vertex_UVCoord[0];
		uVec2[1] = mesh->mesh_vertices[i + 2].vertex_UVCoord[1] - mesh->mesh_vertices[i].vertex_UVCoord[1];


		float denominator = (uVec1[0] * uVec2[1]) - (uVec1[1] * uVec2[0]);
		float someFloat = 1.0f / denominator;

		float dVec1[3];
		float dVec2[3];

		dVec1[0] = vec1[0] * uVec2[1];
		dVec1[1] = vec1[1] * uVec2[1];
		dVec1[2] = vec1[2] * uVec2[1];

		dVec2[0] = vec2[0] * uVec1[1];
		dVec2[1] = vec2[1] * uVec1[1];
		dVec2[2] = vec2[2] * uVec1[1];

		float tan[3];
		tan[0] = dVec1[0] - dVec2[0];
		tan[1] = dVec1[1] - dVec2[1];
		tan[2] = dVec1[2] - dVec2[2];

		tan[0] = tan[0] * someFloat;
		tan[1] = tan[1] * someFloat;
		tan[2] = tan[2] * someFloat;


		//tangent			//store the tangent values 
		//DirectX::XMVECTOR tangent = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&tan));			//normalize?
		mesh->mesh_vertices[i].vertex_tangent[0] = tan[0];
		mesh->mesh_vertices[i].vertex_tangent[1] = tan[1];
		mesh->mesh_vertices[i].vertex_tangent[2] = tan[2];

		mesh->mesh_vertices[i + 1].vertex_tangent[0] = tan[0];
		mesh->mesh_vertices[i + 1].vertex_tangent[1] = tan[1];
		mesh->mesh_vertices[i + 1].vertex_tangent[2] = tan[2];

		mesh->mesh_vertices[i + 2].vertex_tangent[0] = tan[0];
		mesh->mesh_vertices[i + 2].vertex_tangent[1] = tan[1];
		mesh->mesh_vertices[i + 2].vertex_tangent[2] = tan[2];

		//bitangent
		//DirectX::XMVECTOR bitangent = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&vertex[i].tangent), DirectX::XMLoadFloat3(&vertex[i].normal)));
		float bitangent[3];
		bitangent[0] = (mesh->mesh_vertices[i].vertex_tangent[1] * mesh->mesh_vertices[i].vertex_normal[2] - mesh->mesh_vertices[i].vertex_tangent[2] * mesh->mesh_vertices[i].vertex_normal[1]);
		bitangent[1] = (mesh->mesh_vertices[i].vertex_tangent[2] * mesh->mesh_vertices[i].vertex_normal[0] - mesh->mesh_vertices[i].vertex_tangent[0] * mesh->mesh_vertices[i].vertex_normal[2]);
		bitangent[2] = (mesh->mesh_vertices[i].vertex_tangent[0] * mesh->mesh_vertices[i].vertex_normal[1] - mesh->mesh_vertices[i].vertex_tangent[1] * mesh->mesh_vertices[i].vertex_normal[0]);

		//bitangent[0] = (mesh->mesh_vertices[i + 1].vertex_tangent[1] * mesh->mesh_vertices[i + 1].vertex_normal[2] - mesh->mesh_vertices[i + 1].vertex_tangent[2] * mesh->mesh_vertices[i + 1].vertex_normal[1]);
		//bitangent[1] = (mesh->mesh_vertices[i + 1].vertex_tangent[2] * mesh->mesh_vertices[i + 1].vertex_normal[0] - mesh->mesh_vertices[i + 1].vertex_tangent[0] * mesh->mesh_vertices[i + 1].vertex_normal[2]);
		//bitangent[2] = (mesh->mesh_vertices[i + 1].vertex_tangent[0] * mesh->mesh_vertices[i + 1].vertex_normal[1] - mesh->mesh_vertices[i + 1].vertex_tangent[1] * mesh->mesh_vertices[i + 1].vertex_normal[0]);
		//								   
		//bitangent[0] = (mesh->mesh_vertices[i + 2].vertex_tangent[1] * mesh->mesh_vertices[i + 2].vertex_normal[2] - mesh->mesh_vertices[i + 2].vertex_tangent[2] * mesh->mesh_vertices[i + 2].vertex_normal[1]);
		//bitangent[1] = (mesh->mesh_vertices[i + 2].vertex_tangent[2] * mesh->mesh_vertices[i + 2].vertex_normal[0] - mesh->mesh_vertices[i + 2].vertex_tangent[0] * mesh->mesh_vertices[i + 2].vertex_normal[2]);
		//bitangent[2] = (mesh->mesh_vertices[i + 2].vertex_tangent[0] * mesh->mesh_vertices[i + 2].vertex_normal[1] - mesh->mesh_vertices[i + 2].vertex_tangent[1] * mesh->mesh_vertices[i + 2].vertex_normal[0]);

		mesh->mesh_vertices[i].vertex_biTangent[0] = bitangent[0];
		mesh->mesh_vertices[i].vertex_biTangent[1] = bitangent[1];
		mesh->mesh_vertices[i].vertex_biTangent[2] = bitangent[2];

		mesh->mesh_vertices[i + 1].vertex_biTangent[0] = bitangent[0];
		mesh->mesh_vertices[i + 1].vertex_biTangent[1] = bitangent[1];
		mesh->mesh_vertices[i + 1].vertex_biTangent[2] = bitangent[2];

		mesh->mesh_vertices[i + 2].vertex_biTangent[0] = bitangent[0];
		mesh->mesh_vertices[i + 2].vertex_biTangent[1] = bitangent[1];
		mesh->mesh_vertices[i + 2].vertex_biTangent[2] = bitangent[2];
	}
}
}