#include "M2File.h"
#include "M2Types.h"
#include <iostream>
uint32_t* pStartAddy;
/*void M2File::AddLookUpElement(std::map<uint32_t, __int16>& _inMap, uint32_t nArrayIndex, __int16 nPointToIndex)
{
	_inMap.insert(std::make_pair(nArrayIndex, nPointToIndex));// Make a pair with Array[nArrayIndex] = The Id Of this bone. ( the id:bone
		// is just the place on the array ( it should be minus 1, but alas, the m_keybonalookup table will only be changed here)
}
*/
void M2File::AddBone(C3Vector<float> vPivot, __int32 iKeyBone, __int32 iParentBone, unsigned __int32 flags)
{
	/*if (! ((iKeyBone >> 31) & 0x1) ) // if It's negative  we dont save the value to keybonelookuptable
	{
		AddLookUpElement(m_keyBoneLookUp, iKeyBone,mBoneTable.size());// Make a pair with Array[iKeyBone] = The Id Of this bone. ( the id:bone
		// is just the place on the array ( it should be minus 1, but alas, the m_keybonalookup table will only be changed here)
	}*/
	
	//mBoneTable.push_back(M2CompBone(vPivot, flags));
	/* TODO : BONE LOOK UP TABLE*/
	// Bone look up table logic is a little bit harder tho, it looks like every SUBMESH in SKIN HOLDS <=> Submesh[ID]:RootBone == ( BoneLookUpTable[ID] = RootBone )
	// HELP: SUBMESHES[5] AND [6] DONT HOLD FOR THAT STATEMENT. WHY!!??????!?!?!?!??!

	// FINAL THOUGHT: ADD ALL BONES IN THE LOOKUP ARRAY [INDEX] = BONE_INDEX
	// JUST PUT THE ROOT BONE Bones { Xn ... Xy } . I believe WoW will find RootBone in the array of bones in look up table Bones { Xn .. ROOT BONE .. Xy } 
	// That's why there are always the same number of bones.
	// Every LOD has a specific number of bones ?>? PERHAPS The lookup table HAS MORE REPEATED bones, AS TO LESS BONES LOADED, ERGO, LESS BONES "USED"
	// Possibly meaning: closer = more bones. Far away: less bones
	// RootBoneID: Tell THE ACTUAL ID FROM THE GLOBAL LIST
	// Lookup table for bones that transform geometry. 
	// BF_TRANSFORMED FLAG
}
// Add vertex to global vertex table, for further use.
void M2File::AddVertex(C3Vector<float> vPoint, C3Vector<float> vNormal, std::array<C2Vector<float>, 2> vTexture, std::array<__int8,4> inVertexProp)
{
	// M2 File. Push to vertex table:
	//mVertexTable.push_back( M2Vertex(vPoint, vNormal, vTexture) );
	// TODO : SKIN FILE. VERTEXPROP == EDIT : DONE!

	//M2Skin.AddVertexProp(inVertexProp); // Vertex properties. Bone Indices. 
}
#define Cast(x,y) static_cast<x>(y)
#define _M2Header(x) Cast(M2Header*,x)
#define _M2Skin(x)	Cast(SkinHeader*,x)
void M2File::LoadM2FromMemory(void * pStart)
{
	pStartAddy = (uint32_t*)pStart;
	if (_M2Header(pStart)->magic != _byteswap_ulong('MD20'))
	{
		std::cout << "\t<SIGNATURE_NOT_CORRECT>\n" << _M2Header(pStart)->magic << std::endl;
		return;
	}
	std::cout << "\t<SIGNATURE:" << (char(&)[4])_M2Header(pStart)->magic << ">" << std::endl;

	// Let's load the tables of data
	LoadTableOfData( ( (unsigned __int32*)(((__int8*)pStart) + static_cast<M2Header*>(pStart)->vertices.tOffset)), _M2Header(pStart)->vertices.tSize, mVertexTable);
	//Let's try with sequences:
	LoadTableOfData(((unsigned __int32*)(((__int8*)pStart) + static_cast<M2Header*>(pStart)->sequences.tOffset)), _M2Header(pStart)->sequences.tSize, mSequenceTable);

	std::cout << "\t<DEBUG_DATA::SPHERE_RADIUS(FLOAT):" << _M2Header(pStart)->collision_sphere_radius << ">" << std::endl;

	// Now with colors:

	LoadTableOfData(((unsigned __int32*)(((__int8*)pStart) + static_cast<M2Header*>(pStart)->colors.tOffset)), _M2Header(pStart)->colors.tSize, mColorTable);
	
	// Now textures!
	LoadTableOfData(((unsigned __int32*)(((__int8*)pStart) + static_cast<M2Header*>(pStart)->textures.tOffset)), _M2Header(pStart)->textures.tSize, mTextureTable);

	// Now texture weigths!
	LoadTableOfData(((unsigned __int32*)(((__int8*)pStart) + static_cast<M2Header*>(pStart)->texture_weights.tOffset)), _M2Header(pStart)->texture_weights.tSize, mTextureWeightTable);

	// Bone Table

	LoadTableOfData(((unsigned __int32*)(((__int8*)pStart) + static_cast<M2Header*>(pStart)->bones.tOffset)), _M2Header(pStart)->bones.tSize, mBoneTable);

	
}

void M2File::LoadSkinFromMemory(void * pStart)
{
	M2Skin.LoadSkinFromMemory(pStart);
}

void M2File::SkinFile::AddVertexProp(std::array<__int8, 4> VertexProp)
{
	//mVertexesProp.push_back(VertexProp);
}

void M2File::SkinFile::LoadSkinFromMemory(void * pStart)
{
	// Loading a skin file from a memory address! Fun! (Lol...)
	if (_M2Skin(pStart)->magic != _byteswap_ulong('SKIN'))
	{
		std::cout << "\t<SIGNATURE_NOT_CORRECT>\n" << _M2Header(pStart)->magic << std::endl;
		return;
	}
	std::cout << "<SKIN_FILE> CORRECT_MAGIC </SKIN_FILE>" << std::endl;
	// Let's load the vertex indices

	LoadTableOfData<uint16_t, true>(((unsigned __int32*)(((__int8*)pStart) + static_cast<SkinHeader*>(pStart)->vertices.tOffset)), _M2Skin(pStart)->vertices.tSize, SkinFile::mVertexIndices);

	// Let's load the triangle sht

	LoadTableOfData< C3Vector<uint16_t>, true >(((unsigned __int32*)(((__int8*)pStart) + static_cast<SkinHeader*>(pStart)->indices.tOffset)), _M2Skin(pStart)->indices.tSize/3, SkinFile::mTriangles);

	// Let's load vertex properties

	LoadTableOfData<std::array<int8_t, 4>, true>(((unsigned __int32*)(((__int8*)pStart) + static_cast<SkinHeader*>(pStart)->bones.tOffset)), _M2Skin(pStart)->bones.tSize, SkinFile::mVertexesProp);

	// Let's load submeshes

	LoadTableOfData(((unsigned __int32*)(((__int8*)pStart) + static_cast<SkinHeader*>(pStart)->submeshes.tOffset)), _M2Skin(pStart)->submeshes.tSize, SkinFile::mSubmeshes);
}
