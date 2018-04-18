#pragma once
#include <windows.h>
#include <DirectXMath.h>

#define MAX_FILENAME 100
namespace MyLibrary
{
	struct Vertex
	{
		 double vertex_position[3];
		 double vertex_UVCoord[2];
		 double vertex_normal[3];
	};
	struct Transform {
		float transform_position[4];
		float transform_scale[4];
		float transform_rotation[4];
	};
	struct MeshHeader {

		unsigned int mesh_nrOfVertices;
		char mesh_meshID[MAX_FILENAME];
		//unsigned int mesh_materialID;
	};
	struct MaterialHeader
	{
		unsigned int material_nrOfMaterials;
	};
	struct Material
	{
		char diffuse_pathName[MAX_FILENAME];
		char specular_pathName[MAX_FILENAME];
		char normal_pathName[MAX_FILENAME];
		//char emmissive_pathName[MAX_FILENAME];
	};
	struct CameraHeader
	{
		unsigned int camera_nrOfCameras;
	};
	struct Camera
	{
		Transform cam_transform;
		float cam_FOV;
	};
	struct LightHeader
	{
		unsigned int light_nrofLights;
	};
	struct Light
	{
		Transform light_transform;
		float light_color[4]; //4th value is intensity
	};
	struct MorphAnimationHeader
	{
		unsigned int morph_nrOfTargets;
		unsigned int morph_meshID[20];
	};
	struct Target
	{
		unsigned int morphTarget_nrOfvertices;
	};
	struct SkeletonHeader
	{
		unsigned int skeleton_nrOfJoints;
	};

	struct Joint
	{
		Transform joint_transform;
		float joint_orientation[4];
	};

	struct AnimationHeader
	{
		unsigned int anim_nrOfKeys;

	};
	struct AnimKey
	{
		Transform jointTransform;
	};
	//Not yet 100% sure how to solve the groups, it will depend a bit on how we solve some other things in the game...
	//May be stored by name
	struct GroupHeader
	{
		unsigned int group_itemID;
	};
	struct Group
	{
		char groupItemName[100];
	};


	//Structs for returning
//--------------------------------------//
	struct VertexFromFile
	{
		float vertex_position[3];
		float vertex_UVCoord[2];
		float vertex_normal[3];
		float vertex_tangent[3];
		float vertex_biTangent[3];
	};

	struct MeshFromFile
	{
		unsigned int mesh_nrOfVertices;
		char mesh_meshID[MAX_FILENAME];
		//unsigned int mesh_materialID;
		VertexFromFile* mesh_vertices;
	};

	struct CameraFromFile
	{
		unsigned int cam_nrOfCameras;
		Camera* cam_cameras;
	};

	struct MaterialFromFile
	{
		unsigned int mat_nrOfMaterials;
		Material* mat_materials;
	};

	struct LightFromFile
	{
		unsigned int light_nrofLights;
		Light* light_lights;
	};

	struct MorphAnimationFromFile
	{

	};

	struct SkeletonFromFile
	{
		unsigned int skeleton_nrOfJoints;
		Joint* skeleton_joints;
	};

	struct AnimationFromFile
	{

	};
}