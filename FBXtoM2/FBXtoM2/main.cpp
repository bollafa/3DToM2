#include <iostream>
#include <Windows.h>
#include "M2File.h"

/* TESTING ASSIMP - DELETE LATER */
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
/* END */
void main()
{
	
		std::cout << "FBX to M2 - PRE-BETA:ALPHA PREALPHA ALPHA \nYou've been warned! I dont guarantee anything!" << std::endl;
		
		/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! DELETE START !!!!!!!!!!!!!!!!!!!!*/
		/*
		*	Mapping  M2 file to memory - so to cast the M2 Template to it later -
		*/
		HANDLE hMappedFile;
		if ((hMappedFile = CreateFile(L"M2/Ragnaros.m2", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) // Check for errors
		{
			std::cout << "Error while creating file!" << std::endl;
			return;
		}

		/*
		*	Create a file mapping
		*/
		HANDLE hFileMapped;
		if ((hFileMapped = CreateFileMapping(hMappedFile, NULL, PAGE_READONLY, NULL, NULL, L"M2 File")) == NULL) // Check for errors
		{
			std::cout << "Error while mapping file to memory! :: -> " << GetLastError() << " hFileMapped: " << hFileMapped << std::endl;
			return;
		}

		LPVOID pStartAddy;
		if (!(pStartAddy = MapViewOfFile(hFileMapped, FILE_MAP_READ, 0, 0, NULL)))
		{
			std::cout << "Error while MAPPING VIEW" << std::endl;
			return;
		}
		//-----------------------------    MAPPING SKIN    --------------------------------------


		HANDLE hSkinMappedFile;
		if ((hSkinMappedFile = CreateFile(L"M2/Ragnaros00.skin", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) // Check for errors
		{
			std::cout << "Error while creating file!" << std::endl;
			return;
		}

		/*
		*	Create a file mapping
		*/
		HANDLE hSkinFileMapped;
		if ((hSkinFileMapped = CreateFileMapping(hSkinMappedFile, NULL, PAGE_READONLY, NULL, NULL, L"Skin File")) == NULL) // Check for errors
		{
			std::cout << "Error while mapping file to memory! :: -> " << GetLastError() << " hFileMapped: " << hFileMapped << std::endl;
			return;
		}

		LPVOID pSkinStartAddy;
		if (!(pSkinStartAddy = MapViewOfFile(hSkinFileMapped, FILE_MAP_READ, 0, 0, NULL)))
		{
			std::cout << "Error while MAPPING VIEW" << std::endl;
			return;
		}

		// ----------------- ------------------- ---------------- DELETE END ------------------- ---------------- ------------

		M2File Ragnaros;
		Ragnaros.LoadM2FromMemory(pStartAddy);
		Ragnaros.LoadSkinFromMemory(pSkinStartAddy);
		

		// DELETE START - TESTING ASSIMP 

		Assimp::Importer importer;
		const aiScene* ai_Scene = importer.ReadFile("Crate/Crate1", aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

		if (!ai_Scene || ai_Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !ai_Scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}



		// Loading vertices, i know, it's shit. But who cares.




		// DELETE END
}