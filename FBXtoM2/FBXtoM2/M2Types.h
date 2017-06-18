#pragma once
#include "M2File.h"
#include <array>
#include <deque>
extern uint32_t* pStartAddy;
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
	__int32 m_iKeyBoneID;
public:

	M2CompBone(C3Vector<float> &vPoint, unsigned __int32 &vFlags) : mPivotPoint(vPoint), mBoneFlags(Flags( vFlags )) { /*EMPTY*/ }
	~M2CompBone() {/*EMPTY AS OF NOW*/} 

	const __int32& GetKeyBoneID() { return m_iKeyBoneID;  }
	const C3Vector<float>& GetPivotPoint() { return mPivotPoint; }
	const unsigned __int32& GetFlags() { return mBoneFlags; }
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
		//std::copy(std::begin(bone_indices), std::end(bone_indices), std::end(BoneTemp.m_aBoneIndices));
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
struct M2Range
{
	unsigned __int32 iLowerBound;
	unsigned __int32 iUpperBound;
};
struct CAaBox
{
	C3Vector<float> min;
	C3Vector<float> max;
};
struct M2Bounds {
	CAaBox extent;
	float radius;
};
// SEQUENCES

class M2Sequence
{
private:
	enum SequenceID : uint16_t {
		A_Stand = 0,
		A_Death,
		A_Spell,
		A_Stop,
		A_Walk,
		A_Run,
		A_Dead,
		A_Rise,
		A_StandWound,
		A_CombatWound,
		A_CombatCritical,
		A_ShuffleLeft,
		A_ShuffleRight,
		A_Walkbackwards,
		A_Stun,
		A_HandsClosed,
		A_AttackUnarmed,
		A_Attack1H,
		A_Attack2H,
		A_Attack2HL,
		A_ParryUnarmed,
		A_Parry1H,
		A_Parry2H,
		A_Parry2HL,
		A_ShieldBlock,
		A_ReadyUnarmed,
		A_Ready1H,
		A_Ready2H,
		A_Ready2HL,
		A_ReadyBow,
		A_Dodge,
		A_SpellPrecast,
		A_SpellCast,
		A_SpellCastArea,
		A_NPCWelcome,
		A_NPCGoodbye,
		A_Block,
		A_JumpStart,
		A_Jump,
		A_JumpEnd,
		A_Fall,
		A_SwimIdle,
		A_Swim,
		A_SwimLeft,
		A_SwimRight,
		A_SwimBackwards,
		A_AttackBow,
		A_FireBow,
		A_ReadyRifle,
		A_AttackRifle,
		A_Loot,
		A_ReadySpellDirected,
		A_ReadySpellOmni,
		A_SpellCastDirected,
		A_SpellCastOmni,
		A_BattleRoar,
		A_ReadyAbility,
		A_Special1H,
		A_Special2H, 
		A_ShieldBash,
		A_EmoteTalk,
		A_EmoteEat,
		A_EmoteWork,
		A_EmoteUseStanding,
		A_EmoteTalkExclamation,
		A_EmoteTalkQuestion,
		A_EmoteBow,
		A_EmoteWave,
		A_EmoteCheer,
		A_EmoteDance,
		A_EmoteLaugh,
		A_EmoteSleep,
		A_EmoteSitGround,
		A_EmoteRude,
		A_EmoteRoar,
		A_EmoteKneel,
		A_EmoteKiss,
		A_EmoteCry,
		A_EmoteChicken,
		A_EmoteBeg,
		A_EmoteApplaud,
		A_EmoteShout,
		A_EmoteFlex,
		A_EmoteShy,
		A_EmotePoint,
		A_Attack1HPierce,
		A_Attack2HLoosePierce,
		A_AttackOff,
		A_AttackOffPierce,
		A_Sheath,
		A_HipSheath,
		A_Mount,
		A_RunRight,
		A_RunLeft,
		A_MountSpecial,
		A_Kick,
		A_SitGroundDown,
		A_SitGround,
		A_SitGroundUp,
		A_SleepDown,
		A_Sleep,
		A_SleepUp,
		A_SitChairLow,
		A_SitChairMed,
		A_SitChairHigh,
		A_LoadBow,
		A_LoadRifle,
		A_AttackThrown,
		A_ReadyThrown,
		A_HoldBow,
		A_HoldRifle,
		A_HoldThrown,
		A_LoadThrown,
		A_EmoteSalute,
		A_KneelStart,
		A_KneelLoop,
		A_KneelEnd,
		A_AttackUnarmedOff,
		A_SpecialUnarmed,
		A_StealthWalk,
		A_StealthStand,
		A_Knockdown,
		A_EatingLoop,
		A_UseStandingLoop,
		A_ChannelCastDirected,
		A_ChannelCastOmni,
		A_Whirlwind,
		A_Birth,
		A_UseStandingStart,
		A_UseStandingEnd,
		A_CreatureSpecial,
		A_Drown,
		A_Drowned,
		A_FishingCast,
		A_FishingLoop,
		A_Fly,
		A_EmoteWorkNoSheathe,
		A_EmoteStunNoSheathe,
		A_EmoteUseStandingNoSheathe,
		A_SpellSleepDown,
		A_SpellKneelStart,
		A_SpellKneelLoop,
		A_SpellKneelEnd,
		A_Sprint,
		A_InFlight,
		A_Spawn,
		A_Close,
		A_Closed,
		A_Open,
		A_Opened,
		A_Destroy,
		A_Destroyed,
		A_Rebuild,
		A_Custom0,
		A_Custom1,
		A_Custom2,
		A_Custom3,
		A_Despawn,
		A_Hold,
		A_Decay,
		A_BowPull,
		A_BowRelease,
		A_ShipStart,
		A_ShipMoving,
		A_ShipStop,
		A_GroupArrow,
		A_Arrow,
		A_CorpseArrow,
		A_GuideArrow,
		A_Sway,
		A_DruidCatPounce,
		A_DruidCatRip,
		A_DruidCatRake,
		A_DruidCatRavage,
		A_DruidCatClaw,
		A_DruidCatCower,
		A_DruidBearSwipe,
		A_DruidBearBite,
		A_DruidBearMaul,
		A_DruidBearBash,
		A_DragonTail,
		A_DragonStomp,
		A_DragonSpit,
		A_DragonSpitHover,
		A_DragonSpitFly,
		A_EmoteYes,
		A_EmoteNo,
		A_JumpLandRun,
		A_LootHold,
		A_LootUp,
		A_StandHigh,
		A_Impact,
		A_LiftOff,
		A_Hover,
		A_SuccubusEntice,
		A_EmoteTrain,
		A_EmoteDead,
		A_EmoteDanceOnce,
		A_Deflect,
		A_EmoteEatNoSheathe,
		A_Land,
		A_Submerge,
		A_Submerged,
		A_Cannibalize,
		A_ArrowBirth,
		A_GroupArrowBirth,
		A_CorpseArrowBirth,
		A_GuideArrowBirth,
		A_EmoteTalkNoSheathe,
		A_EmotePointNoSheathe,
		A_EmoteSaluteNoSheathe,
		A_EmoteDanceSpecial,
		A_Mutilate,
		A_CustomSpell01,
		A_CustomSpell02,
		A_CustomSpell03,
		A_CustomSpell04,
		A_CustomSpell05,
		A_CustomSpell06,
		A_CustomSpell07,
		A_CustomSpell08,
		A_CustomSpell09,
		A_CustomSpell10,
		A_StealthRun

	};
	SequenceID mId;
	uint16_t mVariationIndex;
	uint32_t mDuration;
	float mMoveSpeed;
	std::pair<unsigned int, unsigned int> mReplay;
	enum Flags : uint32_t
	{
		Set0x80 = 0x1,
		Unknown_01 = 0x2,
		Unknown_02 = 0x4,
		Unknown_03 = 0x8,
		LowPriority = 0x10,
		Looped = 0x20,
		HasNext = 0x40,
		Blended = 0x80,
		SequenceInModel = 0x100,

	};
	Flags mFlags;
	int16_t mFrequency;
	uint32_t mBlendTime;
	M2Bounds mBounds;
	int16_t mVariationNext;
	uint16_t mAliasNext;
public:

