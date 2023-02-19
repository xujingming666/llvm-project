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
#include "DummySubtarget.h"
#include "MCTargetDesc/DummyMCTargetDesc.h"

using namespace llvm;

#define DEBUG_TYPE "asm-parser"

namespace {

using RegType = uint32_t;
using ImmType = const MCExpr*;  // Imm could be a relocation
using LblType = const MCExpr*;  // Label is a expression

struct DummyOperand : public MCParsedAsmOperand {
  enum KindTy {
    Token,
    Register,
    Immediate,
    Memory,       // addresses
    ThreadAddr,   // Thread Address in Group & Increment
    Label,
    Predicate,    // SIP-2.0 Predicate Reg
  } Kind;

  struct RegOp {
    RegType RegNum;
  };

  struct ImmOp {
    ImmType Val;
  };

  struct LblOp {
    LblType Val;
  };

  struct PredOp {
    unsigned Val;
  };

  struct MemOp {
    RegType BaseReg;
    int64_t Disp;
  };

  struct TAgrpOp {
    RegType GroupReg;
    RegType IncReg;
  };

  using MemType = MemOp;
  using TAgrpType = TAgrpOp;
  using PredType = PredOp;

  SMLoc StartLoc, EndLoc;
  union {
    StringRef Tok;
    RegOp Reg;
    ImmOp Imm;
    MemOp Mem;
    TAgrpOp TAgrp;
    LblOp Lbl;
    PredOp Pred;
  };

public:
  DummyOperand(KindTy K) : MCParsedAsmOperand(), Kind(K) {}
  DummyOperand(const DummyOperand &o) : MCParsedAsmOperand() {
    Kind = o.Kind;
    StartLoc = o.StartLoc;
    EndLoc = o.EndLoc;
    switch (Kind) {
    case Register:
      Reg = o.Reg;
      break;
    case Immediate:
      Imm = o.Imm;
      break;
    case Label:
      Lbl = o.Lbl;
      break;
    case Token:
      Tok = o.Tok;
      break;
    case ThreadAddr:
      TAgrp = o.TAgrp;
      break;
    case Memory:
      Mem = o.Mem;
      break;
    case Predicate:
      Pred = o.Pred;
      break;
    default:
      assert(false && "AsmParser: Unexpected operand kind");
      break;
    }
  }

  bool isToken() const override { return Kind == Token; }
  bool isReg() const override { return Kind == Register; }
  bool isImm() const override { return Kind == Immediate; }
  bool isMem() const override { return Kind == Memory; }
  template<int Low, int High> bool isImmInRange() const {
    return true;
  }
  template <int Bits> bool isAddrDisp() const {
    return true;
  }
  bool isBranchTarget() const { return true;}
  bool isMemBase() const { return true;}

    /// getStartLoc - Gets location of the first token of this operand
  SMLoc getStartLoc() const override { return StartLoc; }
  /// getEndLoc - Gets location of the last token of this operand
  SMLoc getEndLoc() const override { return EndLoc; }

  unsigned getReg() const override {
    assert(Kind == Register && "Invalid type access!");
    return Reg.RegNum;
  }

  ImmType getImm() const {
    assert(Kind == Immediate && "Invalid type access!");
    return Imm.Val;
  }

  const MemType getMem() const {
    assert(Kind == Memory && "Invalid type access!");
    return Mem;
  }
  
  LblType getLbl() const {
    assert(Kind == Label && "Invalid type access!");
    return Lbl.Val;
  }

  StringRef getToken() const {
    return Tok;
  }

  static std::unique_ptr<DummyOperand> createToken(StringRef Str, SMLoc S) {
    auto Op = std::make_unique<DummyOperand>(Token);
    Op->Tok = Str;
    Op->StartLoc = S;
    Op->EndLoc = S;
    return Op;
  }

