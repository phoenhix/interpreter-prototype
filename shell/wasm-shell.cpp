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

#include "implementation/Implementation.h"
#include "semantics/Host.h"
#include "module/Module.h"
#include <cstring>
using namespace wasm;

static int
Error(const char* what, ...) {
  va_list ap;
  va_start(ap, what);
  fprintf(stderr, "wasm-shell: command-line error: ");
  vfprintf(stderr, what, ap);
  va_end(ap);
  return 2; // as in sh(1).
}

int
main(int argc, char* argv[]) {
  AssertHostRequirements();

  const char* moduleName = nullptr;
  std::vector<const char*> args;
  NaNBits::Kind nanBitsKind = NaNBits::Kind::Random;

  // Parse command-line options.
  bool sawDashDash = false;
  for (int i = 1; i < argc; ++i) {
    const char* arg = argv[i];

    if (!sawDashDash && arg[0] == '-') {
      if (arg[1] == '-') {
        if (arg[2] == '\0') {
          sawDashDash = true;
          continue;
        }

        const char* argName = arg + 2;
        const char* equals = strchr(argName, '=');
        size_t len = equals ? equals - argName : strlen(argName);
        const char* val = equals ? equals + 1 : nullptr;

        if (strncmp(argName, "nanbits", len) == 0) {
          if (val && strcmp(val, "random"))
            nanBitsKind = NaNBits::Kind::Random;
          else if (val && strcmp(val, "canonical"))
            nanBitsKind = NaNBits::Kind::Canonical;
          else if (val && strcmp(val, "inverse"))
            nanBitsKind = NaNBits::Kind::Canonical;
          else if (!val)
            return Error("--nanbits usage: --nanbits=<kind>");
          else
            return Error("unknown --nanbits kind: %s (expected random, "
                         "canonical, or inverse)",
                         val);
          continue;
        }

        return Error("unknown command-line option: %s", arg);
      }
    }

    args.push_back(arg);

    if (moduleName == nullptr) {
      moduleName = arg;
      continue;
    }
  }

  Implementation implementation(nanBitsKind);

  Module module(moduleName);

  Process process(args);

  process.importFFI(implementation.FFIHandler());
  process.load(module);

  Status status = run(implementation, process);

  switch (status) {
    case Status::success:
      return EXIT_SUCCESS;
    case Status::failure:
      fprintf(stderr, "FAIL\n");
      return EXIT_FAILURE;
    case Status::oom:
      fprintf(stderr, "OOM\n");
      return 128 + SIGKILL; // as in the Linux OOM killer
    case Status::timeout:
      return 124; // as in timeout(1).
  }
}
