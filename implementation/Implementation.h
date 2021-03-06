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

#ifndef WEBASSEMBLY_IMPLEMENTATION_IMPLEMENTATION_H
#define WEBASSEMBLY_IMPLEMENTATION_IMPLEMENTATION_H

#include "implementation/NaNBits.h"
#include <memory>

namespace wasm {

class TrapHandler;
class FFIHandler;

struct Implementation {
  std::unique_ptr<TrapHandler> trapHandler_;
  std::unique_ptr<FFIHandler> ffiHandler_;
  std::unique_ptr<NaNBits> nanBits_;

  explicit Implementation(NaNBits::Kind nanBitsKind);
};

} // namespace wasm

#endif // include guard
