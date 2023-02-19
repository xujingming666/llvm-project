//===----------------------------------------------------------------------===//
// Operand Definitions.
//===----------------------------------------------------------------------===//

def DummyBranchAsmOperand : AsmOperandClass {
  let Name = "BranchOp";
  let PredicateMethod = "isBranchTarget";
  let RenderMethod = "addBranchTargetOperands";
}

def b_target : Operand<OtherVT> {
  let ParserMatchClass = DummyBranchAsmOperand;
}

class AsmImmRange<int Low, int High> : AsmOperandClass {
  let Name = "Imm" # !subst("-", "n", !cast<string>(Low)) # "_" # High;
  let DiagnosticType = "InvalidImm" # !subst("-", "n", !cast<string>(Low)) # "_" # High;
  let RenderMethod = "addImmOperands";
  let PredicateMethod = "isImmInRange<" # Low # "," # High # ">";
}

multiclass Immediate<int N, code s_pred, code u_pred> {
  def DTUsimm # N : Operand<i32>, ImmLeaf<i32, s_pred> {
    defvar high = !shl(1, !add(N, -1));
    let ParserMatchClass = AsmImmRange<!mul(-1, high), high>;
    let DecoderMethod = "DecodeSImm<" # N # ">";
  }
  def DTUimm # N  : Operand<i32>, ImmLeaf<i32, u_pred> {
    let ParserMatchClass = AsmImmRange<0, !shl(1, N)>;
    let DecoderMethod = "DecodeUImm<" # N # ">";
  }
}

defm "" : Immediate<1, [{ return Imm < 1 && Imm >= -1; }],
                       [{ return Imm < 2 && Imm >= 0;  }]>;
defm "" : Immediate<2, [{ return Imm < 2 && Imm >= -2; }],
                       [{ return Imm < 4 && Imm >= 0; }]>;
defm "" : Immediate<3, [{ return Imm < 4 && Imm >= -4; }],
                       [{ return Imm < 8 && Imm >= 0; }]>;
defm "" : Immediate<4, [{ return Imm < 8 && Imm >= -8; }],
                       [{ return Imm < 16 && Imm >= 0; }]>;
defm "" : Immediate<5, [{ return Imm < 16 && Imm >= -16; }],
                       [{ return Imm < 32 && Imm >= 0; }]>;
defm "" : Immediate<6, [{ return Imm < 32 && Imm >= -32; }],
                       [{ return Imm < 64 && Imm >= 0; }]>;
defm "" : Immediate<7, [{ return Imm < 64 && Imm >= -64; }],
                       [{ return Imm < 128 && Imm >= 0; }]>;
defm "" : Immediate<8, [{ return Imm < 128 && Imm >= -128; }],
                       [{ return Imm < 256 && Imm >= 0; }]>;
defm "" : Immediate<9, [{ return Imm < 256 && Imm >= -256; }],
                       [{ return Imm < 512 && Imm >= 0;    }]>;
defm "" : Immediate<10, [{ return Imm < 512 && Imm >= -512; }],
                        [{ return Imm < 1024 && Imm >= 0; }]>;
defm "" : Immediate<11, [{ return Imm < 1024 && Imm >= -1024; }],
                        [{ return Imm < 2048 && Imm >= 0; }]>;
defm "" : Immediate<12, [{ return Imm < 2048 && Imm >= -2048; }],
                        [{ return Imm < 4096 && Imm >= 0; }]>;
defm "" : Immediate<16, [{ return Imm < 32768 && Imm >= -32768; }],
                        [{ return Imm < 65536 && Imm >= 0; }]>;
defm "" : Immediate<20, [{ return Imm < 524288 && Imm >= -524288; }],
                        [{ return Imm < 1048576 && Imm >= 0; }]>;
                        
def DTUimm8_1022  : Operand<i32>, ImmLeaf<i32, [{
  return Imm >= 8 && Imm < 1023;
}]> {
  let ParserMatchClass = AsmImmRange<8, 1023>;
  let DecoderMethod = "DecodeUImm8_1022";
}

def DummyRetFlag    : SDNode<"DUMMYISD::RET_FLAG", SDTNone,
                           [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

def iADDR12 : ComplexPattern<iPTR, 2, "SelectAddrImm12", [], []>;

class DTUAsmAddrDispOperand<string N> : AsmOperandClass {
  let Name = "AddrDisp" # N;
  let PredicateMethod = "isAddrDisp<" # N # ">";
  let RenderMethod = "addAddrDispOperands<" # N # ">";
}

class MEMri<string N, dag MIInfo> : Operand<iPTR> {
  let PrintMethod      = "printMemRegImm"#N;
  let EncoderMethod    = "getMemRI"#N#"Encoding";
  let DecoderMethod    = "decodeMemRI"#N#"Operands";
  let ParserMatchClass = DTUAsmAddrDispOperand<N>;
  let MIOperandInfo    = MIInfo;
}

def DTUAsmMemBaseOperand : AsmOperandClass {
  let Name = "MemBase";
  let PredicateMethod = "isMemBase";
  let RenderMethod = "addMemBaseOperand";
}

def PtrReg : Operand<iPTR> {
  let DecoderMethod = "DecodeGRegsRegisterClass";
  let ParserMatchClass = DTUAsmMemBaseOperand;
}

def MEMri12 : MEMri<"12", (ops PtrReg:$reg, DTUsimm12:$imm)>;