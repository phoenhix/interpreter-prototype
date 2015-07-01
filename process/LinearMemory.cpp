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

#include "process/LinearMemory.h"
#include "implementation/TrapHandler.h"
#include <climits>
#include <cstdlib>
using namespace std;
using namespace wasm;

void
LinearMemory::fail(const char* why, TrapHandler* trapHandler) {
  free(data_);
  data_ = nullptr;
  size_ = 0;
  trapHandler->trap(why);
}

void
LinearMemory::resizeImpl(size_t newSize, TrapHandler* trapHandler) {
  uint8_t* newData = static_cast<uint8_t*>(realloc(data_, newSize));
  if (newData == nullptr) {
    resizeFailed(trapHandler);
    return;
  }

  if (newSize > size_)
    memset(newData + size_, 0, newSize - size_);
  data_ = newData;
  size_ = newSize;
}

void
LinearMemory::resizeFailed(TrapHandler* trapHandler) {
  fail("linear memory resize failed", trapHandler);
}

void
LinearMemory::checkAddr(size_t accessSize, size_t addr, uint8_t p2align,
                        TrapHandler* trapHandler) {
  size_t endAddr = addr + accessSize;
  if (endAddr < addr || endAddr > size_) {
    outOfBounds(trapHandler);
    return;
  }

  if (p2align < CHAR_BIT * sizeof(size_t)) {
    if (addr & (~size_t(0) >> (CHAR_BIT * sizeof(size_t) - p2align)))
      trapHandler->slow("linear memory access address underaligned");
  }
}

void
LinearMemory::outOfBounds(TrapHandler* trapHandler) {
  fail("linear memory address out of bounds", trapHandler);
}

LinearMemory::~LinearMemory() {
  free(data_);
}
