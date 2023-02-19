//===-- DirectXRegisterInfo.h - Define RegisterInfo for DirectX -*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the DirectX specific subclass of TargetRegisterInfo.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_DUMMY_REGISTERINFO_H
#define LLVM_DUMMY_REGISTERINFO_H

#include "DummyFrameLowering.h"
#include "MCTargetDesc/DummyMCTargetDesc.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/MC/MCRegister.h"

#define GET_REGINFO_HEADER
#include "DummyGenRegisterInfo.inc"

namespace llvm {
struct DummyRegisterInfo : public DummyGenRegisterInfo {
  MCPhysReg CSR_Dummy_SaveList[4];

  DummyRegisterInfo()
      : DummyGenRegisterInfo(0),
        CSR_Dummy_SaveList({Dummy::R0, Dummy::R1, Dummy::SP, Dummy::LR}) {}
  ~DummyRegisterInfo();

  virtual const MCPhysReg *getCalleeSavedRegs(const MachineFunction *) const;
  virtual BitVector getReservedRegs(const MachineFunction &) const;
  virtual void eliminateFrameIndex(MachineBasicBlock::iterator, int,
                                   unsigned int, RegScavenger *) const;
  virtual Register getFrameRegister(const MachineFunction & MF) const;
};
} // namespace llvm

#endif // LLVM_DUMMY_REGISTERINFO_H
