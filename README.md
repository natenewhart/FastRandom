# FastRandom

A lightweight, header-only C++ random number library built on hash functions with zero heavy dependencies. Include only what you need.

## Headers

| Header | Depends On | Description |
|---|---|---|
| `FastRandom.hpp` | `<cstdint>` only | Core hash functions, scaling utilities, compile-time seed |
| `FastRandomGen.hpp` | `FastRandom.hpp` | Stateful generator class with persistent ranges |
| `FastRandomSeed.hpp` | `<random>`, `<chrono>` | OS-backed and time-based seed generation |

## Installation

Copy the headers you need from `include/FastRandom/` into your project. No build step required.

## Include Options

```cpp
// Option 1: Just the static hash functions — zero std:: dependency
#include "FastRandom.hpp"

// Option 2: Hash functions + stateful generator
#include "FastRandom.hpp"
#include "FastRandomGen.hpp"

// Option 3: Everything, including OS-backed seed generation
#include "FastRandom.hpp"
#include "FastRandomGen.hpp"
#include "FastRandomSeed.hpp"
```

## Usage

### Static Hash Functions (`FastRandom.hpp`)

Only static functions. Given a seed, hash it and convert it to double, float, or integer.

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
uint64_t seed = FastRandom::CompileTimeSeed(); // Baked in at compile time — same every run
```

### Stateful Generator (`FastRandomGen.hpp`)

Works like `std::mt19937` but hash-based and lighter weight.

```cpp
FastRandom::Generator rng;           // Seeded with CompileTimeSeed()
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

// Reseed at runtime
rng.SetSeed(99999);
```

### OS-Backed Seed Generation (`FastRandomSeed.hpp`)

```cpp
uint32_t s32 = FastRandom::GenerateSeed32();           // std::random_device
uint64_t s64 = FastRandom::GenerateSeed64();           // std::random_device (64-bit)
uint64_t t   = FastRandom::GetCurrentTime<uint64_t>(); // nanoseconds since epoch
uint32_t t32 = FastRandom::GetCurrentTime<uint32_t>();

// Feed into generator
FastRandom::Generator rng(FastRandom::GenerateSeed64());
```

### Hash & Shift Internals (`FastRandom.hpp`)

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

## Requirements

- C++17 or later (for `std::is_same_v`)
- `FastRandomSeed.hpp` requires `<random>` and `<chrono>`
- `FastRandom.hpp` and `FastRandomGen.hpp` require only `<cstdint>` and `<climits>`

## License

MIT — see [LICENSE](LICENSE)
