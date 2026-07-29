#ifndef _CRY_RANDOM__H__
#define _CRY_RANDOM__H__

#include "LCGRandom.h"
#include "MTPseudoRandom.h"

#ifndef USE_SIMPLE_RANDOM
#define USE_SIMPLE_RANDOM 1
#endif

#if USE_SIMPLE_RANDOM
typedef CLCGRndGen CRndGen;
#else
typedef CMTRand_int32 CRndGen;
#endif

// wrapper structure to provide an SPU local random number generater
// only needs to be set an seed, will then init the rng it is used for the first time
// to prevent seeding an rng which isn't used
struct SpuRandomNumberGeneratorWrapper
{
	// use char buffer to prevent using of an constructor in global spu data
	char rng[sizeof(CRndGen)] _ALIGN(128);
	
	uint32 Generate()
	{
		IF( !bIsInit, false )
			Init();

		return GetRng()->GenerateUint32();
	}
	float GenerateFloat()
	{
		IF( !bIsInit, false )
			Init();

		return GetRng()->GenerateFloat();
	}
	SPU_NO_INLINE void Init()
	{			
		GetRng()->Seed(seed);
		bIsInit = true;
	}
	
	void SetSeed( uint32 s )
	{
		seed = s;
		bIsInit = false;
	}
private:
	CRndGen* GetRng()
	{
		return (CRndGen*)rng;
	}

	bool bIsInit;
	uint32 seed;
};


#if defined(__SPU__)
	SPU_LOCAL SpuRandomNumberGeneratorWrapper gSpuRandomNumberGen;
	void SetSpuRNGSeed( uint32 seed )
	{
		gSpuRandomNumberGen.SetSeed( seed );
	}
#else
	// dummy for non spu compilation
	#define SetSpuRNGSeed( seed )
#endif


#endif
