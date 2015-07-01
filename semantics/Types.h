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

#ifndef WEBASSEMBLY_SEMANTICS_TYPES_H
#define WEBASSEMBLY_SEMANTICS_TYPES_H

#include <cstdint>

namespace wasm {

enum class Types {
  int32,
  int64,
  float32,
  float64,
  void_,
};

template <typename TypeTy>
struct TypeTraits {};

template <>
struct TypeTraits<Types::int32> {
  typedef std::int32_t HostTy;
};
template <>
struct TypeTraits<Types::int64> {
  typedef std::int64_t HostTy;
};
template <>
struct TypeTraits<Types::float32> {
  typedef float HostTy;
};
template <>
struct TypeTraits<Types::float64> {
  typedef double HostTy;
};
template <>
struct TypeTraits<Types::void_> {
  typedef void HostTy;
};

} // namespace wasm

#endif
