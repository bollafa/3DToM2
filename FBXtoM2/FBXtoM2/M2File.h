#pragma once
#include "M2Types.h"
#include <map>
#include <deque>
/*
	Make this API as ABSTRACT as POSSIBLE PLEASE.

	Yay! We ended up with a monolith api! How extendable!
*/
extern  uint32_t* pStartAddy;
class M2File
{
private:
	  // <---- SHIT PRACTICE
	/* DECLARE ALL LOOK-UP TABLES AND ALL [___INSERT_HERE___] */

	// Deques go HERE

	/*std::deque<M2CompBone> mBoneTable; // NOT NEEDED AS WE'VE FINISHED DATA TABLE CLASS
	std::deque<M2Vertex> mVertexTable;*/
	/* We will need pairs', or my mind will break (pair::first,pair::second)*/

	// Lookup tables
	/*std::map<uint32_t, __int16> m_keyBoneLookUp; // uint32_t AKA array index , __int16 what bone does this index point to (bone:array_index)
	std::map<uint32_t, __int16> m_BoneLookUp; // uint32_t AKA array index , __int16 what bone does this index point to (bone:array_index)
	void AddLookUpElement(std::map<uint32_t, __int16> &_inMap,uint32_t nArrayIndex, __int16 nPointToIndex);*/
	
	// Data Table goes here:
	
	struct BoneTable : public DataTable <M2CompBone>
	{
		std::map<uint32_t, __int16> mkeyBoneLookUp;
		std::map<uint32_t, __int16> mBoneLookUp;
		void AddElement(M2CompBone dBone) override 
		{

			if (!((dBone.GetKeyBoneID() >> 31) & 0x1)) // if It's negative  we dont save the value to keybonelookuptable
			{
				// Make a pair with Array[iKeyBone] = The Id Of this bone. ( the id:bone
																			   // is just the place on the array ( it should be minus 1, but alas, the m_keybonalookup table will only be changed here)
				//mkeyBoneLookUp.insert(std::make_pair(dBone.GetKeyBoneID(), mBoneLookUp.size()));
				mkeyBoneLookUp.insert(std::make_pair(dBone.GetKeyBoneID(), mDataTable.size()));
			}

			mDataTable.push_back(dBone);

		}
	};
	struct ColorTable : public DataTable<M2Color>
	{
		DataTable<DataTable<uint32_t>> mColorTimestamps;
		DataTable<DataTable<C3Vector<float>>> mColorValues;

		DataTable<DataTable<uint32_t>> mAlphaTimestamps;
		DataTable<DataTable<fixed16>> mAlphaValues;
		void AddElement(M2Color dColor)
		{
			// Color Loading - BAD PRACTICE ( What if I am loading more tracks with even more data? R/ We're effed up ...
			LoadTableOfData((uint32_t*) ( (__int8*)pStartAddy + dColor.mColor.values.tOffset ), dColor.mColor.values.tSize,mColorValues);
			LoadTableOfData((uint32_t*)((__int8*)pStartAddy + dColor.mColor.timestamps.tOffset), dColor.mColor.timestamps.tSize, mColorTimestamps);

			//Alpha Loading!
			LoadTableOfData((uint32_t*)((__int8*)pStartAddy + dColor.mAlpha.values.tOffset), dColor.mAlpha.values.tSize, mAlphaValues);
			//LoadTableOfData((uint32_t*)((__int8*)pStartAddy + dColor.mAlpha.timestamps.tOffset), dColor.mAlpha.timestamps.tSize, mAlphaTimestamps);

		}
	};
	/*struct VertexTable : private DataTable<M2Vertex>
	{
		// NOT NEEDED AS OF NOW!

	};*/

	// ----------------------- TABLES OF DATA -------------------

	BoneTable mBoneTable;
	DataTable<M2Vertex> mVertexTable;
	DataTable<M2Sequence> mSequenceTable;
	ColorTable mColorTable;
	DataTable<M2Texture> mTextureTable;
	DataTable<M2TextureWeight> mTextureWeightTable;
public:

	// I shall declare boilerplate functions, which already have useful names, return values, parameters and value types.

	// Should they be void?
	 
