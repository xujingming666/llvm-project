//===-- DirectXRegisterInfo.cpp - RegisterInfo for DirectX -*- C++ ------*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the DirectX specific subclass of TargetRegisterInfo.
//
//===----------------------------------------------------------------------===//

#include "DummyRegisterInfo.h"
#include "MCTargetDesc/DummyMCTargetDesc.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"

#define GET_REGINFO_TARGET_DESC
#include "DummyGenRegisterInfo.inc"

using namespace llvm;

DummyRegisterInfo::~DummyRegisterInfo() {}

const MCPhysReg *
DummyRegisterInfo::getCalleeSavedRegs(const MachineFunction *) const {
  return CSR_Dummy_SaveList;
}

BitVector DummyRegisterInfo::getReservedRegs(const MachineFunction &) const {
  BitVector Reserved(getNumRegs());

  Reserved.set(Dummy::R0);
  Reserved.set(Dummy::R1);
  Reserved.set(Dummy::SP);
  Reserved.set(Dummy::LR);

  return Reserved;
}

void DummyRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II, int SPAdj,
                                            unsigned int FIOperandNum,
                                            RegScavenger *RS) const {
  MachineInstr &MI = *II;
  MachineBasicBlock &MBB = *MI.getParent();
  MachineFunction &MF = *MBB.getParent();
  MachineOperand &FrameIndexOp = MI.getOperand(FIOperandNum);
  unsigned FI = FrameIndexOp.getIndex();
  
  MachineOperand *ImmOp = nullptr;

  Register FrameReg = Dummy::NoRegister;
  unsigned offset = getFrameLowering(MF)->getFrameIndexReference(MF, FI, FrameReg).getFixed();

  FrameIndexOp.ChangeToRegister(FrameReg, false);
  ImmOp = &MI.getOperand(FIOperandNum + 1);
  ImmOp->setImm(offset);
}

Register DummyRegisterInfo::getFrameRegister(const MachineFunction & MF) const {
  // return Dummy::SP;
  const DummyFrameLowering *TFI = getFrameLowering(MF);
  return TFI->hasFP(MF) ? Dummy::R31 : Dummy::SP;
}
