//===-- DummyPassConfig.h - Define TargetMachine for ARM -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the ARM specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DUMMY_PASSCONFIG_H
#define LLVM_LIB_TARGET_DUMMY_PASSCONFIG_H

#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/LegacyPassManager.h"

namespace llvm {
class DummyTargetMachine;

class DummyPassConfig : public TargetPassConfig {
public:
  DummyPassConfig(DummyTargetMachine &TM, legacy::PassManagerBase &PM);
  bool addPreISel() override;
  bool addInstSelector() override;
  void addPreRegAlloc() override;
  void addOptimizedRegAlloc() override;
  void addPreEmitPass() override;
  void addIRPasses() override;
  bool addPreRewrite() override;
};
} // end namespace llvm

#endif // LLVM_LIB_TARGET_DUMMY_PASSCONFIG_H