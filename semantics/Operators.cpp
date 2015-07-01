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

#include <math.h>
using namespace wasm;

void
InterpretInt32(Node* node, Context* context) {
  switch (node->opcode) {
    case GetLocal: {
      int32_t x = context->load_local_int32(node->payload);
      context->push_int32(x);
      break;
    }
    case SetLocal: {
      int32_t v = context->pop_int32();
      context->store_local_int32(node->payload, v);
      context->push_int32(v);
      break;
    }
    case LoadHeap: {
      int32_t p = context->pop_int32();
      int32_t i = 0;
      int32_t x = context->load_heap_int32(p, i, node->payload);
      context->push_int32(x);
      break;
    }
    case StoreHeap: {
      int32_t p = context->pop_int32();
      int32_t v = context->pop_int32();
      int32_t i = 0;
      context->store_heap_int32(p, i, node->payload, v);
      context->push_int32(v);
      break;
    }
    case LoadHeapWithOffset: {
      int32_t p = context->pop_int32();
      int32_t i = context->read_literal_int32(node->payload);
      int32_t x = context->load_heap_int32(p, i);
      context->push_int32(x);
      break;
    }
    case StoreHeapWithOffset: {
      int32_t p = context->pop_int32();
      int32_t v = context->pop_int32();
      int32_t i = context->read_literal_int32(node->payload);
      context->store_heap_int32(p, i, node->payload, v);
      context->push_int32(v);
      break;
    }
    case LoadGlobal: {
      int32_t x = context->load_global_int32(node->payload);
      context->push_int32(x);
      break;
    }
    case StoreGlobal: {
      int32_t v = context->pop_int32();
      context->store_global_int32(node->payload, v);
      context->push_int32(v);
      break;
    }
    case CallDirect: {
      assert(false && "direct call unimplemented");
      break;
    }
    case CallIndirect: {
      assert(false && "indirect call unimplemented");
      break;
    }
    case AddressOf: {
      int32_t x = context->addressof(node->payload);
      context->push_int32(x);
      break;
    }
    case Literal: {
      int32_t x = context->read_literal_int32(node->payload);
      context->push_int32(x);
      break;
    }
    case Int32Add: {
      int32_t r = context->pop_int32();
      int32_t l = context->pop_int32();
      int32_t x = uint32_t(l) + r;
      context->push_int32(x);
      break;
    }
    case Int32Sub: {
      int32_t r = context->pop_int32();
      int32_t l = context->pop_int32();
      int32_t x = uint32_t(l) - r;
      context->push_int32(x);
      break;
    }
    case Int32Mul: {
      int32_t r = context->pop_int32();
      int32_t l = context->pop_int32();
      int32_t x = uint32_t(l) * r;
      context->push_int32(x);
      break;
    }
    case Int32SDiv: {
      int32_t r = context->pop_int32();
      int32_t l = context->pop_int32();
      if (r == 0) {
        context->trap("signed integer division by zero");
        return;
      }
      if (l == INT32_MIN && r == -1) {
        context->trap("signed integer division overflow");
        return;
      }
      int32_t x = l / r;
      context->push_int32(x);
      break;
    }
    case Int32UDiv: {
      int32_t r = context->pop_int32();
      int32_t l = context->pop_int32();
      if (r == 0) {
        context->trap("unsigned integer division by zero");
        return;
      }
      int32_t x = uint32_t(l) / r;
      context->push_int32(x);
      break;
    }
    case Int32SRem: {
      int32_t r = context->pop_int32();
      int32_t l = context->pop_int32();
      if (r == 0) {
        context->trap("unsigned integer remainder by zero");
        return;
      }
      int32_t x = (l == INT32_MIN && r == -1) ? 0 : (l % r);
      context->push_int32(x);
      break;
    }
    case Int32URem: {
      int32_t r = context->pop_int32();
      int32_t l = context->pop_int32();
      if (r == 0) {
        context->trap("unsigned integer remainder by zero");
        return;
      }
      int32_t x = uint32_t(l) % r;
      context->push_int32(x);
      break;
    }
    case Int32And: {
      int32_t r = context->pop_int32();
      int32_t l = context->pop_int32();
      int32_t x = l & r;
      context->push_int32(x);
      break;
    }
    case Int32Ior: {
      int32_t r = context->pop_int32();
      int32_t l = context->pop_int32();
      int32_t x = l | r;
      context->push_int32(x);
      break;
    }
    case Int32Xor: {
      int32_t r = context->pop_int32();
      int32_t l = context->pop_int32();
      int32_t x = l ^ r;
      context->push_int32(x);
      break;
    }
    case Int32Shl: {
      int32_t r = context->pop_int32();
      int32_t l = context->pop_int32();
      int32_t x = r >= 32 ? 0 : (uint32_t(l) << r);
      context->push_int32(x);
      break;
    }
    case Int32Shr: {
      int32_t r = context->pop_int32();
      int32_t l = context->pop_int32();
      int32_t x = r >= 32 ? 0 : (uint32_t(l) >> r);
      context->push_int32(x);
      break;
    }
    case Int32Sar: {
      int32_t r = context->pop_int32();
      int32_t l = context->pop_int32();
      int32_t x = int32_t(l) >> (r > 31 ? 31 : r);
      context->push_int32(x);
      break;
    }
    case Int32Eq: {
      int32_t r = context->pop_int32();
      int32_t l = context->pop_int32();
      bool x = l == r;
      context->push_boolean(x);
      break;
    }
    case Int32Slt: {
      int32_t r = context->pop_int32();
      int32_t l = context->pop_int32();
      bool x = l < r;
      context->push_boolean(x);
      break;
    }
    case Int32Sle: {
      int32_t r = context->pop_int32();
      int32_t l = context->pop_int32();
      bool x = l <= r;
      context->push_boolean(x);
      break;
    }
    case Int32Ult: {
      int32_t r = context->pop_int32();
      int32_t l = context->pop_int32();
      bool x = uint32_t(l) < r;
      context->push_boolean(x);
      break;
    }
    case Int32Ule: {
      int32_t r = context->pop_int32();
      int32_t l = context->pop_int32();
      bool x = uint32_t(l) <= r;
      context->push_boolean(x);
      break;
    }
    case Float32Eq: {
      float r = context->pop_float32();
      float l = context->pop_float32();
      bool x = l == r;
      context->push_boolean(x);
      break;
    }
    case Float32Lt: {
      float r = context->pop_float32();
      float l = context->pop_float32();
      bool x = l < r;
      context->push_boolean(x);
      break;
    }
    case Float32Le: {
      float r = context->pop_float32();
      float l = context->pop_float32();
      bool x = l <= r;
      context->push_boolean(x);
      break;
    }
    case Float64Eq: {
      double r = context->pop_float64();
      double l = context->pop_float64();
      bool x = l == r;
      context->push_boolean(x);
      break;
    }
    case Float64Lt: {
      double r = context->pop_float64();
      double l = context->pop_float64();
      bool x = l < r;
      context->push_boolean(x);
      break;
    }
    case Float64Le: {
      double r = context->pop_float64();
      double l = context->pop_float64();
      bool x = l <= r;
      context->push_boolean(x);
      break;
    }
    case SInt32FromFloat64: {
      double o = context->pop_float64();
      if (!(o > double(INT32_MIN) - 1 && o < double(INT32_MAX) + 1)) {
        context->trap("float to signed integer conversion failure");
        return;
      }
      int32_t i = o;
      context->push_int32(i);
      break;
    }
    case SInt32FromFloat32: {
      float o = context->pop_float32();
      if (!(o > double(INT32_MIN) - 1 && o < double(INT32_MAX) + 1)) {
        context->trap("float to signed integer conversion failure");
        return;
      }
      int32_t i = o;
      context->push_int32(i);
      break;
    }
    case Uint32FromFloat64: {
      double o = context->pop_float64();
      if (!(o > -1.0 && o < double(UINT32_MAX) + 1)) {
        context->trap("float to unsigned integer conversion failure");
        return;
      }
      int32_t i = o;
      context->push_int32(i);
      break;
    }
    case Uint32FromFloat32: {
      float o = context->pop_float32();
      if (!(o > -1.0 && o < double(UINT32_MAX) + 1)) {
        context->trap("float to unsigned integer conversion failure");
        return;
      }
      int32_t i = o;
      context->push_int32(i);
      break;
    }
    case Int32fromFloat32Bits: {
      float o = context->pop_float32();
      int32_t x;
      static_assert(sizeof(o) == sizeof(x));
      memcpy(&x, &o, sizeof(o));
      context->push_int32(x);
      break;
    }
  }
}

