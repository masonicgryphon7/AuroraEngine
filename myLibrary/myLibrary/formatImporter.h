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

		MeshFromFile readMeshFile(std::string fileName);
		CameraFromFile readCameraFile(std::string fileName);
		MaterialFromFile readMaterialFile(std::string fileName);
		LightFromFile readLightFile(std::string fileName);
		bool readMorphAnimationFile(std::string fileName);
		SkeletonFromFile readSkeletonFile(std::string fileName);
		bool readAnimationFile(std::string fileName);
		bool readGroupFile(std::string fileName);

	private:
		int getNrOfVerticesFromFile(std::ifstream& file);
		void calculateTangentsAndBitangents(MeshFromFile mesh);
	};


	//			//Mesh functions
	////-----------------------------------------//
	//
	//int getNrOfVerticesFromFile(std::ifstream& file)
	//{
	//	//read number of vertices in mesh from custom file
	//	int nrOfVerticesInFile = 0;
	//
	//
	//	return nrOfVerticesInFile;
	//}
	//
	////Vertex getVerticesFromFile(std::ifstream& file, int nrOfVertices)
	////{
	////	//get all the vertex data from the file
	////	Vertex verticesInFile;
	////
	////	return verticesInFile;
	////}
	//
	//int getMeshIdFromFile(std::ifstream& file)
	//{
	//	//get meshID from the file
	//	int meshIdInFile = 0;
	//
	//	return meshIdInFile;
	//}
	//
	//int getMaterialIdFromFile(std::ifstream& file)
	//{
	//	//get materialID from the file
	//	int materialIdInFile = 0;
	//
	//	return materialIdInFile;
	//}
	//
	//			//Camera functions
	////-----------------------------------------//
	//
	//int getNrOfCamerasFromFile(std::ifstream& file)
	//{
	//	int nrOfCamerasInFile = 0;
	//
	//	return nrOfCamerasInFile;
	//}
	//
	//			//Material functions
	////----------------------------------------//
	//
	//int getNrOfMaterialsFromFile(std::ifstream& file)
	//{
	//	int nrOfMaterialsInFile = 0;
	//
	//	return nrOfMaterialsInFile;
	//}
	//
	//			//Light functions
	////---------------------------------------//
	//
	//int getNrOfLightsFromFile(std::ifstream& file)
	//{
	//	int nrOfLightsInFile = 0;
	//
	//	return nrOfLightsInFile;
	//}
}