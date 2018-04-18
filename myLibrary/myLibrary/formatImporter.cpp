#include "formatImporter.h"
namespace MyLibrary
{
	Loadera::Loadera()
	{

	}

	Loadera::~Loadera()
	{
	}

	MeshFromFile Loadera::readMeshFile(std::string fileName)
	{
		//reads the custom mesh file and stores all the data

		MeshFromFile meshToReturn;

		bool fileIsOpen = false;

		std::ifstream customMeshFile(fileName, std::ifstream::binary);
		//customMeshFile.open(fileName, std::ifstream::binary);

		if (customMeshFile.is_open())
		{
			fileIsOpen = true;

			MeshHeader meshname;

			//customMeshFile.read((char*)&meshname.mesh_nrOfVertices, sizeof(meshname.mesh_nrOfVertices));
			//customMeshFile.read((char*)&meshname.mesh_meshID, sizeof(meshname.mesh_meshID));
			customMeshFile.read((char*)&meshname, sizeof(MeshHeader));

			meshToReturn.mesh_nrOfVertices = meshname.mesh_nrOfVertices;

			Vertex* vertices = new Vertex[meshname.mesh_nrOfVertices];
			customMeshFile.read((char*)vertices, meshname.mesh_nrOfVertices * sizeof(Vertex));

			meshToReturn.mesh_vertices = new VertexFromFile[meshname.mesh_nrOfVertices];

			for (int i = 0; i < meshname.mesh_nrOfVertices; i++)
			{
				//std::cout << vertices[i].vertex_position[0] << ", " << vertices[i].vertex_position[1] << ", " << vertices[i].vertex_position[2] << std::endl;
				meshToReturn.mesh_vertices[i].vertex_position[0] = vertices[i].vertex_position[0];
				meshToReturn.mesh_vertices[i].vertex_position[1] = vertices[i].vertex_position[1];
				meshToReturn.mesh_vertices[i].vertex_position[2] = vertices[i].vertex_position[2];

				meshToReturn.mesh_vertices[i].vertex_UVCoord[0] = vertices[i].vertex_UVCoord[0];
				meshToReturn.mesh_vertices[i].vertex_UVCoord[1] = vertices[i].vertex_UVCoord[1];

				meshToReturn.mesh_vertices[i].vertex_normal[0] =vertices[i].vertex_normal[0];
				meshToReturn.mesh_vertices[i].vertex_normal[1] =vertices[i].vertex_normal[1];
				meshToReturn.mesh_vertices[i].vertex_normal[2] =vertices[i].vertex_normal[2];
			}															 
			

			for (int i = 0; i < MAX_FILENAME; i++)
			{
				meshToReturn.mesh_meshID[i] = meshname.mesh_meshID[i];
			}

			calculateTangentsAndBitangents(meshToReturn);

			//meshToReturn.mesh_meshID = meshname.mesh_meshID;
			//meshToReturn.mesh_materialID = meshname.mesh_materialID;

			customMeshFile.close();
		}

		return meshToReturn;
	}

	CameraFromFile Loadera::readCameraFile(std::string fileName)
	{
		//reads the custom camera file

		CameraFromFile cameraToReturn;

		bool fileIsOpen = false;

		std::ifstream customCameraFile(fileName, std::ifstream::binary);
		if (customCameraFile.is_open())
		{
			fileIsOpen = true;

			CameraHeader cameraname;
			customCameraFile.read((char*)&cameraname, sizeof(cameraname));

			cameraToReturn.cam_cameras = new Camera[cameraname.camera_nrOfCameras];
			customCameraFile.read((char*)cameraToReturn.cam_cameras, cameraname.camera_nrOfCameras * sizeof(Camera));

			cameraToReturn.cam_nrOfCameras = cameraname.camera_nrOfCameras;

			customCameraFile.close();
		}

		return cameraToReturn;
	}

	MaterialFromFile Loadera::readMaterialFile(std::string fileName)
	{
		//read the file for textures

		MaterialFromFile materialToReturn;

		bool fileIsOpen = false;

		std::ifstream customMaterialFile(fileName, std::ifstream::binary);
		if (customMaterialFile.is_open())
		{
			fileIsOpen = true;

			MaterialHeader materialname;
			customMaterialFile.read((char*)&materialname, sizeof(materialname));

			materialToReturn.mat_materials = new Material[materialname.material_nrOfMaterials];
			customMaterialFile.read((char*)materialToReturn.mat_materials, materialname.material_nrOfMaterials * sizeof(Material));

			materialToReturn.mat_nrOfMaterials = materialname.material_nrOfMaterials;

			customMaterialFile.close();
		}

		return materialToReturn;
	}

