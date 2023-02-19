//===--- DirectX.h - Declare DirectX target feature support -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares DXIL TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_BASIC_TARGETS_DUMMY_H
#define LLVM_CLANG_LIB_BASIC_TARGETS_DUMMY_H
#include "clang/Basic/CharInfo.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Support/Compiler.h"
#include <cstdlib>
#include <mutex>
#include <vector>

namespace clang {
namespace targets {

class LLVM_LIBRARY_VISIBILITY DummyTargetInfo : public TargetInfo {

  static const Builtin::Info BuiltinInfo[];

public:
  DummyTargetInfo(const llvm::Triple &Triple, const TargetOptions &)
      : TargetInfo(Triple) {
    TLSSupported = false;
    IntWidth = 32;
    IntAlign = 32;
    LongWidth = 32;
    LongLongWidth = 64;
    LongAlign = LongLongAlign = 16;
    PointerWidth = 32;
    PointerAlign = 32;
    SuitableAlign = 32;
    SizeType = UnsignedInt;
    IntMaxType = SignedLongLong;
    IntPtrType = SignedInt;
    PtrDiffType = SignedInt;
    SigAtomicType = SignedLong;
    BFloat16Width = BFloat16Align = 16;
    BFloat16Format = &llvm::APFloat::BFloat();

    resetDataLayout("e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i64:32-f64:32");
  }

  void getTargetDefines(const LangOptions &Opts,
                        MacroBuilder &Builder) const override;

  bool isValidCPUName(StringRef Name) const override;
  bool setCPU(const std::string &Name) override {
    bool isValid = isValidCPUName(Name);
    if (isValid)
      CPU = Name;
    return isValid;
  }

  bool handleTargetFeatures(std::vector<std::string> &Features,
                            DiagnosticsEngine &Diags) override {
    return true;
  }

  void fillValidCPUList(SmallVectorImpl<StringRef> &Values) const override;

  ArrayRef<Builtin::Info> getTargetBuiltins() const override;

  bool isValidGCCRegisterName(StringRef Name) const override {
    if (isDigit(Name[0]))
      return false;
    return TargetInfo::isValidGCCRegisterName(Name);
  }

  ArrayRef<const char *> getGCCRegNames() const override {
    static std::vector<const char *> GCCRegNames({
        "r0",  "r1",  "r2",  "r3",  "r4",  "r5",  "r6",  "r7",
        "r8",  "r9",  "r10", "r11", "r12", "r13", "r14", "r15",
        "r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23",
        "r24", "r25", "r26", "r27", "r28", "r29", "r30", "r31",
    });
    return llvm::makeArrayRef(GCCRegNames);
  }

  ArrayRef<TargetInfo::GCCRegAlias> getGCCRegAliases() const override {
    // No aliases.
    return None;
  }

  bool validateAsmConstraint(const char *&Name,
                             TargetInfo::ConstraintInfo &Info) const override {
    switch (*Name) {
    default:
      return false;
    case 'r':
    case 'v':
    case 'c':
      // "va" also matched here
    case 'a':
    case 's':
    case 'd':
    case 'q':
      Info.setAllowsRegister();
      return true;
    case 'i':
      Info.setRequiresImmediate();
      return true;
    }
  }

  std::string convertConstraint(const char *&Constraint) const override {
    if (Constraint[0] == 'v' && Constraint[1] == 'a') {
      Constraint++;
      return std::string("^va");
    } else if (Constraint[0] == 'c' && Constraint[1] == 'v') {
      Constraint++;
      return std::string("^vcc");
    } else if (Constraint[0] == 'i' && Constraint[1] == 'v') {
      Constraint++;
      return std::string("^iv");
    } else if (Constraint[0] == 'd' && Constraint[1] == 'a') {
      Constraint++;
      return std::string("^da");
    } else if (Constraint[0] == 'q' && Constraint[1] == 'a') {
      Constraint++;
      return std::string("^qa");
    } else if (Constraint[0] == 'd' && Constraint[1] == 'v') {
      Constraint++;
      return std::string("^dv");
    } else if (Constraint[0] == 'd' && Constraint[1] == 'i') {
      Constraint++;
      return std::string("^di");
    }
    return TargetInfo::convertConstraint(Constraint);
  }

  const char *getClobbers() const override { return ""; }

  BuiltinVaListKind getBuiltinVaListKind() const override {
    // TODO: support VaList
    return TargetInfo::CharPtrBuiltinVaList;
  }

  CallingConvCheckResult checkCallingConvention(CallingConv CC) const override {
    return CCCR_OK;
  }

protected:
  std::string CPU;
};

} // namespace targets
} // namespace clang

#endif // LLVM_CLANG_LIB_BASIC_TARGETS_DUMMY_H