	void AddBone(C3Vector<float> vPivot, __int32 iKeyBone, __int32 iParentBone, unsigned __int32 flags); // Missing Bone Lookup Table support!
	void AddVertex(C3Vector<float> vPoint, C3Vector<float> vNormal, std::array<C2Vector<float>, 2> vTexture, std::array<__int8, 4> inVertexProp); // Pretty much done.

	// Load from Memory

	void LoadM2FromMemory(void* pStart);
	void LoadSkinFromMemory(void* pStart);
private:
	class SkinFile // *00.skin file.
	{
	private:
		struct SkinHeader
		{
//#if VERSION >= Wrath 
			uint32_t magic;                         // 'SKIN'
//#endif
			M2Array vertices;						
			M2Array indices;
			M2Array bones;
			M2Array submeshes;
			M2Array batches;
			uint32_t boneCountMax;                  // WoW takes this and divides it by the number of bones in each submesh, then stores the biggest one.
													// Maximum number of bones per drawcall for each view. Related to (old) GPU numbers of registers. 
													// Values seen : 256, 64, 53, 21
			/*
				  M2Array<unsigned short> vertices ;
				  M2Array<unsigned short> indices ;
				  M2Array<ubyte4> bones;
				  M2Array<M2SkinSection> submeshes;
				  M2Array<M2Batch> batches;
				  uint32_t boneCountMax; 
			
			*/
/*#if VERSION >= Cata 
			M2Array<M2ShadowBatch> shadow_batches;
#endif*/
		};
		// Vector of a group of 4 bytes ( first, second, third, fourth bone index )
		DataTable<std::array<int8_t, 4>> mVertexesProp;
		DataTable<uint16_t> mVertexIndices;
		DataTable<C3Vector<uint16_t>> mTriangles;

	public:
		
		SkinFile() {}
		~SkinFile(){}
		void AddVertexProp(std::array<__int8, 4> VertexProp);
		void LoadSkinFromMemory(void* pStart);
	};
	struct M2Header
	{
		unsigned __int32 magic;
		unsigned __int32 version;
		M2Array name;
		unsigned __int32 Flags; // Bitfield struct is VERSION dependant, so i'll implement it later!
		// IMPLEMENTATION SEGMENT -1 ---------------------------------------------
		M2Array global_loops;
		M2Array sequences;
		M2Array sequence_lookup;
		/*
			#IF VERSION <= BURNING_CRUSADE

			M2Array playable_animation_lookup;

			#ENDIF
		*/
		// IMPLEMENTATION SEGMENT 0 -------------------------------------------
		M2Array bones;
		M2Array key_bone_lookup;
		M2Array vertices;
/*#if  VERSION <= BC
		M2Array<M2SkinProfile> skin_profiles;
#else*/
		uint32_t num_skin_profiles;                           // Views (LOD) are now in .skins.
//#endif
		/* TODO: MISSING M2TRACK DEFINITIONS MAKES THIS UNABLE TO WORK */ // IMPLEMENTATION SEGMENT 1-----------------------------------
		M2Array colors;                             // Color and alpha animations definitions.
		M2Array textures;
		M2Array texture_weights;            // Transparency of textures.
		// TODO: IMPLEMENT M2TRACK -- END

		
/*#if  VERSION <= BC
		M2Array< ? > unknown;
#endif*/
		// IMPLEMENTATION SEGMENT 2---------------------------
		M2Array texture_transforms;			// TYPE: M2TextureTransform TODO: BECAUSE OF M2TRACK IMPLEMENTATION ( MIGHT ASWELL IMPLEMENT IT NOW ... ! )
		M2Array replacable_texture_lookup;			// TYPE: uint16_t
		M2Array materials;                       // Blending modes / render flags. TYPE M2Material
		M2Array bone_lookup_table;					// TYPE: uint16_t
		M2Array texture_lookup_table;				// TYPE: uint16_t			
		M2Array tex_unit_lookup_table;              // >= Cata : unused			 TYPE: uint16_t
		M2Array transparency_lookup_table;			// TYPE: uint16_t
		M2Array texture_transforms_lookup_table;	// TYPE: uint16_t
		// SEGMENT 3 ------------------------


		CAaBox bounding_box;                                 // min/max( [1].z, 2.0277779f ) - 0.16f seems to be the maximum camera height
		float bounding_sphere_radius;                         // detail doodad draw dist = clamp (bounding_sphere_radius * detailDoodadDensityFade * detailDoodadDist, …)
		CAaBox collision_box;
		float collision_sphere_radius;

