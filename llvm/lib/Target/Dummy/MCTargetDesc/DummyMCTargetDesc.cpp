//===- DirectXMCTargetDesc.cpp - DirectX Target Implementation --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains DirectX target initializer.
///
//===----------------------------------------------------------------------===//

#include "DummyMCTargetDesc.h"
#include "DummyContainerObjectWriter.h"
#include "TargetInfo/DummyTargetInfo.h"
#include "llvm/ADT/Triple.h"
#include "llvm/MC/LaneBitmask.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSchedule.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/Support/Compiler.h"
#include <iostream>
#include <memory>

using namespace llvm;

#define GET_REGINFO_MC_DESC
#include "DummyGenRegisterInfo.inc"

#define GET_INSTRINFO_MC_DESC
#include "DummyGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "DummyGenSubtargetInfo.inc"

namespace llvm {

// DXILInstPrinter is a null stub because DXIL instructions aren't printed.
// 汇编的打印
class DummyInstPrinter : public MCInstPrinter {
public:
  DummyInstPrinter(const MCAsmInfo &MAI, const MCInstrInfo &MII,
                   const MCRegisterInfo &MRI)
      : MCInstPrinter(MAI, MII, MRI) {}

  void printInstruction(const MCInst *MI, uint64_t Address, raw_ostream &O);
  void printInst(const MCInst *MI, uint64_t Address, StringRef Annot,
                 const MCSubtargetInfo &STI, raw_ostream &O) override {
    MI->print(llvm::errs());
    printInstruction(MI, Address, O);
    printAnnotation(O, Annot);
  }

  static const char *getRegisterName(unsigned RegNo);

  std::pair<const char *, uint64_t> getMnemonic(const MCInst *MI) override;
  void printRegName(raw_ostream &OS, unsigned RegNo) const {
    OS << StringRef(getRegisterName(RegNo)).lower();
  }

  void printOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O);
  void printMemRegImm12(const MCInst *MI, unsigned OpNo, raw_ostream &O) {
    const MCOperand & MO = MI->getOperand(OpNo);
    if (MO.isExpr()) {
      MO.getExpr()->print(O, &MAI);
      unsigned Disp = MI->getOperand(OpNo + 1).getImm();
      O << ", " << formatImm(Disp);
      return;
    }

    unsigned RegNo = MI->getOperand(OpNo).getReg();
    unsigned Disp = MI->getOperand(OpNo + 1).getImm();
    O << StringRef(getRegisterName(RegNo)).lower() << ", " << formatImm(Disp);
  }

private:
};

#include "DummyGenAsmWriter.inc"

void DummyInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                    raw_ostream &O) {
  MI->print(llvm::errs());
  const MCOperand &Op = MI->getOperand(OpNo);
  Op.print(llvm::errs());

  if (Op.isReg()) {
    printRegName(O, Op.getReg());
    return;
  }

  if (Op.isImm()) {
    O << formatImm(Op.getImm());
    return;
  }

  assert(Op.isExpr() && "unknown operand kind in printOperand");
  Op.getExpr()->print(O, &MAI);
}

class DummyMCCodeEmitter : public MCCodeEmitter {

  const MCInstrInfo &MCII;
  MCContext &CTX;

public:
  DummyMCCodeEmitter(const MCInstrInfo &mcii, MCContext &ctx)
      : MCII(mcii), CTX(ctx) {}

  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;
  unsigned getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                            SmallVectorImpl<MCFixup> &Fixups,
                            const MCSubtargetInfo &STI) const;
  unsigned getMemRI12Encoding(const MCInst &MI, unsigned OpNo,
                              SmallVectorImpl<MCFixup> &Fixups,
                              const MCSubtargetInfo &STI) const;
  template <int N>
  unsigned getMemRIEncoding(const MCInst &MI, unsigned OpIdx,
                            SmallVectorImpl<MCFixup> &Fixups,
                            const MCSubtargetInfo &STI) const;

  void EmitByte(unsigned char C, raw_ostream &OS) const { OS << (char)C; }

  void EmitConstant(uint32_t Val, unsigned Size, raw_ostream &OS) const {
    // Output the constant in little endian byte order.
    for (unsigned i = 0; i != Size; ++i) {
      EmitByte(Val & 255, OS);
      Val >>= 8;
    }
  }

  void encodeInstruction(const MCInst &MI, raw_ostream &OS,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override {
    llvm::errs() << "getNumOpcodes: " << MCII.getNumOpcodes() << "\n";
    const MCInstrDesc &Desc = MCII.get(MI.getOpcode());
    const uint64_t Binary64 = getBinaryCodeForInstr(MI, Fixups, STI);
    EmitConstant(Binary64, Desc.getSize(), OS);
  }
};

#include "DummyGenMCCodeEmitter.inc"