	LightFromFile Loadera::readLightFile(std::string fileName)
	{
		//read the light file

		LightFromFile lightToReturn;

		bool fileIsOpen = false;

		std::ifstream customLightFile(fileName, std::ifstream::binary);
		if (customLightFile.is_open())
		{
			fileIsOpen = true;

			LightHeader lightname;
			customLightFile.read((char*)&lightname, sizeof(lightname));

			lightToReturn.light_lights = new Light[lightname.light_nrofLights];
			customLightFile.read((char*)lightToReturn.light_lights, lightname.light_nrofLights * sizeof(Light));

			lightToReturn.light_nrofLights = lightname.light_nrofLights;

			customLightFile.close();
		}

		return lightToReturn;
	}

	bool Loadera::readMorphAnimationFile(std::string fileName)
	{
		bool fileIsOpen = false;

		std::ifstream customMorphAnimationFile(fileName, std::ifstream::binary);
		if (customMorphAnimationFile.is_open())
		{
			fileIsOpen = true;

			customMorphAnimationFile.close();
		}

		return fileIsOpen;
	}

	SkeletonFromFile Loadera::readSkeletonFile(std::string fileName)
	{
		//read the skeleton file

		SkeletonFromFile skeletonToReturn;

		bool fileIsOpen = false;

		std::ifstream customSkeletonFile(fileName, std::ifstream::binary);
		if (customSkeletonFile.is_open())
		{
			fileIsOpen = true;

			SkeletonHeader skeletonname;
			customSkeletonFile.read((char*)&skeletonname, sizeof(skeletonname));

			skeletonToReturn.skeleton_joints = new Joint[skeletonname.skeleton_nrOfJoints];
			customSkeletonFile.read((char*)skeletonToReturn.skeleton_joints, skeletonname.skeleton_nrOfJoints * sizeof(Joint));

			skeletonToReturn.skeleton_nrOfJoints = skeletonname.skeleton_nrOfJoints;

			customSkeletonFile.close();
		}

		return skeletonToReturn;
	}

	bool Loadera::readAnimationFile(std::string fileName)
	{
		bool fileIsOpen = false;

		std::ifstream customAnimationFile(fileName, std::ifstream::binary);
		if (customAnimationFile.is_open())
		{
			fileIsOpen = true;

			customAnimationFile.close();
		}

		return fileIsOpen;
	}

	bool Loadera::readGroupFile(std::string fileName)
	{
		bool fileIsOpen = false;

		std::ifstream customGroupFile(fileName, std::ifstream::binary);
		if (customGroupFile.is_open())
		{
			fileIsOpen = true;

			customGroupFile.close();
		}

		return fileIsOpen;
	}

	int Loadera::getNrOfVerticesFromFile(std::ifstream & file)
	{
		//read number of vertices in mesh from custom file
		int nrOfVerticesInFile = 0;


		return nrOfVerticesInFile;
	}

