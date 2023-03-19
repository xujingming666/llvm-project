//===-- DirectXInstrInfo.h - Define InstrInfo for DirectX -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the DirectX specific subclass of TargetInstrInfo.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_DUMMY_DUMMYINSTRINFO_H
#define LLVM_DUMMY_DUMMYINSTRINFO_H

#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/CodeGen/MachineFrameInfo.h"


#define GET_INSTRINFO_HEADER
#include "DummyGenInstrInfo.inc"

namespace llvm {
struct DummyInstrInfo : public DummyGenInstrInfo {
  explicit DummyInstrInfo() : DummyGenInstrInfo(0, 0, 0, 0) {}

  bool expandPostRAPseudo(MachineInstr &MI) const override;
  unsigned getInstSizeInBytes(const MachineInstr &MI) const override;
  ~DummyInstrInfo() override;

  void storeRegToStackSlot(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator MI, Register SrcReg,
                           bool IsKill, int FrameIndex,
                           const TargetRegisterClass *RC,
                           const TargetRegisterInfo *TRI) const override;
  void loadRegFromStackSlot(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator MI,
                            Register DestReg, int FrameIndex,
                            const TargetRegisterClass *RC,
                            const TargetRegisterInfo *TRI) const override;
  void insertIndirectBranch(MachineBasicBlock &MBB,
                            MachineBasicBlock &NewDestBB,
                            MachineBasicBlock &RestoreBB,
                            const DebugLoc &DL, int64_t BrOffset = 0,
                            RegScavenger *RS = nullptr) const override;
  virtual MachineBasicBlock * getBranchDestBlock(const MachineInstr &MI) const override;
  virtual bool isBranchOffsetInRange(unsigned BranchOpc,
                                    int64_t BrOffset) const override;
};
} // namespace llvm

#endif // LLVM_DUMMY_DUMMYINSTRINFO_H
