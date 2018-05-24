#pragma once
#include <windows.h>
#include <DirectXMath.h>
#include <vector>
#define MAX_FILENAME 100
namespace MyLibrary
{
	struct Vertex
	{
		 float vertex_position[3];
		 float vertex_UVCoord[2];
		 float vertex_normal[3];
	};
	struct AnimatedVertex
	{
		float vertex_position[3];
		float vertex_UVCoord[2];
		float vertex_normal[3];
		unsigned int influencing_joint[4];
		float joint_weights[4];
	};
	struct Transform {
		float transform_position[3];
		float transform_rotation[3];
		float transform_scale[3];
	};
	struct MeshHeader {

		unsigned int mesh_nrOfVertices;
		char mesh_meshID[MAX_FILENAME];
	
	};
	struct MaterialHeader
	{
		unsigned int material_nrOfMaterials;
		unsigned int material_nrOfTextures;
	};
	struct MorphAnimVert
	{
		float position[3];
		float normal[3];
		float UV[2];
	};
	struct Material
	{
		//Lambert
		float AmbientFactor;
		float AmbientColor[3];
		float DiffuseFactor;
		float DiffuseColor[3];
		float EmissiveFactor;
		float EmissiveColor[3];

		float Transparency;

		//Phong
		float SpecularFactor;
		float SpecularColor[3];
		float Shininess;
		float Reflection;
		float ReflectionColor[3];

		//TexturePaths
		char TexturePath[150];
		char BumpPath[150];

	};
	struct MaterialFromFile
	{
		unsigned int mat_nrOfMaterials;
		unsigned int mat_nrOfTextures;
		std::vector<Material>  mat_materials;
	};
	struct CameraHeader
	{
		unsigned int camera_nrOfCameras;
	};
	struct Camera
	{
		char cam_name[100];
		Transform cam_transform;
		float cam_FOV;
	};
	struct LightHeader
	{
		unsigned int light_nrofLights;
	};
	struct Light
	{
		char light_name[100];
		Transform light_transform;
		float light_color[4]; //4th value is intensity
	};
	struct MorphAnimationHeader
	{
		char morph_ID[100];
		unsigned int morph_nrOfTargets;
	};
	struct Target
	{
		unsigned int morphTarget_nrOfvertices;
	};
	struct SkeletonHeader
	{
		unsigned int skeleton_nrOfJoints;
		char skeletonID[MAX_FILENAME];
	};
	struct MorphAnimationFromFile
	{
		char mesh_ID[100];
		unsigned int nrOfShapes;
		std::vector<MorphAnimVert> transforms;
	};

	struct Joint
	{
		char joint_name[100];
		Transform joint_transform;
		unsigned int parentIndex;
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
		unsigned int items_in_group;
	};

	struct groupChild
	{
		char name[100];
	};
	struct Group
	{
		Transform group_transform;
		std::vector<groupChild> children_names;
		//std::vector<Child> children; // doesn't work.
	};
	struct CustomAttributes
	{
		bool customBool;
		float customFloat;
		int customInt;
		int customEnum;
		float customVector[3];
		char customString[128];
		int nrOfCustomAttributes;
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

	struct AnimatedVertexFromFile
	{
		float vertex_position[3];
		float vertex_UVCoord[2];
		float vertex_normal[3];
		float vertex_tangent[3];
		float vertex_biTangent[3];
		unsigned int influencing_joint[4];
		float joint_weights[4];
	};

	struct AnimatedMeshFromFile
	{
		unsigned int mesh_nrOfVertices;
		char mesh_meshID[MAX_FILENAME];
		std::vector<AnimatedVertexFromFile> mesh_vertices;
	};

	struct MeshFromFile
	{
		unsigned int mesh_nrOfVertices;
		char mesh_meshID[MAX_FILENAME];
		std::vector<VertexFromFile> mesh_vertices;
	};

	struct CamerasFromFile
	{
		unsigned int cam_nrOfCameras;
		std::vector<Camera> cam_cameras;
	};


	struct LightFromFile
	{
		unsigned int light_nrofLights;
		std::vector<Light> light_lights;
	};



	struct SkeletonFromFile
	{
		char skeletonID[100];
		unsigned int skeleton_nrOfJoints;
		std::vector<Joint> skeleton_joints;
	};

	struct AnimationFromFile
	{
		unsigned int nr_of_keyframes;
		std::vector<Transform> keyframe_transformations;
	};

}