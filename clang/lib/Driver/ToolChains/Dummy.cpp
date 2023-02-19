//===--- DTU.cpp - DTU ToolChain Implementations ----------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Dummy.h"
#include "CommonArgs.h"
#include "clang/Driver/DriverDiagnostic.h"
#include "clang/Driver/Options.h"
#include "clang/Driver/Compilation.h"
#include "llvm/Option/ArgList.h"

using namespace clang::driver;
using namespace clang::driver::toolchains;
using namespace clang::driver::tools;
using namespace clang;
using namespace llvm::opt;

DummyToolChain::DummyToolChain(const Driver &D, const llvm::Triple &Triple,
                           const llvm::opt::ArgList &Args)
  : Generic_ELF(D, Triple, Args) {}

Tool *DummyToolChain::buildLinker() const {
  return new DummyLinker(*this);
}
void DummyToolChain::AddClangSystemIncludeArgs(const ArgList &DriverArgs,
                                          ArgStringList &CC1Args) const {
  if (!DriverArgs.hasArg(clang::driver::options::OPT_nostdinc))
    addSystemInclude(DriverArgs, CC1Args,
                     StringRef(getDriver().getInstalledDir()) +
                     "/../lib/DTU/include");
}
void DummyToolChain::addClangTargetOptions(const ArgList &DriverArgs,
                                         ArgStringList &CC1Args,
                                         Action::OffloadKind) const {
  const Driver &D = getDriver();

  CC1Args.push_back("-nostdsysteminc");
  CC1Args.push_back("-fnative-half-type");
}

void DummyLinker::ConstructJob(Compilation &C, const JobAction &JA,
                          const InputInfo &Output,
                          const InputInfoList &Inputs,
                          const ArgList &Args,
                          const char *LinkingOutput) const {


  std::string Linker = getToolChain().GetProgramPath(getShortName());
  auto BinPath = getToolChain().getDriver().getInstalledDir();
  ArgStringList CmdArgs;

  CmdArgs.push_back("-o");
  CmdArgs.push_back(Output.getFilename());

  C.addCommand(std::make_unique<Command>(JA, *this, ResponseFileSupport::None(), Args.MakeArgString(Linker),
                                          CmdArgs, Inputs));
}