void
InterpretFloat32(Node* node, Context* context) {
  switch (node->opcode) {
    case GetLocal: {
      float x = context->load_local_float32(node->payload);
      context->push_float32(x);
      break;
    }
    case SetLocal: {
      float v = context->pop_float32();
      context->store_local_float32(node->payload, v);
      context->push_float32(v);
      break;
    }
    case LoadHeap: {
      int32_t p = context->pop_int32();
      int32_t i = 0;
      float x = context->load_heap_float32(p, i, node->payload);
      context->push_float32(x);
      break;
    }
    case StoreHeap: {
      int32_t p = context->pop_int32();
      float v = context->pop_float32();
      int32_t i = 0;
      context->store_heap_float32(p, i, node->payload, v);
      context->push_float32(v);
      break;
    }
    case LoadHeapWithOffset: {
      int32_t p = context->pop_int32();
      int32_t i = context->read_literal_int32(node->payload);
      float x = context->load_heap_float32(p, i, node->payload);
      context->push_float32(x);
      break;
    }
    case StoreHeapWithOffset: {
      int32_t p = context->pop_int32();
      float v = context->pop_float32();
      int32_t i = context->read_literal_int32(node->payload);
      context->store_heap_float32(p, i, node->payload v);
      context->push_float32(v);
      break;
    }
    case LoadGlobal: {
      float x = context->load_global_float32(node->payload);
      context->push_float32(x);
      break;
    }
    case StoreGlobal: {
      float v = context->pop_float32();
      context->store_global_float32(node->payload, v);
      context->push_float32(v);
      break;
    }
    case CallDirect: {
      assert(false && "direct call unimplemented");
      break;
    }
    case CallIndirect: {
      assert(false && "indirect call unimplemented");
      break;
    }
    case Literal: {
      float x = context->read_literal_float32(node->payload);
      context->push_float32(x);
      break;
    }
    case Float32Add: {
      float r = context->pop_float32();
      float l = context->pop_float32();
      float x = l + r;
      if (x != x)
        x = nan_float32(node->opcode, l, r);
      context->push_float32(x);
      break;
    }
    case Float32Sub: {
      float r = context->pop_float32();
      float l = context->pop_float32();
      float x = l - r;
      if (x != x)
        x = nan_float32(node->opcode, l, r);
      context->push_float32(x);
      break;
    }
    case Float32Mul: {
      float r = context->pop_float32();
      float l = context->pop_float32();
      float x = l * r;
      if (x != x)
        x = nan_float32(node->opcode, l, r);
      context->push_float32(x);
      break;
    }
    case Float32Div: {
      float r = context->pop_float32();
      float l = context->pop_float32();
      // C++ annoyingly says that division by 0 is UB even for floating point.
      float x =
        r == 0 ? (l == 0 ? float(NAN) : copysignf(INFINITY, l * r)) : l / r;
      if (x != x)
        x = nan_float32(node->opcode, l, r);
      context->push_float32(x);
      break;
    }
    case Float32Abs: {
      float o = context->pop_float32();
      float x = fabsf(o);
      context->push_float32(x);
      break;
    }
    case Float32Neg: {
      float o = context->pop_float32();
      float x = -o;
      context->push_float32(x);
      break;
    }
    case Float32Copysign: {
      float r = context->pop_float32();
      float l = context->pop_float32();
      float x = copysignf(l, r);
      context->push_float32(x);
      break;
    }
    case Float32Ceil: {
      float o = context->pop_float32();
      float x = ceilf(o);
      if (x != x)
        x = nan_float32(node->opcode, o);
      context->push_float32(x);
      break;
    }
    case Float32Floor: {
      float o = context->pop_float32();
      float x = floorf(o);
      if (x != x)
        x = nan_float32(node->opcode, o);
      context->push_float32(x);
      break;
    }
    case Float32Sqrt: {
      float o = context->pop_float32();
      float x = sqrtf(o);
      if (x != x)
        x = nan_float32(node->opcode, o);
      context->push_float32(x);
      break;
    }
    case Float32FromFloat64: {
      double o = context->pop_float64();
      float x = o;
      if (x != x)
        x = nan_float32(node->opcode, o);
      context->push_float32(x);
      break;
    }
    case Float32FromSInt32: {
      int32_t o = context->pop_int32();
      float x = o;
      context->push_float32(x);
      break;
    }
    case Float32FromUInt32: {
      int32_t o = context->pop_int32();
      float x = uint32_t(o);
      context->push_float32(x);
      break;
    }
    case Float32FromInt32Bits: {
      int32_t o = context->pop_int32();
      float x;
      static_assert(sizeof(o) == sizeof(x));
      memcpy(&x, &o, sizeof(o));
      context->push_int32(x);
      break;
    }
  }
}