		// IMPLEMENTATION SEGMENT 4 ------------------------ TODO!!!!!!!!!!!!!

	};
	SkinFile M2Skin;
	public:
	template< typename Data >
	static void LoadTableOfData(unsigned __int32 *pStart, size_t nElements, DataTable<Data>& );
	
	template<typename Data, bool magic>
	static void LoadTableOfData(unsigned __int32 *pStart, size_t nElements, DataTable<Data>&);

	static void LoadTableOfData(unsigned __int32 *pStart, size_t nElements, DataTable<C3Vector<float>>&);
	
	static void LoadTableOfData(unsigned __int32 *pStart, size_t nElements, DataTable<fixed16>&);

	static void LoadTableOfData(unsigned __int32 *pStart, size_t nElements, DataTable<uint32_t>&);
	
	static void LoadTableOfData(unsigned __int32 *pStart, size_t nElements, DataTable<Quat16>&);
};

template<typename Data>
static inline void M2File::LoadTableOfData(unsigned __int32* pStart, size_t nElements, DataTable<Data>& vDataTable)
{
	for (unsigned int i = 0; i < nElements; i++)
	{
		unsigned __int32* Place = (pStart + i * sizeof(Data::InternalData));
		Data vTemp = (Data)(*((Data::InternalData*)((unsigned __int8*)pStart + (i) * sizeof(Data::InternalData))));
		vDataTable.AddElement(vTemp);
	}
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! EVEN WORSE THAN BAD PRACTICE HOLY-SHIT DELETE THIS CRAP AFTER PROPER CRAP TESTING !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
template<typename Data,bool magic>
static inline void M2File::LoadTableOfData(unsigned __int32* pStart, size_t nElements, DataTable<Data>& vDataTable)
{
	for (unsigned int i = 0; i < nElements; i++)
	{
//		unsigned __int32* Place = (pStart + i * sizeof(Data::InternalData));
		Data vTemp = (Data)(*((Data*)((unsigned __int8*)pStart + (i) * sizeof(Data))));
		vDataTable.AddElement(vTemp);
	}
}
/*																	END																								*/
inline void M2File::LoadTableOfData(unsigned __int32 * pStart, size_t nElements, DataTable<C3Vector<float>>& vDataTable)
{
	for (unsigned int i = 0; i < nElements; i++)
	{
		unsigned __int32* Place = (pStart + i * sizeof(C3Vector<float>));
		C3Vector<float> vTemp = (C3Vector<float>)(*((C3Vector<float>*)((unsigned __int8*)pStart + (i) * sizeof(C3Vector<float>))));
		vDataTable.AddElement(vTemp);
	}
}

inline void M2File::LoadTableOfData(unsigned __int32 * pStart, size_t nElements, DataTable<fixed16>& vDataTable)
{
	for (unsigned int i = 0; i < nElements; i++)
	{
		unsigned __int32* Place = (pStart + i * sizeof(C3Vector<float>));
		fixed16 vTemp = (fixed16)(*((fixed16*)((unsigned __int8*)pStart + (i) * sizeof(fixed16))));
		vDataTable.AddElement(vTemp);
	}
}

inline void M2File::LoadTableOfData(unsigned __int32 * pStart, size_t nElements, DataTable<uint32_t>& vDataTable)
{
	for (unsigned int i = 0; i < nElements; i++)
	{
		unsigned __int32* Place = (pStart + i * sizeof(C3Vector<float>));
		uint32_t vTemp = (uint32_t)(*((uint32_t*)((unsigned __int8*)pStart + (i) * sizeof(uint32_t))));
		vDataTable.AddElement(vTemp);
	}
}

inline void M2File::LoadTableOfData(unsigned __int32 * pStart, size_t nElements, DataTable<Quat16>& vDataTable)
{
	for (unsigned int i = 0; i < nElements; i++)
	{
		unsigned __int32* Place = (pStart + i * sizeof(C3Vector<float>));
		Quat16 vTemp = (Quat16)(*((Quat16*)((unsigned __int8*)pStart + (i) * sizeof(Quat16))));
		vDataTable.AddElement(vTemp);
	}
}