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

#include "semantics/Host.h"
#include <cassert>
#include <cfloat>
#include <limits>
using namespace std;
using namespace wasm;

void
wasm::AssertHostRequirements() {
  assert(numeric_limits<float>::is_iec559);
  assert(numeric_limits<double>::is_iec559);

  assert(numeric_limits<float>::radix == 2);
  assert(numeric_limits<double>::radix == 2);

  assert(sizeof(float) == 4);
  assert(sizeof(double) == 8);

  assert(numeric_limits<float>::has_denorm == denorm_present);
  assert(numeric_limits<double>::has_denorm == denorm_present);

  assert(numeric_limits<float>::has_denorm_loss);
  assert(numeric_limits<double>::has_denorm_loss);

  assert(numeric_limits<float>::round_style == round_to_nearest);
  assert(numeric_limits<double>::round_style == round_to_nearest);

  assert(!numeric_limits<float>::traps);
  assert(!numeric_limits<double>::traps);

  assert(FLT_EVAL_METHOD == 0);
}
