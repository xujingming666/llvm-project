//===-- ARMTargetMachine.h - Define TargetMachine for ARM -------*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_DUMMY_ARMTARGETMACHINE_H
#define LLVM_LIB_TARGET_DUMMY_ARMTARGETMACHINE_H

#include "DummySubtarget.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class DummyTargetMachine : public LLVMTargetMachine {
  DummySubtarget Subtarget;
  std::unique_ptr<TargetLoweringObjectFile> TLOF;

public:
  DummyTargetMachine(const Target &T, const Triple &TargetTriple,
                     const StringRef &CPU, const StringRef &FS,
                     const TargetOptions &Options,
                     const llvm::Optional<llvm::Reloc::Model> &RM,
                     const llvm::Optional<llvm::CodeModel::Model> &CM,
                     const llvm::CodeGenOpt::Level &OL, const bool &is_jit);
  // bool addPassesToEmitFile(
  //   PassManagerBase &PM, raw_pwrite_stream &Out, raw_pwrite_stream *DwoOut,
  //   CodeGenFileType FileType, bool DisableVerify,
  //   MachineModuleInfoWrapperPass *MMIWP);
  const DummySubtarget *getSubtargetImpl() { return &Subtarget; }
  virtual const TargetSubtargetInfo *getSubtargetImpl(const Function &) const {
    return &Subtarget;
  }
  TargetPassConfig *createPassConfig(legacy::PassManagerBase &PM);
  virtual TargetLoweringObjectFile *getObjFileLowering() const override;
};
} // end namespace llvm

#endif // LLVM_LIB_TARGET_ARM_ARMTARGETMACHINE_H
