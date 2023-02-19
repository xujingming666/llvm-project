//===-- DirectXInstrInfo.cpp - InstrInfo for DirectX -*- C++ ------------*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the DirectX specific subclass of TargetInstrInfo.
//
//===----------------------------------------------------------------------===//

#include "DummyFrameLowering.h"
#include "DummyInstrInfo.h"
#include "DummyRegisterInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFrameInfo.h"

using namespace llvm;


StackOffset DummyFrameLowering::getFrameIndexReference(const MachineFunction &MF, int FI,
                                             Register &FrameReg) const {
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  const TargetSubtargetInfo &subtarget = MF.getSubtarget();

  FrameReg = subtarget.getRegisterInfo()->getFrameRegister(MF);
  // return StackOffset::getFixed(MFI.getObjectOffset(FI));
  return StackOffset::getFixed(MFI.getObjectOffset(FI) + MFI.getStackSize());
}
