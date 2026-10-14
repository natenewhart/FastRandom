# FastRandom

A lightweight, header-only C++ random number library. Can be used with or without dependency to C++ std library.

## Header Files

| Header | Dependencies | Description |
|---|---|---|
| `FastRandom.hpp` | `<cstdint>` only | Core hash functions, scaling utilities, compile-time seed generation |
| `FastRandomGen.hpp` | `FastRandom.hpp` | Stateful generator class with persistent ranges |
| `FastRandomSeed.hpp` | `<random>`, `<chrono>` | time-based seed generation using c++ standard library |

## Installation

Copy the headers you need from `include/FastRandom/` into your project.

## Include Options

```cpp
// Option 1: Just the static hash functions — zero std:: dependency
#include "FastRandom.hpp"

// Option 2: Hash functions + stateful generator
#include "FastRandom.hpp"
#include "FastRandomGen.hpp"

// Option 3: Include All
#include "FastRandom.hpp"
#include "FastRandomGen.hpp"
#include "FastRandomSeed.hpp"
```

## Usage

### Static Hash Functions (`FastRandom.hpp`)

Only static functions. Given a seed, hash it and convert it to double, float, or integer.

`FastRandom.hpp` only contains two base [hash functions](#fastrandom-internals) for 32 bit and 64 integers which are used for their respective types.

```cpp
double   d  = FastRandom::i64Double(seed);  // [0, 1) using Splittable64
float    f  = FastRandom::i64Float(seed);   // [0, 1) using Splittable64
uint64_t u  = FastRandom::uInt64(seed);     // raw uint64_t

double   d2 = FastRandom::i32Double(seed);  // [0, 1) using PCG (less precision)
float    f2 = FastRandom::i32Float(seed);   // [0, 1) using PCG
uint32_t u2 = FastRandom::uInt32(seed);     // raw uint32_t
```

### Scaling Utilities (`FastRandom.hpp`)

```cpp
double   d = FastRandom::ScaleDouble(value, 0.0, 100.0);
float    f = FastRandom::ScaleFloat(value, 0.0f, 1.0f);
int      i = FastRandom::ScaleInt(value, -10, 10);
uint64_t u = FastRandom::ScaleUInt(value, 0ull, 1000ull);
```

### Compile-Time Seed (`FastRandom.hpp`)

```cpp
uint64_t seed = FastRandom::CompileTimeSeed(); // Generate a compile time seed using the __TIME__ macro
```

### Random Number Generator Object (`FastRandomGen.hpp`)

A Random Number Generation Object Using 64 Bit and 32 Bit Hashing (Similar to `std::mt19937`)

```cpp
FastRandom::Generator rng;           // Default constructor seeded with CompileTimeSeed()
FastRandom::Generator rng(12345);    // User provided seed

// One-off calls using default ranges
double   d = rng.Double();   // [0.0, 1.0]
float    f = rng.Float();    // [0.0f, 1.0f]
int      i = rng.Int();      // [0, INT32_MAX]
uint64_t u = rng.UInt64();   // [0, UINT64_MAX]
uint32_t v = rng.UInt32();   // [0, UINT32_MAX]

// One-off calls with inline range
double d = rng.Double(0.0, 100.0);
int    i = rng.Int(-50, 50);

// Raw outputs
float    f = rng.RandF();   // [0, 1)
uint64_t u = rng.Rand64();  // raw 64-bit state
uint32_t v = rng.Rand32();  // raw 32-bit value

// Persistent ranges
rng.SetRangeInt(-100, 100);
rng.Int(); // now always draws from [-100, 100]
rng.ResetRanges(); // restore all defaults

// Reseeding during runtime
rng.SetSeed(99999);
```

### C++ Standard Seed Generation (`FastRandomSeed.hpp`)

```cpp
uint32_t s32 = FastRandom::GenerateSeed32();           // std::random_device
uint64_t s64 = FastRandom::GenerateSeed64();           // std::random_device (64-bit)
uint64_t t   = FastRandom::GetCurrentTime<uint64_t>(); // nanoseconds since epoch
uint32_t t32 = FastRandom::GetCurrentTime<uint32_t>();

// Feed into generator
FastRandom::Generator rng(FastRandom::GenerateSeed64());
```

### FastRandom Internals (`FastRandom.hpp`)

Available for advanced use:

```cpp
// Hash functions
uint64_t h64 = FastRandom::Hash::Splittablei64(seed); // SplitMix64
uint32_t h32 = FastRandom::Hash::PCGi32(seed);        // PCG

// Bit shifting into float ranges
double   d = FastRandom::Shift::ToDouble(hash64);  // 64 bits → [0, 1)
float    f = FastRandom::Shift::ToFloat(hash64);   // 64 bits → [0, 1)
float    g = FastRandom::Shift::ToFloat(hash32);   // 32 bits → [0, 1)
uint32_t u = FastRandom::Shift::ToUInt32(hash64);  // 64 bits → 32 bits
```

## Seed Strategy Guide

| Situation | Recommended Seed |
|---|---|
| Reproducible results (testing, procgen) | `CompileTimeSeed()` or a hardcoded value |
| Different every run, no std:: | `GetCurrentTime<uint64_t>()` |
| Best entropy, std:: acceptable | `GenerateSeed64()` |

## License

MIT — see [LICENSE](LICENSE)