	void Loadera::calculateTangentsAndBitangents(MeshFromFile mesh)
	{
		for (int i = 0; i < mesh.mesh_nrOfVertices; i += 3)
		{
			//mesh.mesh_vertices[i].vertex_position;
			float vec1[3];
			float vec2[3];

			vec1[0] = mesh.mesh_vertices[i + 1].vertex_position[0] - mesh.mesh_vertices[i].vertex_position[0];
			vec1[1] = mesh.mesh_vertices[i + 1].vertex_position[1] - mesh.mesh_vertices[i].vertex_position[1];
			vec1[2] = mesh.mesh_vertices[i + 1].vertex_position[2] - mesh.mesh_vertices[i].vertex_position[2];

			vec2[0] = mesh.mesh_vertices[i + 2].vertex_position[0] - mesh.mesh_vertices[i].vertex_position[0];
			vec2[1] = mesh.mesh_vertices[i + 2].vertex_position[1] - mesh.mesh_vertices[i].vertex_position[1];
			vec2[2] = mesh.mesh_vertices[i + 2].vertex_position[2] - mesh.mesh_vertices[i].vertex_position[2];


			float uVec1[2];
			float uVec2[2];

			uVec1[0] = mesh.mesh_vertices[i + 1].vertex_UVCoord[0] - mesh.mesh_vertices[i].vertex_UVCoord[0];
			uVec1[1] = mesh.mesh_vertices[i + 1].vertex_UVCoord[1] - mesh.mesh_vertices[i].vertex_UVCoord[1];

			uVec2[0] = mesh.mesh_vertices[i + 2].vertex_UVCoord[0] - mesh.mesh_vertices[i].vertex_UVCoord[0];
			uVec2[1] = mesh.mesh_vertices[i + 2].vertex_UVCoord[1] - mesh.mesh_vertices[i].vertex_UVCoord[1];


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
			mesh.mesh_vertices[i].vertex_tangent[0] = tan[0];
			mesh.mesh_vertices[i].vertex_tangent[1] = tan[1];
			mesh.mesh_vertices[i].vertex_tangent[2] = tan[2];

			mesh.mesh_vertices[i + 1].vertex_tangent[0] = tan[0];
			mesh.mesh_vertices[i + 1].vertex_tangent[1] = tan[1];
			mesh.mesh_vertices[i + 1].vertex_tangent[2] = tan[2];

			mesh.mesh_vertices[i + 2].vertex_tangent[0] = tan[0];
			mesh.mesh_vertices[i + 2].vertex_tangent[1] = tan[1];
			mesh.mesh_vertices[i + 2].vertex_tangent[2] = tan[2];

			//bitangent
			//DirectX::XMVECTOR bitangent = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&vertex[i].tangent), DirectX::XMLoadFloat3(&vertex[i].normal)));
			float bitangent[3];
			bitangent[0] = (mesh.mesh_vertices[i].vertex_tangent[1] * mesh.mesh_vertices[i].vertex_normal[2] - mesh.mesh_vertices[i].vertex_tangent[2] * mesh.mesh_vertices[i].vertex_normal[1]);
			bitangent[1] = (mesh.mesh_vertices[i].vertex_tangent[2] * mesh.mesh_vertices[i].vertex_normal[0] - mesh.mesh_vertices[i].vertex_tangent[0] * mesh.mesh_vertices[i].vertex_normal[2]);
			bitangent[2] = (mesh.mesh_vertices[i].vertex_tangent[0] * mesh.mesh_vertices[i].vertex_normal[1] - mesh.mesh_vertices[i].vertex_tangent[1] * mesh.mesh_vertices[i].vertex_normal[0]);

			//bitangent[0] = (mesh.mesh_vertices[i + 1].vertex_tangent[1] * mesh.mesh_vertices[i + 1].vertex_normal[2] - mesh.mesh_vertices[i + 1].vertex_tangent[2] * mesh.mesh_vertices[i + 1].vertex_normal[1]);
			//bitangent[1] = (mesh.mesh_vertices[i + 1].vertex_tangent[2] * mesh.mesh_vertices[i + 1].vertex_normal[0] - mesh.mesh_vertices[i + 1].vertex_tangent[0] * mesh.mesh_vertices[i + 1].vertex_normal[2]);
			//bitangent[2] = (mesh.mesh_vertices[i + 1].vertex_tangent[0] * mesh.mesh_vertices[i + 1].vertex_normal[1] - mesh.mesh_vertices[i + 1].vertex_tangent[1] * mesh.mesh_vertices[i + 1].vertex_normal[0]);
			//								   
			//bitangent[0] = (mesh.mesh_vertices[i + 2].vertex_tangent[1] * mesh.mesh_vertices[i + 2].vertex_normal[2] - mesh.mesh_vertices[i + 2].vertex_tangent[2] * mesh.mesh_vertices[i + 2].vertex_normal[1]);
			//bitangent[1] = (mesh.mesh_vertices[i + 2].vertex_tangent[2] * mesh.mesh_vertices[i + 2].vertex_normal[0] - mesh.mesh_vertices[i + 2].vertex_tangent[0] * mesh.mesh_vertices[i + 2].vertex_normal[2]);
			//bitangent[2] = (mesh.mesh_vertices[i + 2].vertex_tangent[0] * mesh.mesh_vertices[i + 2].vertex_normal[1] - mesh.mesh_vertices[i + 2].vertex_tangent[1] * mesh.mesh_vertices[i + 2].vertex_normal[0]);

			mesh.mesh_vertices[i].vertex_biTangent[0] = bitangent[0];
			mesh.mesh_vertices[i].vertex_biTangent[1] = bitangent[1];
			mesh.mesh_vertices[i].vertex_biTangent[2] = bitangent[2];

			mesh.mesh_vertices[i + 1].vertex_biTangent[0] = bitangent[0];
			mesh.mesh_vertices[i + 1].vertex_biTangent[1] = bitangent[1];
			mesh.mesh_vertices[i + 1].vertex_biTangent[2] = bitangent[2];

			mesh.mesh_vertices[i + 2].vertex_biTangent[0] = bitangent[0];
			mesh.mesh_vertices[i + 2].vertex_biTangent[1] = bitangent[1];
			mesh.mesh_vertices[i + 2].vertex_biTangent[2] = bitangent[2];
		}
	}
}