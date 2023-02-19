//===--- DTU.h - DTU Tool and ToolChain Implementations ---------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_DUMMY_H
#define LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_DUMMY_H

#include "Gnu.h"
#include "clang/Driver/InputInfo.h"
#include "clang/Driver/ToolChain.h"
#include "clang/Driver/Tool.h"


namespace clang {
namespace driver {
namespace toolchains {
class LLVM_LIBRARY_VISIBILITY DummyToolChain : public Generic_ELF {
public:
  DummyToolChain(const Driver &D, const llvm::Triple &Triple,
               const llvm::opt::ArgList &Args);
  bool IsIntegratedAssemblerDefault() const override {
    return true;
  }
  void AddClangSystemIncludeArgs(
      const llvm::opt::ArgList &DriverArgs,
      llvm::opt::ArgStringList &CC1Args) const override;
  void addClangTargetOptions(const llvm::opt::ArgList &DriverArgs,
                             llvm::opt::ArgStringList &CC1Args,
                             Action::OffloadKind DeviceOffloadKind) const override;
protected:
  Tool *buildLinker() const override;
};
} // end namespace toolchains

namespace tools {
class LLVM_LIBRARY_VISIBILITY DummyLinker : public gnutools::Linker {
public:
  DummyLinker(const ToolChain &TC) : gnutools::Linker(TC) {}
  bool isLinkJob() const override { return true; }
  bool hasIntegratedCPP() const override { return false; }
  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};
} // end namespace tools

} // end namespace driver
} // end namespace clang

#endif // LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_DUMMY_H
