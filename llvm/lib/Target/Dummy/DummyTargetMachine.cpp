//===-- ARMTargetMachine.cpp - Define TargetMachine for ARM ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

#include "DummyTargetMachine.h"
#include "DummyPassConfig.h"
#include "DummySubtarget.h"
#include "TargetInfo/DummyTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/PassRegistry.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"

#include <iostream>

using namespace llvm;

/*
FS: target feature strings
*/
DummyTargetMachine::DummyTargetMachine(
    const Target &T, const Triple &TargetTriple, const StringRef &CPU,
    const StringRef &FS, const TargetOptions &Options,
    const llvm::Optional<llvm::Reloc::Model> &RM,
    const llvm::Optional<llvm::CodeModel::Model> &CM,
    const llvm::CodeGenOpt::Level &OL, const bool &is_jit)
    : LLVMTargetMachine(
          T, "e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i64:32-f64:32",
          TargetTriple, CPU, FS, Options, RM.value(), CM.value(), OL),
      Subtarget(TargetTriple, CPU, FS, *this),
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()) {
  std::cout << "CPU: " << CPU.str() << "FS: " << FS.str() << std::endl;
  // abort();
  initAsmInfo();
}

TargetPassConfig *
DummyTargetMachine::createPassConfig(legacy::PassManagerBase &PM) {
  return new DummyPassConfig(*this, PM);
}

TargetLoweringObjectFile *
DummyTargetMachine::getObjFileLowering() const {
  return TLOF.get();
}

// bool DummyTargetMachine::addPassesToEmitFile(
//     PassManagerBase &PM, raw_pwrite_stream &Out, raw_pwrite_stream *DwoOut,
//     CodeGenFileType FileType, bool DisableVerify,
//     MachineModuleInfoWrapperPass *MMIWP) {
//   // TargetPassConfig *PassConfig = createPassConfig(PM);
//   // PassConfig->addCodeGenPrepare();

//   // if (TargetPassConfig::willCompleteCodeGenPipeline()) {
//   //   PM.add(createDXILEmbedderPass());
//   // }
//   // switch (FileType) {
//   // case CGFT_AssemblyFile:
//   //   PM.add(createPrintModulePass(Out, "", true));
//   //   break;
//   // case CGFT_ObjectFile:
//   //   if (TargetPassConfig::willCompleteCodeGenPipeline()) {
//   //     if (!MMIWP)
//   //       MMIWP = new MachineModuleInfoWrapperPass(this);
//   //     PM.add(MMIWP);
//   //     if (addAsmPrinter(PM, Out, DwoOut, FileType,
//   //                       MMIWP->getMMI().getContext()))
//   //       return true;
//   //   } else
//   //     PM.add(createDXILWriterPass(Out));
//   //   break;
//   // case CGFT_Null:
//   //   break;
//   // }
//   return false;
// }

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeDummyTarget() {
    llvm::errs() << "register DummyTargetMachine ok";
  RegisterTargetMachine<DummyTargetMachine> X(getTheDummyTarget());

  PassRegistry *PR = PassRegistry::getPassRegistry();
}