unsigned DummyMCCodeEmitter::getMachineOpValue(const MCInst &MI,
                                             const MCOperand &MO,
                                             SmallVectorImpl<MCFixup> &Fixups,
                                             const MCSubtargetInfo &STI) const {
  if (MO.isReg()) {
    return CTX.getRegisterInfo()->getEncodingValue(MO.getReg());
  }

  if (MO.isImm()) {
    return static_cast<unsigned>(MO.getImm());
  }

  const MCExpr *Expr = MO.getExpr();
  MCExpr::ExprKind Kind = Expr->getKind();
  if (Kind == MCExpr::Binary) {
    Expr = static_cast<const MCBinaryExpr*>(Expr)->getLHS();
    Kind = Expr->getKind();
  }

  if (Kind == MCExpr::Target) {
    assert(false && " DummyMCCodeEmitter::getMachineOpValue MCExpr::Target not supported \n ");
  }else if (Kind == MCExpr::Constant)
    return cast<MCConstantExpr>(Expr)->getValue();
  else {
    assert (Kind == MCExpr::SymbolRef);
  }

  return 0;
  
}

unsigned DummyMCCodeEmitter::getMemRI12Encoding(const MCInst &MI, unsigned OpNo,
                                            SmallVectorImpl<MCFixup> &Fixups,
                                            const MCSubtargetInfo &STI) const {
  return getMemRIEncoding<12>(MI, OpNo, Fixups, STI);
}


template <int N>
unsigned DummyMCCodeEmitter::getMemRIEncoding(const MCInst &MI, unsigned OpIdx,
                                          SmallVectorImpl<MCFixup> &Fixups,
                                          const MCSubtargetInfo &STI) const {

  static_assert(N < 32, "Not an acceptable instantiation.");

  constexpr unsigned Mask = ((1 << N) - 1);

  unsigned Reg = getMachineOpValue(MI, MI.getOperand(OpIdx), Fixups, STI);;
  int64_t  Imm = MI.getOperand(OpIdx + 1).getImm();

  assert((((Imm & ~Mask) == 0) || ((Imm & ~Mask) == ~Mask)) &&
         "Displacement value is out of instruction capability.");

  return ((Imm & Mask) << 5) | Reg;
}

class DummyAsmBackend : public MCAsmBackend {
  uint8_t OSABI;
public:
  DummyAsmBackend(const MCSubtargetInfo &STI, uint8_t _OSABI) : MCAsmBackend(support::little) , OSABI(_OSABI){}
  ~DummyAsmBackend() override = default;

  void applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                  const MCValue &Target, MutableArrayRef<char> Data,
                  uint64_t Value, bool IsResolved,
                  const MCSubtargetInfo *STI) const override {}

  std::unique_ptr<MCObjectTargetWriter>
  createObjectTargetWriter() const override {
    return createDummyContainerTargetObjectWriter(OSABI);
  }

  unsigned getNumFixupKinds() const override { return 0; }

  bool writeNopData(raw_ostream &OS, uint64_t Count,
                    const MCSubtargetInfo *STI) const override {
    return true;
  }

  bool fixupNeedsRelaxation(const MCFixup &Fixup, uint64_t Value,
                            const MCRelaxableFragment *DF,
                            const MCAsmLayout &Layout) const override {
    return true;
  }
};

class DummyMCAsmInfo : public MCAsmInfo {
public:
  explicit DummyMCAsmInfo(const Triple &TT, const MCTargetOptions &Options)
      : MCAsmInfo() {}
};

} // namespace llvm

static MCInstPrinter *createDummyMCInstPrinter(const Triple &T,
                                               unsigned SyntaxVariant,
                                               const MCAsmInfo &MAI,
                                               const MCInstrInfo &MII,
                                               const MCRegisterInfo &MRI) {
  if (SyntaxVariant == 0)
    return new DummyInstPrinter(MAI, MII, MRI);
  return nullptr;
}

MCCodeEmitter *createDummyMCCodeEmitter(const MCInstrInfo &MCII,
                                        MCContext &Ctx) {
  return new DummyMCCodeEmitter(MCII, Ctx);
}

MCAsmBackend *createDummyMCAsmBackend(const Target &T,
                                      const MCSubtargetInfo &STI,
                                      const MCRegisterInfo &MRI,
                                      const MCTargetOptions &Options) {
  const uint8_t ABI = MCELFObjectTargetWriter::getOSABI(STI.getTargetTriple().getOS());
  return new DummyAsmBackend(STI, ABI);
}

static MCSubtargetInfo *
createDummyMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  std::cout << "createDummyMCSubtargetInfo  CPU:" << CPU.str()
            << " FS:" << FS.str() << std::endl;
  return createDummyMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

static MCRegisterInfo *createDummyMCRegisterInfo(const Triple &Triple) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitDummyMCRegisterInfo(X, Dummy::LR);
  return X;
}

static MCInstrInfo *createDummyMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo(); 
  InitDummyMCInstrInfo(X);
  return X; 
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeDummyTargetMC() {
  Target &T = getTheDummyTarget();
  RegisterMCAsmInfo<DummyMCAsmInfo> X(T);
  TargetRegistry::RegisterMCInstrInfo(T, createDummyMCInstrInfo);
  TargetRegistry::RegisterMCInstPrinter(T, createDummyMCInstPrinter);
  TargetRegistry::RegisterMCRegInfo(T, createDummyMCRegisterInfo);
  TargetRegistry::RegisterMCSubtargetInfo(T, createDummyMCSubtargetInfo);
  TargetRegistry::RegisterMCCodeEmitter(T, createDummyMCCodeEmitter);
  TargetRegistry::RegisterMCAsmBackend(T, createDummyMCAsmBackend);
}