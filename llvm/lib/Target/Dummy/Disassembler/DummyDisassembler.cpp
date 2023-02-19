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
#include "llvm/MC/TargetRegistry.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCParser/MCAsmLexer.h"
#include "llvm/MC/MCParser/MCAsmParser.h"
#include "llvm/MC/MCParser/MCAsmParserExtension.h"
#include "llvm/MC/MCParser/MCAsmParserUtils.h"
#include "llvm/MC/MCParser/MCParsedAsmOperand.h"
#include "llvm/MC/MCParser/MCTargetAsmParser.h"
#include "llvm/MC/MCDisassembler/MCDisassembler.h"
#include "llvm/MC/MCDecoderOps.h"
#include "DummySubtarget.h"
#include "MCTargetDesc/DummyMCTargetDesc.h"

using namespace llvm;

#define DEBUG_TYPE "dummydisassembler"

typedef MCDisassembler::DecodeStatus DecodeStatus;

namespace {

class DummyDisassembler : public MCDisassembler {
private:
  std::unique_ptr<MCInstrInfo const> const MCII;

public:
  DummyDisassembler(const MCSubtargetInfo &STI, MCContext &Ctx,
                  MCInstrInfo const *MCII)
      : MCDisassembler(STI, Ctx), MCII(MCII) {}

  DummyDisassembler() = delete;

  DecodeStatus getInstruction(MCInst &Instr, uint64_t &Size,
                              ArrayRef<uint8_t> Bytes, uint64_t Address,
                              raw_ostream &CStream) const override;
};
} // end anonymous namespace

template <int Bits>
static DecodeStatus DecodeSImm(llvm::MCInst &Inst, uint64_t Imm,
                               uint64_t Address, const void *Decoder) {
  if (Imm & ~((1LL << Bits) - 1))
    return MCDisassembler::Fail;

  // Imm is a signed immediate, so sign extend it.
  if (Imm & (1 << (Bits - 1)))
    Imm |= ~((1LL << Bits) - 1);

  Inst.addOperand(MCOperand::createImm(Imm));
  return MCDisassembler::Success;
}

template<int Bits>
static DecodeStatus DecodeUImm(llvm::MCInst &Inst, uint64_t Imm,
                               uint64_t Address, const void *Decoder) {
 if (Imm & ~((1LL << Bits) - 1))
     return MCDisassembler::Fail;

 Inst.addOperand(MCOperand::createImm(Imm));
 return MCDisassembler::Success;
}


DecodeStatus DummyDisassembler::getInstruction(MCInst &MI, uint64_t &Size,
                                             ArrayRef<uint8_t> Bytes,
                                             uint64_t Address,
                                             raw_ostream &CS) const {
  DecodeStatus Result = DecodeStatus::Success;
  FeatureBitset FeatureBits = STI.getFeatureBits();
  static bool CCEBit = false;
  static bool LCEBit = false;

  MI.print(llvm::errs());
  llvm::errs() << "\n";
  llvm::errs() << "Bytes size = " << Bytes.size() << "\n";
  for(uint8_t x : Bytes) {
    llvm::errs() << x << ":";
  }
  llvm::errs() << "\n";

  return Result;
}


static MCDisassembler *createDummyDisassembler(const Target &T,
                                             const MCSubtargetInfo &STI,
                                             MCContext &Ctx) {
  return new DummyDisassembler(STI, Ctx, T.createMCInstrInfo());
}

#include "DummyGenDisassemblerTables.inc"

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeDummyDisassembler() {
  // Register the disassembler for each target.
  TargetRegistry::RegisterMCDisassembler(getTheDummyTarget(),
                                         createDummyDisassembler);
}

