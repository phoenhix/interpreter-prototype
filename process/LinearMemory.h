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

#ifndef WEBASSEMBLY_PROCESS_LINEARMEMORY_H
#define WEBASSEMBLY_PROCESS_LINEARMEMORY_H

#include <cstdint>
#include <cstring>

namespace wasm {

class TrapHandler;

class LinearMemory {
  std::uint8_t* data_;
  std::size_t size_;

  void fail(const char* why, TrapHandler* trapHandler);
  void resizeImpl(std::size_t newSize, TrapHandler* trapHandler);
  void resizeFailed(TrapHandler* trapHandler);
  void checkAddr(std::size_t accessSize, std::size_t addr, std::uint8_t p2align,
                 TrapHandler* trapHandler);
  void outOfBounds(TrapHandler* trapHandler);

public:
  LinearMemory()
    : data_(nullptr)
    , size_(0) {}
  ~LinearMemory();

  template <typename AddrTy>
  void resize(AddrTy newSize, TrapHandler* trapHandler) {
    std::size_t castedNewSize = newSize;
    if (castedNewSize != newSize) {
      resizeFailed(trapHandler);
      return;
    }
    resizeImpl(castedNewSize, trapHandler);
  }

  template <typename AddrTy, typename T>
  T load(AddrTy addr, std::uint8_t p2align, TrapHandler* trapHandler) {
    std::size_t castedAddr = addr;
    if (castedAddr != addr) {
      outOfBounds(trapHandler);
      return;
    }
    checkAddr(sizeof(T), castedAddr, p2align, trapHandler);
    T value;
    std::memcpy(&value, data_ + castedAddr, sizeof(T));
    return value;
  }

  template <typename AddrTy, typename T>
  void store(AddrTy addr, std::uint8_t p2align, TrapHandler* trapHandler,
             T value) {
    std::size_t castedAddr = addr;
    if (castedAddr != addr) {
      outOfBounds(trapHandler);
      return;
    }
    checkAddr(sizeof(T), castedAddr, p2align, trapHandler);
    std::memcpy(data_ + castedAddr, &value, sizeof(T));
  }
};

} // namespace wasm

#endif // include guard