	struct  InternalData
	{
		uint16_t id;                   // Animation id in AnimationData.dbc
		uint16_t variationIndex;       // Sub-animation id: Which number in a row of animations this one is.
/*#if VERSION <= VERSION_CRUSADE
		uint32_t start_timestamp;
		uint32_t end_timestamp;
#else*/
		uint32_t duration;             // The length (timestamps) of the animation. I believe this actually the length of the animation in milliseconds.
//#endif
		float movespeed;               // This is the speed the character moves with in this animation.
		uint32_t flags;                // See below.
		int16_t frequency;             // This is used to determine how often the animation is played. For all animations of the same type, this adds up to 0x7FFF (32767).
		uint16_t _padding;
		M2Range replay;                // May both be 0 to not repeat. Client will pick a random number of repetitions within bounds if given.
		uint32_t blendtime;            // The client blends (lerp) animation states between animations where the end and start values differ. This specifies how long that blending takes. Values: 0, 50, 100, 150, 200, 250, 300, 350, 500.
		M2Bounds bounds;
		int16_t variationNext;         // id of the following animation of this AnimationID, points to an Index or is -1 if none.
		uint16_t aliasNext;			   // id in the list of animations. Used to find actual animation if this sequence is an alias (flags & 0x40)

		operator M2Sequence()
		{
			
			
			return M2Sequence(id, variationIndex, duration,movespeed, Flags(flags),frequency, std::make_pair(replay.iLowerBound, replay.iUpperBound), blendtime,bounds, variationNext, aliasNext);
		}

	};
	// TODO
	//M2Sequence(/* I DON'T KNOW YET */)   {}
	M2Sequence( const uint16_t& _id,const uint16_t& _variationIndex,const uint32_t& _duration,float _movespeed,const Flags& _Flags, \
		int16_t _frequency,const std::pair<unsigned int, unsigned int>& _Replay,uint32_t _BlendTime,M2Bounds _bounds,int16_t _variationNext,uint16_t _aliasNext) : mId(SequenceID(_id)),mVariationIndex(_variationIndex), \
	mDuration(_duration),mMoveSpeed(_movespeed), mFlags(_Flags),mFrequency(_frequency),mReplay(_Replay),mBlendTime(_BlendTime),mBounds(_bounds),mVariationNext(_variationNext),mAliasNext(_aliasNext) {}
	~M2Sequence() {}

};