void
InterpretFloat64(Node* node, Context* context) {
  switch (node->opcode) {
    case GetLocal: {
      double x = context->load_local_float64(node->payload);
      context->push_float64(x);
      break;
    }
    case SetLocal: {
      double v = context->pop_float64();
      context->store_local_float64(node->payload, v);
      context->push_float64(v);
      break;
    }
    case LoadHeap: {
      int32_t p = context->pop_int32();
      int32_t i = 0;
      double x = context->load_heap_float64(p, i, node->payload);
      context->push_float64(x);
      break;
    }
    case StoreHeap: {
      int32_t p = context->pop_int32();
      double v = context->pop_float64();
      int32_t i = 0;
      context->store_heap_float64(p, i, node->payload, v);
      context->push_float64(v);
      break;
    }
    case LoadHeapWithOffset: {
      int32_t p = context->pop_int32();
      int32_t i = context->read_literal_int32(node->payload);
      double x = context->load_heap_float64(p, i, node->payload);
      context->push_float64(x);
      break;
    }
    case StoreHeapWithOffset: {
      int32_t p = context->pop_int32();
      double v = context->pop_float64();
      int32_t i = context->read_literal_int32(node->payload);
      context->store_heap_float64(p, i, node->payload, v);
      context->push_float64(v);
      break;
    }
    case LoadGlobal: {
      double x = context->load_global_float64(node->payload);
      context->push_float64(x);
      break;
    }
    case StoreGlobal: {
      double v = context->pop_float64();
      context->store_global_float64(node->payload, v);
      context->push_float64(v);
      break;
    }
    case CallDirect: {
      assert(false && "direct call unimplemented");
      break;
    }
    case CallIndirect: {
      assert(false && "indirect call unimplemented");
      break;
    }
    case Literal: {
      double x = context->read_literal_float64(node->payload);
      context->push_float64(x);
      break;
    }
    case Float64Add: {
      double r = context->pop_float64();
      double l = context->pop_float64();
      double x = l + r;
      if (x != x)
        x = nan_float64(node->opcode, l, r);
      context->push_float64(x);
      break;
    }
    case Float64Sub: {
      double r = context->pop_float64();
      double l = context->pop_float64();
      double x = l - r;
      if (x != x)
        x = nan_float64(node->opcode, l, r);
      context->push_float64(x);
      break;
    }
    case Float64Mul: {
      double r = context->pop_float64();
      double l = context->pop_float64();
      double x = l * r;
      if (x != x)
        x = nan_float64(node->opcode, l, r);
      context->push_float64(x);
      break;
    }
    case Float64Div: {
      double r = context->pop_float64();
      double l = context->pop_float64();
      // C++ annoyingly says that division by 0 is UB even for floating point.
      double x = r == 0 ? (l == 0 ? NAN : copysign(INFINITY, l * r)) : l / r;
      if (x != x)
        x = nan_float64(node->opcode, l, r);
      context->push_float64(x);
      break;
    }
    case Float64Abs: {
      double o = context->pop_float64();
      double x = fabs(o);
      context->push_float64(x);
      break;
    }
    case Float64Neg: {
      double o = context->pop_float64();
      double x = -o;
      context->push_float64(x);
      break;
    }
    case Float64Copysign: {
      double r = context->pop_float64();
      double l = context->pop_float64();
      double x = copysign(l, r);
      context->push_float64(x);
      break;
    }
    case Float64Ceil: {
      double o = context->pop_float64();
      double x = ceil(o);
      if (x != x)
        x = nan_float64(node->opcode, o);
      context->push_float64(x);
      break;
    }
    case Float64Floor: {
      double o = context->pop_float64();
      double x = floor(o);
      if (x != x)
        x = nan_float64(node->opcode, o);
      context->push_float64(x);
      break;
    }
    case Float64Sqrt: {
      double o = context->pop_float64();
      double x = sqrt(o);
      if (x != x)
        x = nan_float64(node->opcode, o);
      context->push_float64(x);
      break;
    }
    case Float64FromFloat32: {
      float o = context->pop_float32();
      double x = o;
      if (x != x)
        x = nan_float64(node->opcode, o);
      context->push_float64(x);
      break;
    }
    case Float64FromSInt32: {
      int32_t o = context->pop_int32();
      double x = o;
      context->push_float64(x);
      break;
    }
    case Float64FromUInt32: {
      int32_t o = context->pop_int32();
      double x = uint32_t(o);
      context->push_float64(x);
      break;
    }
  }
}
