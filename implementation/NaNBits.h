/*
 * Copyright 2015 WebAssembly Community Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WEBASSEMBLY_IMPLEMENTATION_NANBITS_H
#define WEBASSEMBLY_IMPLEMENTATION_NANBITS_H

#include <random>

namespace wasm {

class NaNBits {
public:
  enum class Kind {
    Canonical, // Canonical NaN; payload and sign bit are zero
    Inverse,   // Inverse canonical NaN; payload and sign bit are one
    Random,    // Payload and sign bit are assigned pseudo-randomly.
  };

private:
  Kind kind_;
  bool propagating_;
  std::random_device randDev_;
  std::default_random_engine randEngine_;
  std::uniform_int_distribution<uint64_t> randDist_;

public:
  explicit NaNBits(Kind kind);

  float getFloat32();
  double getFloat64();

  float transformFloat32(float old);
  double transformFloat64(double old);
};

} // namespace wasm

#endif // include guard
