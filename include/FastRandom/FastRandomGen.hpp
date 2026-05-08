// Nate Newhart - FastRandom random generator class which utilizes the hash functions in FastRandom.hpp 

#pragma once

#include "FastRandom.hpp"
#include  <climits> // For INT_MAX macros

namespace FastRandom
{
    class Generator
    {
    public:
        Generator();                    // Default seeds with CompileTimeSeed()
        Generator(uint64_t seed);       // User provided seed

        void SetSeed(uint64_t seed);    // Reseed at runtime

        // Range Setting Functions
        void SetRangeDouble(double min, double max);
        void SetRangeFloat (float min, float max);
        void SetRangeInt   (int min, int max);
        void SetRangeUInt64(uint64_t min, uint64_t max);
        void SetRangeUInt32(uint32_t min, uint32_t max);

        void ResetRanges();

        // Random Functions
		float    RandF();  // Returns a float in [0, 1) using the current state
		uint64_t Rand64(); // Returns a uint64_t using the current state
		uint32_t Rand32(); // Returns a uint32_t using the current state

        double   Double();
        double   Double(double min, double max);
        float    Float();
        float    Float(float min, float max);
        int      Int();
        int      Int(int min, int max);
        uint64_t UInt64();
        uint64_t UInt64(uint64_t min, uint64_t max);
        uint32_t UInt32();
        uint32_t UInt32(uint32_t min, uint32_t max);

    private:
		void ProcessState(); // Update internal state using the current seed and hash function

    private:
        uint64_t mState;  // Current PRNG state

        // Stored ranges
        int      mIntMin, mIntMax;
        float    mFloatMin,  mFloatMax;
        double   mDoubleMin, mDoubleMax;
        uint64_t mUInt64Min, mUInt64Max;
        uint32_t mUInt32Min, mUInt32Max;
    };
}

// -------------------------------- Inline Function Definitions --------------------------------

inline FastRandom::Generator::Generator()
    : mState(CompileTimeSeed())
    , mDoubleMin(0.0),  mDoubleMax(1.0)
    , mFloatMin (0.0f), mFloatMax(1.0f)
    , mIntMin   (0),    mIntMax(INT32_MAX)
    , mUInt64Min(0),    mUInt64Max(UINT64_MAX)
	, mUInt32Min(0),    mUInt32Max(UINT32_MAX)
{
	ProcessState(); // Initialize state with the first hash
}

inline FastRandom::Generator::Generator(uint64_t seed)
    : mState    (seed)
    , mDoubleMin(0.0),  mDoubleMax(1.0)
    , mFloatMin (0.0f), mFloatMax(1.0f)
    , mIntMin   (0),    mIntMax(INT32_MAX)
    , mUInt64Min(0),    mUInt64Max(UINT64_MAX)
    , mUInt32Min(0),    mUInt32Max(UINT32_MAX)
{
	ProcessState(); // Initialize state with the first hash
}

inline void FastRandom::Generator::ProcessState()
{
	mState = Hash::Splittablei64(mState);
}

inline void FastRandom::Generator::SetSeed(uint64_t seed)
{
    mState = seed;
}

inline void FastRandom::Generator::SetRangeDouble(double min, double max)
{
	mDoubleMin = min;
    mDoubleMax = max;
}

inline void FastRandom::Generator::SetRangeFloat(float min, float max)
{
    mFloatMin = min;
	mFloatMax = max;
}

inline void FastRandom::Generator::SetRangeInt(int min, int max)
{
	mIntMin = min;
	mIntMax = max;
}

inline void FastRandom::Generator::SetRangeUInt64(uint64_t min, uint64_t max)
{
    mUInt64Min = min;
	mUInt64Max = max;
}

inline void FastRandom::Generator::SetRangeUInt32(uint32_t min, uint32_t max)
{
    mUInt32Min = min;
	mUInt32Max = max;
}

inline void FastRandom::Generator::ResetRanges()
{
    mDoubleMin = 0.0;  mDoubleMax = 1.0;
    mFloatMin  = 0.0f; mFloatMax  = 1.0f;
    mIntMin    = 0;    mIntMax    = INT32_MAX;
    mUInt64Min = 0;    mUInt64Max = UINT64_MAX;
	mUInt32Min = 0;    mUInt32Max = UINT32_MAX;
}

inline float FastRandom::Generator::RandF()
{
    ProcessState();
    return Shift::ToFloat(mState);
}

inline uint64_t FastRandom::Generator::Rand64()
{
    ProcessState();
	return mState;
}

inline uint32_t FastRandom::Generator::Rand32()
{
    ProcessState();
	return Shift::ToUInt32(mState);
}

inline double FastRandom::Generator::Double()
{
    ProcessState();
	return ScaleDouble(Shift::ToDouble(mState), mDoubleMin, mDoubleMax);
}

inline double FastRandom::Generator::Double(double min, double max)
{
    ProcessState();
	return ScaleDouble(Shift::ToDouble(mState), min, max);
}

inline float FastRandom::Generator::Float()
{
    ProcessState();
	return ScaleFloat(Shift::ToFloat(mState), mFloatMin, mFloatMax);
}

inline float FastRandom::Generator::Float(float min, float max)
{
    ProcessState();
	return ScaleFloat(Shift::ToFloat(mState), min, max);
}

inline int FastRandom::Generator::Int()
{
    ProcessState();
	return ScaleInt(Shift::ToUInt32(mState), mIntMin, mIntMax);
}

inline int FastRandom::Generator::Int(int min, int max)
{
    ProcessState();
	return ScaleInt(Shift::ToUInt32(mState), min, max);
}

inline uint64_t FastRandom::Generator::UInt64()
{
    ProcessState();
	return ScaleUInt(mState, mUInt64Min, mUInt64Max);
}

inline uint64_t FastRandom::Generator::UInt64(uint64_t min, uint64_t max)
{
    ProcessState();
	return ScaleUInt(mState, min, max);
}

inline uint32_t FastRandom::Generator::UInt32()
{
    ProcessState();
	return ScaleUInt(Shift::ToUInt32(mState), mUInt32Min, mUInt32Max);
}

inline uint32_t FastRandom::Generator::UInt32(uint32_t min, uint32_t max)
{
    ProcessState();
	return ScaleUInt(Shift::ToUInt32(mState), min, max);
}   