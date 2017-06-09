#pragma once
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