#include "DummyPassConfig.h"
#include "DummyTargetMachine.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/Pass.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

extern FunctionPass *createDummyIselPass(DummyTargetMachine &TM, CodeGenOpt::Level OptLevel);

DummyPassConfig::DummyPassConfig(DummyTargetMachine &TM,
                                 legacy::PassManagerBase &PM)
    : TargetPassConfig(TM, PM) {
  // substitutePass(&PostRASchedulerID, &PostMachineSchedulerID);
}

bool DummyPassConfig::addPreISel() { return true; }

void DummyPassConfig::addPreRegAlloc() {}

bool DummyPassConfig::addInstSelector() {
  DummyTargetMachine &TM = getTM<DummyTargetMachine>();
  addPass(createDummyIselPass(TM, getOptLevel()));
  return false;
}

void DummyPassConfig::addPreEmitPass() { addPass(&BranchRelaxationPassID); }

void DummyPassConfig::addIRPasses() { TargetPassConfig::addIRPasses(); }

void DummyPassConfig::addOptimizedRegAlloc() {
  TargetPassConfig::addOptimizedRegAlloc();
}

bool DummyPassConfig::addPreRewrite() { return true; }

