#pragma once
#include <array>
/*
	M2Types.h
	Purpose: Define common types found in M2's structure

	-- THE UNLICENSE LICENSE -- 
*/	

/* COMMON SPACE-TYPES (VECTORS,QUADS,...) */
template <typename T>
struct C3Vector
{
	T x;
	T y;	// lets hope floats are IIEE-756 compliant (SIGN-MANTISSA-POWER)
	T z;
};

template <typename T>
struct C2Vector
{
	T x;
	T y;	
};

// TODO : QUADS



// END : TODO

/* !!! M2'S SPECIAL SNOWFLAKES GO HERE !!! */

class M2CompBone 
{
private:
	enum Flags : unsigned __int32 {
	
	
	};

	Flags mBoneFlags;
	C3Vector<float> mPivotPoint;

public:

	M2CompBone(C3Vector<float> &vPoint, unsigned __int32 &vFlags) : mPivotPoint(vPoint), mBoneFlags(Flags( vFlags )) { /*EMPTY*/ }
	~M2CompBone() {/*EMPTY AS OF NOW*/} 

};

/* COLLAPSED CLASS */
/*
	class BoneWeight
		Collapses BoneWeights into a one fancy class
	end

*/
struct M2Array
{
	unsigned __int32 tSize;
	unsigned __int32 tOffset;
	
};
struct BoneWeight
{
	std::array<__int8, 4> m_aBoneIndices;
	std::array<__int8, 4> m_aBoneWeights;
	BoneWeight() : m_aBoneIndices({ 0,0,0,0 }), m_aBoneWeights({-1,0,0,0}) {} // Default should be 0,0,0,0 or with -1 ?
	BoneWeight(std::array<__int8, 4> &vBoneIndices, std::array<__int8, 4> &vBoneWeights) : m_aBoneIndices(vBoneIndices),m_aBoneWeights(vBoneWeights) {}
	
};
// M2Vertex class, it has a normal texture, the actual 3d point and the 3d point of its normal
class M2Vertex
{
private:

	C3Vector<float> m_vPoint;
	C3Vector<float> m_vNormal;
	std::array<C2Vector<float>,2> m_uvTexture;
	BoneWeight m_BoneWeight;
public: // Should be privated later?
	struct InternalData
	{
		C3Vector<float> pos;
		 __int8 bone_weights[4];
		 __int8 bone_indices[4];
		C3Vector<float> normal;
		C2Vector<float> tex_coords[2];  // two textures, depending on shader used

		operator M2Vertex() {

			BoneWeight BoneTemp;
//			std::copy(std::begin(bone_indices), std::end(bone_indices), std::end(BoneTemp.m_aBoneIndices));
			//std::copy(std::begin(bone_weights), std::end(bone_weights), std::end(BoneTemp.m_aBoneWeights));
			std::copy_n(bone_indices, 4, std::begin(BoneTemp.m_aBoneIndices));
			std::copy_n(bone_weights, 4, std::begin(BoneTemp.m_aBoneWeights));
			std::array<C2Vector<float>, 2> tempTextures;
			//std::copy(std::begin(tex_coords), std::end(tex_coords), std::end(tempTextures));
			std::copy_n(tex_coords, 2, std::begin(tempTextures));
			return M2Vertex(pos, normal, tempTextures, BoneTemp);

		}
	};
	/* !!! CONVERTER !!!*/
	/*M2Vertex operator=(InternalData RawVertex)
	{
		// Converting C array to std::array
		BoneWeight BoneTemp;
		std::copy(std::begin(RawVertex.bone_indices), std::end(RawVertex.bone_indices), std::end(BoneTemp.m_aBoneIndices));
		std::copy(std::begin(RawVertex.bone_weights), std::end(RawVertex.bone_weights), std::end(BoneTemp.m_aBoneWeights));

		std::array<C2Vector<float>, 2> tempTextures;
		std::copy(std::begin(RawVertex.tex_coords), std::end(RawVertex.tex_coords), std::end(tempTextures));
		return M2Vertex(RawVertex.pos, RawVertex.normal, tempTextures, BoneTemp);
	}
	// Converts from actual struct representation of data and API representation of data. Now you can actually cast directly!
	InternalData operator=(M2Vertex DataM2)
	{
		InternalData Temp;
		
		//std::copy(std::begin(DataM2.m_BoneWeight.m_aBoneWeights), std::end(DataM2.m_BoneWeight.m_aBoneWeights), std::end(Temp.bone_weights));
		//std::copy(std::begin(DataM2.m_BoneWeight.m_aBoneIndices), std::end(DataM2.m_BoneWeight.m_aBoneIndices), std::end(Temp.bone_indices)); std::back_inserter
		std::copy_n(std::begin(DataM2.m_BoneWeight.m_aBoneIndices), 4, Temp.bone_indices);
		std::copy_n(std::begin(DataM2.m_BoneWeight.m_aBoneWeights), 4, Temp.bone_weights);
		Temp.pos = m_vPoint;
		Temp.normal = m_vNormal;
		//std::copy(std::begin(DataM2.m_uvTexture), std::end(DataM2.m_uvTexture), std::end(Temp.tex_coords));
		std::copy_n(std::begin(DataM2.m_uvTexture), 2, Temp.tex_coords);
		return Temp;
	}*/

	operator InternalData() 
	{


	}

	/* !!! CONVERTER:END !!!*/
//public:

	M2Vertex(C3Vector<float> &vPoint, C3Vector<float> &vNormal, std::array<C2Vector<float>, 2> &uvTexture) : m_vPoint(vPoint), m_vNormal(vNormal), m_uvTexture(uvTexture), m_BoneWeight() {}
	M2Vertex(C3Vector<float> &vPoint, C3Vector<float> &vNormal, std::array<C2Vector<float>, 2> &uvTexture,BoneWeight inBoneWeight) : m_vPoint(vPoint), m_vNormal(vNormal), m_uvTexture(uvTexture), m_BoneWeight(inBoneWeight) {}
	M2Vertex() {}
	~M2Vertex() {}



};