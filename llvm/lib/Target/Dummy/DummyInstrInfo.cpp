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

#include "DummyInstrInfo.h"

#define GET_INSTRINFO_ENUM
#define GET_INSTRINFO_CTOR_DTOR
#include "DummyGenInstrInfo.inc"

using namespace llvm;

DummyInstrInfo::~DummyInstrInfo() {}
bool DummyInstrInfo::expandPostRAPseudo(MachineInstr &MI) const {
  MachineBasicBlock &MBB = *MI.getParent();
  MBB.erase(MI);
  
  return true;
}

unsigned DummyInstrInfo::getInstSizeInBytes(const MachineInstr &MI) const {
  return 8;
}

void DummyInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                          MachineBasicBlock::iterator MI, Register SrcReg,
                          bool IsKill, int FrameIndex,
                          const TargetRegisterClass *RC,
                          const TargetRegisterInfo *TRI) const {
  MachineFunction &MF = *MBB.getParent();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  int64_t Offsets = 0;
  Align SlotAlign = MFI.getLocalFrameMaxAlign();
  DebugLoc DL = MBB.findDebugLoc(MI);

  MachineMemOperand *MMO = MF.getMachineMemOperand(
      MachinePointerInfo::getFixedStack(MF, FrameIndex, Offsets), MachineMemOperand::MOStore,
      MFI.getObjectSize(FrameIndex), SlotAlign);

  auto Op = Dummy::L_ST_w;
  const MachineInstrBuilder II = BuildMI(MBB, MI, DL, get(Op))
                                  .addReg(SrcReg, getKillRegState(IsKill))
                                  .addFrameIndex(FrameIndex);
  II.addImm(0);
  II.addMemOperand(MMO);
}

void DummyInstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator MI,
                            Register DestReg, int FrameIndex,
                            const TargetRegisterClass *RC,
                            const TargetRegisterInfo *TRI) const {
  MachineFunction &MF = *MBB.getParent();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  int64_t Offsets = 0;
  Align SlotAlign = MFI.getLocalFrameMaxAlign();
  DebugLoc DL = MBB.findDebugLoc(MI);

  MachineMemOperand *MMO = MF.getMachineMemOperand(
      MachinePointerInfo::getFixedStack(MF, FrameIndex, Offsets), MachineMemOperand::MOLoad,
      MFI.getObjectSize(FrameIndex), SlotAlign);

  auto op = Dummy::L_LD_w;
  const MachineInstrBuilder II = BuildMI(MBB, MI, DL, get(op))
                                    .addReg(DestReg, getDefRegState(true))
                                    .addFrameIndex(FrameIndex);
  II.addImm(0);
  II.addMemOperand(MMO);
}

//什么是indirect branch
void DummyInstrInfo::insertIndirectBranch(MachineBasicBlock &MBB,
                            MachineBasicBlock &NewDestBB,
                            MachineBasicBlock &RestoreBB,
                            const DebugLoc &DL, int64_t BrOffset,
                            RegScavenger *RS) const {
                              

}

//这个是求取dest block吗？
MachineBasicBlock * DummyInstrInfo::getBranchDestBlock(const MachineInstr &MI) const {
  assert(MI.getDesc().isBranch() && "Unexpected opcode!");
  // The branch target is always the last operand.
  int NumOp = MI.getNumExplicitOperands();
  return MI.getOperand(NumOp - 1).getMBB();
}

//跳转offset暂时写成支持所有的
bool DummyInstrInfo::isBranchOffsetInRange(unsigned BranchOpc,
                                    int64_t BrOffset) const {
   return true;
}