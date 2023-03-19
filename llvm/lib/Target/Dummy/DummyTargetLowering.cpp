//===-- DirectXTargetLowering.h - Define DX TargetLowering  -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the DirectX specific subclass of TargetLowering.
//
//===----------------------------------------------------------------------===//

#include "DummyTargetLowering.h"
#include "DummySubtarget.h"
#include "DummyTargetMachine.h"

using namespace llvm;

DummyTargetLowering::DummyTargetLowering(const DummyTargetMachine &TM,
                                         const DummySubtarget &STI)
    : TargetLowering(TM), Subtarget(STI) {
  addRegisterClass(MVT::i32, &Dummy::GRegsRegClass);
  computeRegisterProperties(Subtarget.getRegisterInfo());

  setStackPointerRegisterToSaveRestore(Dummy::SP);

  for (auto T : {MVT::i32, MVT::i64}) {
    // Expand unavailable integer operations.
    for (auto Op :
         {ISD::BSWAP,
          ISD::SMUL_LOHI, ISD::UMUL_LOHI,
          ISD::MULHU, ISD::MULHS,
          ISD::SDIV, ISD::UDIV,
          ISD::SREM, ISD::UREM,
          ISD::SDIVREM, ISD::UDIVREM,
          ISD::SHL_PARTS, ISD::SRA_PARTS, ISD::SRL_PARTS,
          ISD::ROTL, ISD::ROTR,
          ISD::ADDC, ISD::ADDE, ISD::SUBC, ISD::SUBE,
          ISD::SELECT, ISD::CTLZ, ISD::CTPOP}) {
      setOperationAction(Op, T, Expand);
    }
    setOperationAction(ISD::SETCC, T, Expand);
  }
}

#include "DummyGenCallingConv.inc"

SDValue DummyTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &dl,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  MachineFunction &MF = DAG.getMachineFunction();
  MachineRegisterInfo &RegInfo = MF.getRegInfo();

  // Assign locations to all of the incoming arguments.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(), ArgLocs,
                 *DAG.getContext());
  llvm::errs() << "xujing Ins: " << Ins.size() << "\n";
  CCInfo.AnalyzeFormalArguments(Ins, CC_DUMMY);
  llvm::errs() << "xujing ArgLocs: " << ArgLocs.size() << "\n";
  for (auto &VA : ArgLocs) {
    if (VA.isRegLoc()) {
      // Arguments passed in registers
      MVT RegVT = VA.getLocVT();
      const TargetRegisterClass *RC = getRegClassFor(RegVT);
      assert(RC && "parameter type is not supported");
      const unsigned VReg = RegInfo.createVirtualRegister(RC);
      RegInfo.addLiveIn(VA.getLocReg(), VReg);
      SDValue ArgIn = DAG.getCopyFromReg(Chain, dl, VReg, RegVT);

      InVals.push_back(ArgIn);
      continue;
    } else {
      assert(VA.isMemLoc() &&
             "Can only pass arguments as either registers or via the stack");

      // Arguments passed in stack
      const unsigned Offset = VA.getLocMemOffset();
      unsigned ObjectSize = VA.getLocVT().getStoreSize();
      const int FI =
          MF.getFrameInfo().CreateFixedObject(ObjectSize, Offset, true);
      SDValue FIPtr = DAG.getTargetFrameIndex(FI, VA.getValVT());
      // EVT PtrTy = getPointerTy(DAG.getDataLayout());
      // SDValue FIPtr = DAG.getFrameIndex(FI, PtrTy);

      SDValue Load =
          DAG.getLoad(VA.getValVT(), dl, Chain, FIPtr, MachinePointerInfo());

      InVals.push_back(Load);
    }
  }

  return Chain;
}

SDValue
DummyTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                 bool isVarArg,
                                 const SmallVectorImpl<ISD::OutputArg> &Outs,
                                 const SmallVectorImpl<SDValue> &OutVals,
                                 const SDLoc &dl, SelectionDAG &DAG) const {
  // CCValAssign - represent the assignment of
  // the return value to a location
  SmallVector<CCValAssign, 16> RVLocs;

  // CCState - Info about the registers and stack slot.
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());

  CCInfo.AnalyzeReturn(Outs, CC_DUMMY_RET);

  SDValue Flag;
  SmallVector<SDValue, 4> RetOps(1, Chain);

  // Copy the result values into the output registers.
  for (unsigned i = 0, e = RVLocs.size(); i < e; ++i) {
    CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Can only return in registers!");

    Chain = DAG.getCopyToReg(Chain, dl, VA.getLocReg(), OutVals[i], Flag);

    Flag = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
  }

  RetOps[0] = Chain; // Update chain.

  // Add the flag if we have it.
  if (Flag.getNode()) {
    RetOps.push_back(Flag);
  }

  return DAG.getNode(DUMMYISD::RET_FLAG, dl, MVT::Other, RetOps);
}