template< typename Data >
class DataTable // This class should be inside M2File, AND PRIVATE
{
protected:
	std::deque<Data> mDataTable;
public:
	struct InternalData // BAD PRACTICE - BEGIN 
	{
		unsigned __int32 tSize;
		unsigned __int32 tOffset;
		operator DataTable<Data>()
		{
			DataTable<Data> vTemp;
			M2File::LoadTableOfData((uint32_t*)( (__int8*)pStartAddy + tOffset ) , tSize, vTemp);
			return vTemp;
		}
	};
	virtual void AddElement(Data _In)
	{
		mDataTable.push_back(_In);
	};

};

struct M2TrackBase
{
	uint16_t interpolation_type;
	uint16_t global_sequence;
	/*#if  VERSION < Wrath
	M2Array<pair<uint32_t>> interpolation_ranges;   // no longer required >= Wrath , as implicit by minimum and maximum timestamp per sequence.
	M2Array<uint32_t> timestamps;
	#else*/
	DataTable<DataTable<uint32_t>> timestamps; // M2Array<M2Array<uint32_t>>
											   //#endif
};
template<typename T>
struct M2Track : M2TrackBase
{
/*#if  < Wrath 
	M2Array<T> values;
#else*/
	DataTable<DataTable<T>> values; // M2Array<M2Array<T>>
	M2Track<T>& operator=(M2Track<T>& other)
	{
		/*M2Track_cint<T> temp;
		temp.global_sequence = other.global_sequence;
		temp.interpolation_type = other.interpolation_type;
		temp.timestamps = other.timestamps;
		temp.values = other.values;*/
		global_sequence = other.global_sequence;
		interpolation_type = other.interpolation_type;
		timestamps = other.timestamps;
		values = other.values;
		return *this;
	}
//#endif
};
struct M2TrackBase_cint
{
	uint16_t interpolation_type;
	uint16_t global_sequence;
	/*#if  VERSION < Wrath
	M2Array<pair<uint32_t>> interpolation_ranges;   // no longer required >= Wrath , as implicit by minimum and maximum timestamp per sequence.
	M2Array<uint32_t> timestamps;
	#else*/
	M2Array timestamps; // M2Array<M2Array<uint32_t>>
};
template <typename T>
struct M2Track_cint : M2TrackBase_cint
{
	M2Array values;