  static std::unique_ptr<DummyOperand> createPredOp(unsigned Val, SMLoc S,
                                              SMLoc E) {
    auto Op = std::make_unique<DummyOperand>(Predicate);
    Op->Pred.Val = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<DummyOperand> createReg(RegType RegNo, SMLoc S,
                                                 SMLoc E) {
    auto Op = std::make_unique<DummyOperand>(Register);
    Op->Reg.RegNum = RegNo;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<DummyOperand> createImm(ImmType Val, SMLoc S,
                                                 SMLoc E) {
    auto Op = std::make_unique<DummyOperand>(Immediate);
    Op->Imm.Val = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<DummyOperand> createLbl(LblType Val,
                                               SMLoc S, SMLoc E) {
    auto Op = std::make_unique<DummyOperand>(Label);
    Op->Lbl.Val = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<DummyOperand>
  createMem(RegType Reg, int64_t Imm, SMLoc S, SMLoc E) {
    auto Op = std::make_unique<DummyOperand>(Memory);
    Op->Mem.BaseReg = Reg;
    Op->Mem.Disp = Imm;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  void addRegOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    Inst.addOperand(MCOperand::createReg(getReg()));
  }

  void addImmOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    const MCExpr *Expr = getImm();
    if (auto *CE = dyn_cast<MCConstantExpr>(Expr)) {
      Inst.addOperand(MCOperand::createImm(CE->getValue()));
    } else {
      Inst.addOperand(MCOperand::createExpr(Expr));
    }
  }
  
  void addBranchTargetOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    if (Kind == Label) {
      Inst.addOperand(MCOperand::createExpr(getLbl()));
    } else if (Kind == Immediate) {
      Inst.addOperand(MCOperand::createExpr(getImm()));
    }
  }
  template<int Bits>
  void addMemoryOperands(MCInst &Inst, unsigned N) const {
    assert(N == 2 && "Invalid number of operands!");
    assert(Kind == Memory && "Not a memory operand!");

    Inst.addOperand(MCOperand::createReg(getMem().BaseReg));
    Inst.addOperand(MCOperand::createImm(SignExtend64<Bits>(getMem().Disp)));
  }

  template <int Bits> void addAddrDispOperands(MCInst &Inst, unsigned N) const {
    addMemoryOperands<Bits>(Inst, N);
  }

  void print(raw_ostream &OS) const override {
    switch (Kind) {
    case Immediate:
      OS << getImm();
      break;
    case Register:
      OS << "<register x" << getReg() << ">";
      break;
    case Memory:
      OS << "<memory "
         << " base:" << getMem().BaseReg << ", disp: " << getMem().Disp;
      OS << ">";
      break;
    case Label:
      OS << "<label: " << getLbl() << ">";
      break;
    case Token:
      OS << "'" << getToken() << "'";
      break;
    }
  }
};

class DummyAsmParser : public MCTargetAsmParser {
public:
  enum DTUMatchResultTy {
    Match_Dummy = FIRST_TARGET_MATCH_RESULT_TY,
    #define GET_OPERAND_DIAGNOSTIC_TYPES
    #include "DummyGenAsmMatcher.inc"
    #undef GET_OPERAND_DIAGNOSTIC_TYPES
  };

  DummyAsmParser(const MCSubtargetInfo &STI, MCAsmParser &Parser,
               const MCInstrInfo &MII, const MCTargetOptions &Options)
    : MCTargetAsmParser(Options, STI, MII) {
        llvm::errs() << " DummyAsmParser create ok \n";
  }

  SMLoc getLoc() const { return getParser().getTok().getLoc(); }
  bool MatchAndEmitInstruction(SMLoc IDLoc, unsigned &Opcode,
                              OperandVector &Operands, MCStreamer &Out,
                              uint64_t &ErrorInfo,
                              bool MatchingInlineAsm) override {
    auto CurrentOperand = static_cast<DummyOperand &>(*Operands[0]);
    assert(CurrentOperand.isToken() && "instruction should start with token \n");
    
    MCInst *Inst = new (getContext()) MCInst;
    Inst->setLoc(IDLoc);

    unsigned MatchResult =
      MatchInstructionImpl(Operands, *Inst, ErrorInfo, MatchingInlineAsm);

    switch (MatchResult) {
      default:
        if (MatchResult > FIRST_TARGET_MATCH_RESULT_TY) {
          SMLoc ErrorLoc = IDLoc;
          if (ErrorInfo != ~0U) {
            ErrorLoc = ((DummyOperand &)*Operands[ErrorInfo]).getStartLoc();
            if (ErrorLoc == SMLoc())
              ErrorLoc = IDLoc;
          }
          return Error(ErrorLoc, "invalid match for instruction");
        }
        break;
      case Match_Success: {
        LLVM_DEBUG(llvm::errs() << "[Asm Parser] Matched: " << Inst << "\n");
        Inst->print(llvm::errs());
        llvm::errs() << "\n";
        Out.emitInstruction(*Inst, getSTI());
        return false;
      }
      case Match_MissingFeature:
        return Error(IDLoc, "instruction use requires an option to be enabled");
      case Match_MnemonicFail:
        return Error(IDLoc, "unrecognized instruction mnemonic");
      case Match_InvalidOperand: {
        SMLoc ErrorLoc = IDLoc;
        if (ErrorInfo != ~0U) {
          if (ErrorInfo >= Operands.size())
            return Error(ErrorLoc, "too few operands for instruction");

          ErrorLoc = ((DummyOperand &)*Operands[ErrorInfo]).getStartLoc();
          if (ErrorLoc == SMLoc())
            ErrorLoc = IDLoc;
        }
        return Error(ErrorLoc, "invalid operand for instruction");
      }
    }

    llvm_unreachable("Unknown match type detected!");
  }

  bool ParseInstruction(ParseInstructionInfo &Info, StringRef Name,
                                SMLoc NameLoc, OperandVector &Operands) override;
  bool ParseDirective(AsmToken DirectiveID) override { return true; }
  OperandMatchResultTy tryParseRegister(unsigned &RegNo, SMLoc &StartLoc, SMLoc &EndLoc) override;
  bool ParseRegister(unsigned &RegNo, SMLoc &StartLoc, SMLoc &EndLoc) override;
  
  OperandMatchResultTy parseMemory(OperandVector &Operands);
  OperandMatchResultTy parseRegister(OperandVector &Operands);
  OperandMatchResultTy parseImmediate(OperandVector &Operands);
  OperandMatchResultTy parseLabel(OperandVector &Operands);
  bool parseOperand(OperandVector &Operands);

  #define GET_ASSEMBLER_HEADER
  #include "DummyGenAsmMatcher.inc"
};
} // namespace

#define GET_REGISTER_MATCHER
#define GET_MATCHER_IMPLEMENTATION
#include "DummyGenAsmMatcher.inc"

OperandMatchResultTy
DummyAsmParser::tryParseRegister(unsigned &RegNo, SMLoc &StartLoc, SMLoc &EndLoc) { 
  RegNo = Dummy::NoRegister;

  switch (getLexer().getKind()) {
    default:
      break;
    case AsmToken::Identifier:
      std::string Name = getLexer().getTok().getString().lower();
      RegNo = MatchRegisterName(Name);
      break;
  }

  const AsmToken &Tok = getParser().getTok();
  StartLoc = Tok.getLoc();
  EndLoc = Tok.getEndLoc();

  if (RegNo == Dummy::NoRegister)
    return MatchOperand_NoMatch;
  return MatchOperand_Success;
}

bool DummyAsmParser::ParseRegister(unsigned &RegNo, SMLoc &StartLoc,
                            SMLoc &EndLoc) { 
  if (tryParseRegister(RegNo, StartLoc, EndLoc) == MatchOperand_NoMatch) {
    return Error(StartLoc, "invalid register name");
  }

  getParser().Lex(); // Eat identifier token.
  return false;
}

bool DummyAsmParser::ParseInstruction(ParseInstructionInfo &Info, StringRef Name,
                              SMLoc NameLoc, OperandVector &Operands) {
  Operands.push_back(DummyOperand::createToken(Name, NameLoc));
  
  if (getLexer().is(AsmToken::EndOfStatement))
    return false;

  // Parse first operand
  if (parseOperand(Operands))
    return true;

  // Parse until end of statement, consuming commas between operands
  while (getLexer().is(AsmToken::Comma)) {
    // Consume comma token
    getLexer().Lex();

    // Parse next operand
    if (parseOperand(Operands))
      return true;
  }

  if (getLexer().isNot(AsmToken::EndOfStatement)) {
    SMLoc Loc = getLexer().getLoc();
    getParser().eatToEndOfStatement();
    return Error(Loc, "unexpected token");
  }

  getParser().Lex(); // Consume the EndOfStatement.

  return false;
}

OperandMatchResultTy DummyAsmParser::parseMemory(OperandVector &Operands) {
  const auto &Lexer = getLexer();
  auto &Parser = getParser();

  if (Parser.getTok().isNot(AsmToken::LBrac)) {
    return MatchOperand_ParseFail;
  }

  Parser.Lex(); // Eat '['
  Operands.push_back(DummyOperand::createToken("[", getLoc()));

  RegType RegNo;
  int64_t Imm = 0;
  RegType XACCNo = Dummy::NoRegister;

  SMLoc StartLoc;
  SMLoc EndLoc;

  if (tryParseRegister(RegNo, StartLoc, EndLoc) == MatchOperand_NoMatch) {
    Error(getLoc(), "expecting register");
    return MatchOperand_ParseFail;
  }

  Parser.Lex(); // Eat reg

  if (Lexer.is(AsmToken::Comma)) { // [rn, imm] or [rn, xaccm]
    Parser.Lex(); // eat comma
    if (tryParseRegister(XACCNo, StartLoc, EndLoc) == MatchOperand_NoMatch) {
      if (Parser.parseAbsoluteExpression(Imm)) {
        Error(getLoc(), "expecting register or imm");
        return MatchOperand_ParseFail;
      }
    } else
      Parser.Lex(); // Eat the token
  }

  if (Lexer.isNot(AsmToken::RBrac)) {
    Error(getLoc(), "expected ']'");
    return MatchOperand_ParseFail;
  }

  Parser.Lex(); // Eat ']'

  SMLoc S = getLoc();
  SMLoc E = SMLoc::getFromPointer(S.getPointer() - 1);
  Operands.push_back(DummyOperand::createMem(RegNo, Imm, S, E));
  Operands.push_back(DummyOperand::createToken("]", getLoc()));

  return MatchOperand_Success;
}

OperandMatchResultTy DummyAsmParser::parseRegister(OperandVector &Operands) {
  RegType RegNo;
  SMLoc StartLoc;
  SMLoc EndLoc;

  if (tryParseRegister(RegNo, StartLoc, EndLoc) == MatchOperand_NoMatch)
    return MatchOperand_NoMatch;

  SMLoc S = getLoc();
  SMLoc E = SMLoc::getFromPointer(S.getPointer() - 1);

  Operands.push_back(DummyOperand::createReg(RegNo, S, E));

  getLexer().Lex(); // Eat the token

  return MatchOperand_Success;
}

OperandMatchResultTy DummyAsmParser::parseImmediate(OperandVector &Operands) {
  auto & Parser = getParser();
  SMLoc S = getLoc();
  SMLoc E = SMLoc::getFromPointer(S.getPointer() - 1);
  const MCExpr *Expr;

  switch (getLexer().getKind()) {
    case AsmToken::Identifier:
    default:
      return MatchOperand_NoMatch;
    case AsmToken::Minus:
    case AsmToken::Integer:
      if (Parser.parseExpression(Expr))
        return MatchOperand_ParseFail;
      break;
  }

  Operands.push_back(DummyOperand::createImm(Expr, S, E));
  return MatchOperand_Success;
}

OperandMatchResultTy DummyAsmParser::parseLabel(OperandVector &Operands) {
  const auto & Lexer = getLexer();

  if (Lexer.isNot(AsmToken::Identifier)) {
    return MatchOperand_ParseFail;
  }

  StringRef Identifier;
  if (getParser().parseIdentifier(Identifier)) { // eat on success
    return MatchOperand_ParseFail;
  }

  // Create a symbol for the label
  MCSymbol *Sym = getContext().getOrCreateSymbol(Identifier);
  const MCExpr *Res =
    MCSymbolRefExpr::create(Sym, MCSymbolRefExpr::VK_None, getContext());

  SMLoc S = getLoc();
  SMLoc E = SMLoc::getFromPointer(S.getPointer() - 1);

  Operands.push_back(DummyOperand::createLbl(Res, S, E));

  return MatchOperand_Success;
}

bool DummyAsmParser::parseOperand(OperandVector &Operands) {
  if (parseMemory(Operands) == MatchOperand_Success) {
    return false;
  }

  // Attempt to parse token as register
  if (parseRegister(Operands) == MatchOperand_Success) {
    return false;
  }

  if (parseImmediate(Operands) == MatchOperand_Success) {
    return false;
  }

  // Attempt to parse token as a label
  if (parseLabel(Operands) == MatchOperand_Success) {
    return false;
  }

  return Error(getLoc(), "unknown operand");
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeDummyAsmParser() {
  RegisterMCAsmParser<DummyAsmParser> X(getTheDummyTarget());
  llvm::errs() << "register DummyAsmParser ok";
}