MachineBasicBlock *DummyTargetLowering::ExpandSelectCC(MachineInstr &MI,
                                                     MachineBasicBlock *BB,
                                                     unsigned BranchOp) const {
  const DummyInstrInfo *TII = (DummyInstrInfo *)(Subtarget.getInstrInfo());
  DebugLoc dl = MI.getDebugLoc();

  // To "insert" a SELECT_CC instruction, we actually have to insert the
  // triangle control-flow pattern. The incoming instruction knows the
  // destination vreg to set, the condition code register to branch on, the
  // true/false values to select between, and the condition code for the branch.
  //
  // We produce the following control flow:
  //     ThisMBB
  //     |  \
  //     |  IfFalseMBB
  //     | /
  //    SinkMBB
  const BasicBlock *LLVM_BB = BB->getBasicBlock();
  MachineFunction::iterator It = ++BB->getIterator();

  MachineBasicBlock *ThisMBB = BB;
  MachineFunction *F = BB->getParent();
  MachineBasicBlock *IfFalseMBB = F->CreateMachineBasicBlock(LLVM_BB);
  MachineBasicBlock *SinkMBB = F->CreateMachineBasicBlock(LLVM_BB);
  F->insert(It, IfFalseMBB);
  F->insert(It, SinkMBB);

  // Transfer the remainder of ThisMBB and its successor edges to SinkMBB.
  SinkMBB->splice(SinkMBB->begin(), ThisMBB,
                  std::next(MachineBasicBlock::iterator(MI)), ThisMBB->end());
  SinkMBB->transferSuccessorsAndUpdatePHIs(ThisMBB);

  // Set the new successors for ThisMBB.
  ThisMBB->addSuccessor(IfFalseMBB);
  ThisMBB->addSuccessor(SinkMBB);

  unsigned LHS = MI.getOperand(1).getReg();
  unsigned RHS = MI.getOperand(2).getReg();

  MachineRegisterInfo &RegInfo = MI.getMF()->getRegInfo();

  {
    BuildMI(ThisMBB, dl, TII->get(BranchOp))
                          .addReg(LHS)
                          .addReg(RHS)
                          .addMBB(SinkMBB);
  }

  // IfFalseMBB just falls through to SinkMBB.
  IfFalseMBB->addSuccessor(SinkMBB);

  // %Result = phi [ %TrueValue, ThisMBB ], [ %FalseValue, IfFalseMBB ]
  BuildMI(*SinkMBB, SinkMBB->begin(), dl, TII->get(Dummy::PHI),
          MI.getOperand(0).getReg())
      .addReg(MI.getOperand(3).getReg())
      .addMBB(ThisMBB)
      .addReg(MI.getOperand(4).getReg())
      .addMBB(IfFalseMBB);
  MI.eraseFromParent(); // The pseudo instruction is gone now.
  return SinkMBB;
}

MachineBasicBlock *
DummyTargetLowering::EmitInstrWithCustomInserter(MachineInstr &MI,
                                               MachineBasicBlock *BB) const {
  unsigned BranchOp = 0;
  switch (MI.getOpcode()) {
  default:
    llvm_unreachable("Unknown SELECT_CC pseudo!");
  case Dummy::SEL20_EQ:
    BranchOp = Dummy::L_BEQ;
    break;
  case Dummy::SEL20_NE:
    BranchOp = Dummy::L_BNE;
    break;
  case Dummy::SEL20_LT:
    BranchOp = Dummy::L_BLTS;
    break;
  case Dummy::SEL20_ULT:
    BranchOp = Dummy::L_BLTU;
    break;
  case Dummy::SEL20_GE:
    BranchOp =  Dummy::L_BGES ;
    break;
  case Dummy::SEL20_UGE:
    BranchOp =  Dummy::L_BGEU;
    break;
  }

  return ExpandSelectCC(MI, BB, BranchOp);
}