//===--- DirectX.cpp - Implement DirectX target feature support -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements DirectX TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#include "Dummy.h"
#include "clang/Basic/MacroBuilder.h"
#include "clang/Basic/TargetBuiltins.h"

using namespace clang;
using namespace clang::targets;

void DummyTargetInfo::getTargetDefines(const LangOptions &Opts,
                                       MacroBuilder &Builder) const {
  Builder.defineMacro("DUMMY");
  Builder.defineMacro("__DUMMY__");
}

const Builtin::Info DummyTargetInfo::BuiltinInfo[] = {
  Builtin::Info(),
};

ArrayRef<Builtin::Info> DummyTargetInfo::getTargetBuiltins() const {
  // Some testcases have no -target-cpu
  return llvm::makeArrayRef(BuiltinInfo);
}

static constexpr llvm::StringLiteral ValidFamilyNames[] = {
    "dummy",
};

bool DummyTargetInfo::isValidCPUName(StringRef Name) const {
  bool IsFamily =
      llvm::find(ValidFamilyNames, Name) != std::end(ValidFamilyNames);

  return IsFamily;
}

void DummyTargetInfo::fillValidCPUList(
    SmallVectorImpl<StringRef> &Values) const {
  Values.append(std::begin(ValidFamilyNames), std::end(ValidFamilyNames));
}
