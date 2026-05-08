// Nate Newhart - FastRandom C++ standard library dependent seed generation
// All in one header file for ease of access

#pragma once

#include <random>
#include <chrono>

namespace FastRandom
{
	template<typename T>
	T GetCurrentTime(); // Get current time in nanoseconds for seed generation, cast to uint32_t or uint64_t

	// Seed generation using std::random_device
	uint32_t GenerateSeed32();
	uint64_t GenerateSeed64();
}

// -------------------------------- Inline Function Definitions --------------------------------

template<typename T>
T FastRandom::GetCurrentTime()
{
	static_assert(std::is_same_v<T, uint32_t> || std::is_same_v<T, uint64_t>, "GetCurrentTime only supports uint32_t and uint64_t");

	auto now = std::chrono::high_resolution_clock::now();
	auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());

	return static_cast<T>(ns.count());
}

inline uint32_t FastRandom::GenerateSeed32()
{
	return std::random_device{}();
}

inline uint64_t FastRandom::GenerateSeed64()
{
	auto rd = std::random_device{};
	return ((uint64_t)rd() << 32) | rd();
}