#pragma once
#include "M2Types.h"
#include <map>
#include <deque>
/*
	Make this API as ABSTRACT as POSSIBLE PLEASE.
*/
class M2File
{
private:
	/* DECLARE ALL LOOK-UP TABLES AND ALL [____INSERT_HERE___] */

	// Deques go HERE

	std::deque<M2CompBone> mBoneTable;
	std::deque<M2Vertex> mVertexTable;
	/* We will need pairs', or my mind will break (pair::first,pair::second)*/

	// Lookup tables
	std::map<uint32_t, __int16> m_keyBoneLookUp; // uint32_t AKA array index , __int16 what bone does this index point to (bone:array_index)
	std::map<uint32_t, __int16> m_BoneLookUp; // uint32_t AKA array index , __int16 what bone does this index point to (bone:array_index)
	void AddLookUpElement(std::map<uint32_t, __int16> &_inMap,uint32_t nArrayIndex, __int16 nPointToIndex);
	template< typename Data >
	void LoadTableOfData(unsigned __int32 *pStart, size_t nElements, std::deque<Data> &vFinalArray);
public:

	// I shall declare boilerplate functions, which already have useful names, return values, parameters and value types.

	// Should they be void?
	 
	void AddBone(C3Vector<float> vPivot, __int32 iKeyBone, __int32 iParentBone, unsigned __int32 flags); // Missing Bone Lookup Table support!
	void AddVertex(C3Vector<float> vPoint, C3Vector<float> vNormal, std::array<C2Vector<float>, 2> vTexture, std::array<__int8, 4> inVertexProp); // Pretty much done.

	// Load from Memory

	void LoadM2FromMemory(void* pStart);
private:
	class SkinFile // *00.skin file.
	{
	private:
		// Vector of a group of 4 bytes ( first, second, third, fourth bone index )
		std::deque<std::array<__int8, 4>> mVertexesProp;


	public:

		SkinFile() {}
		~SkinFile(){}
		void AddVertexProp(std::array<__int8, 4> VertexProp);

	};
	struct M2Header
	{
		unsigned __int32 magic;
		unsigned __int32 version;
		M2Array name;
		unsigned __int32 Flags; // Bitfield struct is VERSION dependant, so i'll implement it later!
		M2Array global_loops;
		M2Array sequences;
		M2Array sequence_lookup;
		/*
			#IF VERSION <= BURNING_CRUSADE

			M2Array playable_animation_lookup;

			#ENDIF
		*/
		M2Array bones;
		M2Array key_bone_lookup;
		M2Array vertices;
	};
	SkinFile M2Skin;
};

template<typename Data>
inline void M2File::LoadTableOfData(unsigned __int32* pStart, size_t nElements, std::deque<Data>& vFinalArray)
{
	for (unsigned int i = 0; i < nElements; i++)
	{
		unsigned __int32* Place = (pStart + i * sizeof(Data::InternalData));
		Data vTemp = (Data)(*((Data::InternalData*)((unsigned __int8*)pStart + (i) * sizeof(Data::InternalData))));
		vFinalArray.push_back(vTemp);
	}
}
