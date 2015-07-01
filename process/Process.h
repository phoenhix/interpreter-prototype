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

#ifndef WEBASSEMBLY_PROCESS_PROCESS_H
#define WEBASSEMBLY_PROCESS_PROCESS_H

#include <memory>

namespace wasm {

class Environment;
class GlobalVariables;
class LinearMemory;
class TrustedStack;
class Module;

struct Process {
  std::unique_ptr<Environment> environment_;
  std::unique_ptr<GlobalVariables> globalVariables_;
  std::unique_ptr<LinearMemory> linearMemory_;
  std::unique_ptr<TrustedStack> trustedStack_;
  std::unique_ptr<Module> module_;

  Process();
};

} // namespace wasm

#endif // include guard
