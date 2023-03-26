//===-- DirectXAsmPrinter.cpp - DirectX assembly writer --------*- C++ -*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains AsmPrinters for the DirectX backend.
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/DummyTargetInfo.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/SectionKind.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Target/TargetLoweringObjectFile.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

namespace {

// The DXILAsmPrinter is mostly a stub because DXIL is just LLVM bitcode which
// gets embedded into a DXContainer file.
class DummyAsmPrinter : public AsmPrinter {
public:
  explicit DummyAsmPrinter(TargetMachine &TM,
                           std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)) {}
  virtual void emitInstruction(const MachineInstr *MI) override {
    MCInst *ResultMCInstr = new MCInst();
    ResultMCInstr->setOpcode(MI->getOpcode());
    for (auto &MO : MI->operands()) {
      MachineOperand::MachineOperandType MOTy = MO.getType();
      
      if (MOTy == MachineOperand::MO_Immediate) {
        ResultMCInstr->addOperand(MCOperand::createImm(MO.getImm()));
        continue;
      }

      llvm::errs() << "MOTy:" << MOTy << "\n";

      if (MOTy == MachineOperand::MO_MachineBasicBlock) {
        const MCSymbolRefExpr *expr = MCSymbolRefExpr::create(MO.getMBB()->getSymbol(), MO.getMBB()->getParent()->getContext());
        ResultMCInstr->addOperand(MCOperand::createExpr(expr));
        continue;
      }

      if (MOTy == MachineOperand::MO_GlobalAddress) {        
        const MCSymbolRefExpr *expr = MCSymbolRefExpr::create(StringRef(MO.getGlobal()->getGlobalIdentifier()), MCSymbolRefExpr::VK_None, MI->getParent()->getParent()->getContext());
        ResultMCInstr->addOperand(MCOperand::createExpr(expr));
        continue;
      }
      // if (MO.isReg() && MO.isImplicit()) {
      //   continue;
      // }
      const MCOperand MCOp = MCOperand::createReg(MO.getReg());

      if (MCOp.isValid()) {
        ResultMCInstr->addOperand(MCOp);
      }
    }
    OutStreamer->emitInstruction(*ResultMCInstr, getSubtargetInfo());
  }

  StringRef getPassName() const override { return "Dummy Assembly Printer"; }
  void emitGlobalVariable(const GlobalVariable *GV) override;
  bool runOnMachineFunction(MachineFunction &MF) override {
    return AsmPrinter::runOnMachineFunction(MF);
  }
};
} // namespace

void DummyAsmPrinter::emitGlobalVariable(const GlobalVariable *GV) {
  // If there is no initializer, or no explicit section do nothing
  if (!GV->hasInitializer() || GV->hasImplicitSection() || !GV->hasSection())
    return;
  // Skip the LLVM metadata
  if (GV->getSection() == "llvm.metadata")
    return;
  SectionKind GVKind = TargetLoweringObjectFile::getKindForGlobal(GV, TM);
  MCSection *TheSection = getObjFileLowering().SectionForGlobal(GV, GVKind, TM);
  OutStreamer->switchSection(TheSection);
  emitGlobalConstant(GV->getParent()->getDataLayout(), GV->getInitializer());
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeDummyAsmPrinter() {
  RegisterAsmPrinter<DummyAsmPrinter> X(getTheDummyTarget());
  llvm::errs() << "register DummyAsmPrinter ok";
}
