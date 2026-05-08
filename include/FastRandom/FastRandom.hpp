// Nate Newhart - Lightweight hash-based random library
// All in one header file for ease of access

#pragma once

#include <cstdint> // For fixed width integer types

namespace FastRandom
{
	// Using Splittable64 Hash (64-bit)
	double   i64Double(uint64_t seed); // Return double between 0 and 1 using Splittable64 hash
	float    i64Float (uint64_t seed); // Return float  between 0 and 1 using Splittable64 hash
	uint64_t uInt64   (uint64_t seed); // Random uint64_t using Splittable64 hash

	// Using PCG Hash (32-bit)
	double   i32Double(uint32_t seed); // Return double between 0 and 1 using PCG hash NOTE: has less precision than its 64 bit counterpart
	float    i32Float (uint32_t seed); // Return float  between 0 and 1 using PCG hash
	uint32_t uInt32   (uint32_t seed); // Random uint32_t using PCG hash

	uint64_t CompileTimeSeed(); // Generate a compile time seed using the __TIME__ macro

	double   ScaleDouble(double value, double min, double max); // value must be in [0, 1]
	float    ScaleFloat (float value, float min, float max);    // value must be in [0, 1]
	uint64_t ScaleUInt  (uint64_t value, uint64_t min, uint64_t max);
	uint32_t ScaleUInt  (uint32_t value, uint32_t min, uint32_t max); 
	int      ScaleInt   (uint32_t value, int min, int max); // Given a random uint32_t value, scale it to an int in the range [min, max]

	namespace Shift
	{
		double   ToDouble(uint64_t hash); // Shifts 64 bits into [0, 1)
		float    ToFloat (uint64_t hash); // Shifts 64 bits into [0, 1)
		float    ToFloat (uint32_t hash); // Shifts 32 bits into [0, 1)
		uint32_t ToUInt32(uint64_t hash); // Shifts 64 bits into 32 bits
	}

	namespace Hash
	{
		uint32_t PCGi32(uint32_t seed);
		uint64_t Splittablei64(uint64_t seed);
	}
}

// -------------------------------- Inline Function Definitions --------------------------------

inline double FastRandom::i64Double(uint64_t seed)
{
	return Shift::ToDouble(Hash::Splittablei64(seed));
}

inline float FastRandom::i64Float(uint64_t seed)
{
	return Shift::ToFloat(Hash::Splittablei64(seed));
}

inline uint64_t FastRandom::uInt64(uint64_t seed)
{
	return Hash::Splittablei64(seed);
}

inline double FastRandom::i32Double(uint32_t seed)
{
	return Shift::ToDouble(Hash::PCGi32(seed));
}

inline float FastRandom::i32Float(uint32_t seed)
{
	return Shift::ToFloat(Hash::PCGi32(seed));
}

inline uint32_t FastRandom::uInt32(uint32_t seed)
{
	return Hash::PCGi32(seed);
}

inline uint64_t FastRandom::CompileTimeSeed()
{
	uint64_t h = (__TIME__[0] - '0') * 10 + (__TIME__[1] - '0'); // hours
	uint64_t m = (__TIME__[3] - '0') * 10 + (__TIME__[4] - '0'); // minutes
	uint64_t s = (__TIME__[6] - '0') * 10 + (__TIME__[7] - '0'); // seconds

	return h * 3600 + m * 60 + s;
}

inline double FastRandom::ScaleDouble(double value, double min, double max)
{
	return min + value * (max - min);
}

inline float FastRandom::ScaleFloat(float value, float min, float max)
{
	return min + value * (max - min);
}

inline uint64_t FastRandom::ScaleUInt(uint64_t value, uint64_t min, uint64_t max)
{
	return min + (value % (max - min + 1));
}

inline uint32_t FastRandom::ScaleUInt(uint32_t value, uint32_t min, uint32_t max)
{
	return min + (value % (max - min + 1));
}

inline int FastRandom::ScaleInt(uint32_t value, int min, int max)
{
	uint32_t range  = (uint32_t)(max - min + 1);
	uint32_t uvalue = (uint32_t)value;
	return min + (int)(uvalue % range);
}

// -------------------------------- Shift Functions --------------------------------

inline double FastRandom::Shift::ToDouble(uint64_t hash)
{
	return (hash >> 11) * (1.0 / ((uint64_t)1 << 53));
}

inline float FastRandom::Shift::ToFloat(uint64_t hash)
{
	uint32_t folded = (uint32_t)(hash ^ (hash >> 32));
	return (folded >> 9) * (1.0f / (1u << 23));
}

inline float FastRandom::Shift::ToFloat(uint32_t hash)
{
	return (hash >> 9) * (1.0f / (1u << 23));
}

inline uint32_t FastRandom::Shift::ToUInt32(uint64_t hash)
{
	return (uint32_t)(hash ^ (hash >> 32));
}

// -------------------------------- Hash Functions --------------------------------

inline uint32_t FastRandom::Hash::PCGi32(uint32_t seed)
{
	seed = seed * 747796405u + 2891336453u;
	seed = ((seed >> ((seed >> 28u) + 4u)) ^ seed) * 277803737u;
	return (seed >> 22u) ^ seed;
}

inline uint64_t FastRandom::Hash::Splittablei64(uint64_t seed)
{
	seed ^= seed >> 30;
	seed *= 0xbf58476d1ce4e5b9;
	seed ^= seed >> 27;
	seed *= 0x94d049bb133111eb;
	seed ^= seed >> 31;
	return seed;
}