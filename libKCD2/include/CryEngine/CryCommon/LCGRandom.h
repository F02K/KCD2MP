#ifndef __LCGRandom_h__
#define __LCGRandom_h__

//////////////////////////////////////////////////////////////////////////
//
// A Simple LCG Random Number Generator, that is cheap to seed
//
// NOTE: This should _NOT_ be used for any encryption methods

class CLCGRndGen
{ 
public:
	CLCGRndGen()
	{
		Seed(5489UL);
	}

	CLCGRndGen(uint32 seed)
	{
		Seed(seed);
	}

	// initializes the generator using an unsigned 32-bit number
	void Seed(uint32 seed)
	{
		m_seed = (uint64)seed;
	}

	// generates a random unsigned 32-bit number
	uint32 GenerateUint32()
	{
    	const uint64 newSeed = ((uint64)214013) * m_seed + ((uint64)2531011);
		m_seed = newSeed;
		return (uint32)(newSeed >> 16);
	}

	// generates a random unsigned 32-bit number in the closed interval [0, nRange - 1]
	uint32 GenerateUint32(uint32 nRange)
	{
		return uint32((uint64(GenerateUint32()) * nRange) >> 32);
	}

	// generates a random unsigned 64-bit number
	uint64 GenerateUint64()
	{
		return (((uint64)GenerateUint32()) << 32) | ((uint64)GenerateUint32());
	}

	// generates a random floating number in the closed interval [0.0f, 1.0f]
	float GenerateFloat()
	{
		return (float)GenerateUint32() * (1.0f / 4294967295.0f);
	}

private:
	uint64 m_seed;
};

#endif