	M2Track_cint<T>& operator=(M2Track_cint<T>& other)
	{
		/*M2Track_cint<T> temp;
		temp.global_sequence = other.global_sequence;
		temp.interpolation_type = other.interpolation_type;
		temp.timestamps = other.timestamps;
		temp.values = other.values;*/
		global_sequence = other.global_sequence;
		interpolation_type = other.interpolation_type;
		timestamps = other.timestamps;
		values = other.values;
		return *this;
	}
	operator M2Track<T>()
	{
		M2Track<T> temp;
		temp.global_sequence = global_sequence;
		temp.interpolation_type = interpolation_type;
		M2File::LoadTableOfData((uint32_t*)((__int8*)pStartAddy + values.tOffset), values.tSize, temp.values);
		M2File::LoadTableOfData((uint32_t*)((__int8*)pStartAddy + timestamps.tOffset), timestamps.tSize, temp.timestamps);
		return temp;
	}
};

using fixed16 = short; // divide by 0x7fff to get float value
class M2Color
{
public:
	M2Track_cint<C3Vector<float>> mColor; // vertex colors in rgb order
	M2Track_cint<fixed16> mAlpha; // 0 - transparent, 0x7FFF - opaque. Normaly NonInterp
	
//public:
	struct InternalData
	{
		M2Track_cint<C3Vector<float>> color; // vertex colors in rgb order
		M2Track_cint<fixed16> alpha;
		operator M2Color()
		{
			M2Color temp;
			temp.mColor = color;
			temp.mAlpha = alpha;
			return temp;
		}
	};
	M2Color() {}
	~M2Color() {}
};
class M2Texture
{
private:
	
	uint32_t mType;          // see below
	uint32_t mFlags;         // see below
	
public:
	char* mFilename; // Make private later
	struct InternalData
	{
		uint32_t type;          // see below
		uint32_t flags;         // see below
		M2Array filename; // for non-hardcoded textures (type != 0), this still points to a zero-sized string

		operator M2Texture()
		{
			
			return M2Texture((char*)((__int8*)pStartAddy + filename.tOffset), type, flags);
		}
	};
	M2Texture(char* _Filename, uint32_t _type, uint32_t _flags) : mFilename(_Filename), mType(_type), mFlags(_flags) {}
};

class M2TextureWeight
{
private:
	
public:
	M2Track<fixed16> mWeight;
	struct InternalData
	{
		M2Track_cint<fixed16> weight;
		operator M2TextureWeight()
		{
			M2TextureWeight temp;
			temp.mWeight = (M2Track<fixed16>)weight;
			return temp;
		}
	};
	M2TextureWeight() {}
};
