#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "FormatHeader.h"

namespace MyLibrary
{
	class Loadera
	{
	public:
		Loadera();
		~Loadera();
	
		bool readMeshFile(std::string fileName, MeshFromFile *inMesh);
		bool readAnimatedMeshFile(std::string fileName, AnimatedMeshFromFile *inMesh);
		bool readCameraFile(std::string fileName, CamerasFromFile *inCamera);
		MaterialFromFile readMaterialFile(std::string fileName);
		CustomAttributes readCustomAttributeFile(std::string fileName);
		bool readMorphAnimationFile(std::string fileName, MorphAnimationFromFile *inMorph);
		bool readSkeletonFile(std::string fileName, SkeletonFromFile *inSkeleton);
		bool readAnimationFile(std::string fileName, int nrOfJoints, AnimationFromFile *inAnim);
		bool readGroupFile(std::string fileName, Group * inGroup);
		bool readLightFile(std::string fileName, LightFromFile *inLight);
	private:
		
		template<typename T> 
		void calculateTangentsAndBitangents(T mesh);

		
	};

}