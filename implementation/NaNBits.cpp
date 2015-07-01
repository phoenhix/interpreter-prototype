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

#include "implementation/NaNBits.h"
#include <cstring>
using namespace std;
using namespace wasm;

NaNBits::NaNBits(Kind kind)
  : kind_(kind)
  , randEngine_(randDev_()) {}

float
NaNBits::getFloat32() {
  uint32_t x = 0;
  switch (kind_) {
    case Kind::Canonical:
      break;
    case Kind::Inverse:
      x = ~x;
      break;
    case Kind::Random:
      x = uint32_t(randDist_(randEngine_));
      break;
  }
  x |= UINT32_C(0x7F800000);
  float f;
  memcpy(&f, &x, sizeof(f));
  return f;
}

double
NaNBits::getFloat64() {
  uint64_t x = 0;
  switch (kind_) {
    case Kind::Canonical:
      break;
    case Kind::Inverse:
      x = ~x;
      break;
    case Kind::Random:
      x = randDist_(randEngine_);
      break;
  }
  x |= UINT64_C(0x7ff0000000000000);
  double d;
  memcpy(&d, &x, sizeof(d));
  return d;
}

float
NaNBits::transformFloat32(float old) {
  if (propagating_)
    return old;
  return getFloat32();
}

double
NaNBits::transformFloat64(double old) {
  if (propagating_)
    return old;
  return getFloat64();
}
