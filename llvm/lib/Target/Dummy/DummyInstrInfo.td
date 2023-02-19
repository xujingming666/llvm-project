class VectorOpType<bits<2> val> {
  bits<2> v = val;
}

def VOT_NONE : VectorOpType<0b00>;
def VOT_1D   : VectorOpType<0b01>;
def VOT_2D   : VectorOpType<0b10>;
def VOT_CLR  : VectorOpType<0b11>;

class PacketSlotType<bits<4> val> {
  bits<4> v = val;
}

def S20_PST_V32     : PacketSlotType<0b0001>;
def S20_PST_M48     : PacketSlotType<0b0010>;
def S20_PST_L32     : PacketSlotType<0b0011>;
def S20_PST_S016    : PacketSlotType<0b0100>;
def S20_PST_S116    : PacketSlotType<0b0101>;
def S20_PST_C16     : PacketSlotType<0b0110>;
def S20_PST_CANY16  : PacketSlotType<0b0111>;
def S20_PST_SANY16  : PacketSlotType<0b1000>;
def S20_PST_MANY48  : PacketSlotType<0b1001>;
def S20_PST_VANY32  : PacketSlotType<0b1010>;

class DummyInst32<dag outs, dag ins, string asmstr, list<dag> pattern = []>
    : Instruction {

  field bits<32> Inst;
  field bits<32> SoftFail = 0;
  bit WriteVRPort1Ex3 = 0;
  bit WriteVRPort1Ex7 = 0;
  bit WriteVRPort1Ex9 = 0;
  bit WriteIVPort1 = 0;
  bit ReadTARLDST = 0;
  bit WriteTARGroup = 0;
  bit ReadVRPort0 = 0;
  bit ReadVRPort1 = 0;
  bit ReadIVPort1 = 0;
  bit ReadVALV = 0;
  bit VectorLD = 0;
  bit ScalarLDST = 0;
  bit Control = 0;
  bit MUseVA = 0;
  bit WriteSMR = 0;
  bit VectorST = 0;
  bit VALU_EXE2 = 0;
  bit VALU_EXE3 = 0;
  bit VA_EXE2 = 0;
  bit VA_EXE3 = 0;
  bit VA_EXE7 = 0;
  bit MALUStore = 0;
  bit CPR = 0;
  bit LPR = 0;
  bit MPR = 0;
  bit VPR = 0;
  bits<3> TypeFlag = 0;
  bit CALU1 = 0;
  bit CALU2 = 0;

  let Inst{0} = 1;

  let TSFlags{5-4}   = VOT_NONE.v;  // default set to none vector
  let TSFlags{6}  = WriteVRPort1Ex3;
  let TSFlags{7}  = WriteVRPort1Ex7;
  let TSFlags{8}  = WriteVRPort1Ex9;
  let TSFlags{9}  = WriteIVPort1;
  let TSFlags{10} = WriteTARGroup;
  let TSFlags{11} = ReadTARLDST;
  let TSFlags{12} = ReadVRPort0;
  let TSFlags{13} = ReadVRPort1;
  let TSFlags{14} = ReadIVPort1;
  let TSFlags{15} = ReadVALV;
  let TSFlags{16} = VectorLD;
  let TSFlags{17} = ScalarLDST;
  let TSFlags{18} = Control;
  let TSFlags{19} = MUseVA;
  let TSFlags{20} = WriteSMR;
  let TSFlags{21} = VectorST;
  let TSFlags{24} = VALU_EXE2;
  let TSFlags{25} = VALU_EXE3;
  let TSFlags{26} = VA_EXE2;
  let TSFlags{27} = VA_EXE3;
  let TSFlags{28} = VA_EXE7;
  let TSFlags{29} = MALUStore;
  let TSFlags{30} = CPR;
  let TSFlags{31} = LPR;
  let TSFlags{32} = MPR;
  let TSFlags{33} = VPR;
  let TSFlags{36-34} = TypeFlag;
  let TSFlags{37} = CALU1;
  let TSFlags{38} = CALU2;

  let Namespace      = "Dummy";
  dag OutOperandList = outs;
  dag InOperandList  = ins;
  let AsmString      = asmstr;
  let Pattern        = pattern;
  let Size           = 4;
}

class PseudoInst32<dag outs, dag ins, string asmstr, list<dag> pattern = []>
    : DummyInst32<outs, ins, asmstr, pattern>, Sched<[]> {  //FIXME: sched
  let isPseudo      = 1;
  let isCodeGenOnly = 1;
}

class BranchInst<bits<5> opcode, dag outs, dag ins, string asmstr,
                 list<dag> pattern = []>
    : DummyInst32<outs, ins, asmstr, pattern> {
  let Inst{5-1}      = opcode;
  let TSFlags{3-0}   = S20_PST_M48.v;
  let mayLoad        = 0;
  let mayStore       = ?;
  let hasSideEffects = 0;
}

class ALUInst32<bits<5> opcode, dag outs, dag ins, string asmstr,
                list<dag> pattern = []>
    : DummyInst32<outs, ins, asmstr, pattern> {
  let Inst{5-1}      = opcode;
  let TSFlags{3-0}   = S20_PST_M48.v;
}

//--------------------------------------
// AForm - ALU 32-bit arithmetic
//--------------------------------------
class AForm_RRR<bits<5> op, bits<3> sub, string asm_str, bits<2> cc,
                bits<3> mode, bits<4> slot>
    : ALUInst32<op, (outs GRegs:$Rd), (ins GRegs:$Rs1, GRegs:$Rs2),
                asm_str, []> {
  bits<5> Rd;
  bits<5> Rs1;
  bits<5> Rs2;

  let Inst{10-6}  = Rd;
  let Inst{15-11} = Rs1;
  let Inst{20-16} = Rs2;

  let Inst{22-21} = cc;
  let Inst{25-23} = sub;
  let Inst{29-27} = mode;

  let TSFlags{3-0} = slot;
}

// Normal 12-bits immediate value.
class AForm_RRI<bits<5> op, bits<1> sub, string asm_str, bits<2> cc,
                bits<1> mode, bits<4> slot, dag input, list<dag> pattern>
    : ALUInst32<op, (outs GRegs:$Rd), input, asm_str, pattern> {
  bits<5> Rd;
  bits<5> Rs;
  bits<12> imm;

  let Inst{10-6}  = Rd;
  let Inst{15-11} = Rs;
  let Inst{27-16} = imm;

  let Inst{28} = sub;
  let Inst{29} = mode;
  let Inst{31-30} = cc;

  let TSFlags{3-0} = slot;
}


// Short 12-bits immediate value. Note: UNSIGNED
class AForm_RRI5<bits<5> op, bits<3> sub, string asm_str, bits<2> cc,
                 bits<1> mode, bits<4> slot>
    : ALUInst32<op, (outs GRegs:$Rd), (ins GRegs:$Rs, DTUimm5:$imm),
                asm_str, []> {
  bits<5> Rd;
  bits<5> Rs;
  bits<5> imm;

  let Inst{10-6}  = Rd;
  let Inst{15-11} = Rs;
  let Inst{20-16} = imm;

  let Inst{22-21} = cc;
  let Inst{25-23} = sub;
  let Inst{27} = mode;

  let TSFlags{3-0} = slot;
}

class AForm_RR<bits<5> op, bits<3> sub, bits<2> cc, string asm_str,
               bits<4> slot>
    : ALUInst32<op, (outs GRegs:$Rd), (ins GRegs:$Rs), asm_str> {
  bits<5> Rd;
  bits<5> Rs;

  let Inst{10-6}  = Rd;
  let Inst{15-11} = Rs;
  let Inst{22-21} = cc;
  let Inst{25-23} = sub;

  let TSFlags{3-0} = slot;
}

let Defs = [SCC] in {

// AForm_RRR with Sign
multiclass AForm_RRR_S<bits<5> op, bits<3> sub, string asm_op, bits<2> cc,
                       bits<4> slot, list<dag> spat = [], list<dag> upat = []> {
  let Pattern = upat in {
    def _u : AForm_RRR<op, sub, asm_op # ".u" # " $Rd, $Rs1, $Rs2", cc, 2, slot>,
             Sched<[WriteInt, ReadInt]>;
  }
  let Pattern = spat in {
    def _s : AForm_RRR<op, sub, asm_op # ".s" # " $Rd, $Rs1, $Rs2", cc, 3, slot>,
             Sched<[WriteInt, ReadInt]>;
  }
}

// AForm_RRR with CC and Sign
multiclass AForm_RRR_CC_S<bits<5> op, bits<3> sub, string asm_op, bits<4> slot,
                          list<dag> pattern = [], list<dag> pattern2 = []> {
  defm _a : AForm_RRR_S<op, sub, asm_op # "a", 0, slot, pattern, pattern2>;
  let Uses = [SCC] in {
    let InOperandList = (ins GRegs:$Rs1, GRegs:$Rs2, GRegs:$Rsp),
        Constraints = "$Rd = $Rsp" in {
      defm _t : AForm_RRR_S<op, sub, asm_op # "t", 1, slot>;
      defm _f : AForm_RRR_S<op, sub, asm_op # "f", 2, slot>;
      defm _c : AForm_RRR_S<op, sub, asm_op # "c", 3, slot>;
    }
  }
}

// AForm_RRR with CC only
multiclass AForm_RRR_CC<bits<5> op, bits<3> sub, string asm_op, bits<3> mode,
                        bits<4> slot, list<dag> pattern = []> {
  let InOperandList = (ins GRegs:$Rs1, GRegs:$Rs2), Pattern = pattern,
    Itinerary = IIC_Scalar_RP23_WP1_L in {
    def _a : AForm_RRR<op, sub, asm_op # "a" # " $Rd, $Rs1, $Rs2", 0, mode,
                       slot>,
             Sched<[WriteInt, ReadInt]>;
  }
  let Uses = [SCC], InOperandList = (ins GRegs:$Rs1, GRegs:$Rs2, GRegs:$Rsp),
      Constraints = "$Rd = $Rsp", Itinerary = IIC_Scalar_RP23_RSCC_WP1_L in {
    def _t : AForm_RRR<op, sub, asm_op # "t" # " $Rd, $Rs1, $Rs2", 1, mode,
                       slot>,
             Sched<[WriteInt, ReadInt]>;
    def _f : AForm_RRR<op, sub, asm_op # "f" # " $Rd, $Rs1, $Rs2", 2, mode,
                       slot>,
             Sched<[WriteInt, ReadInt]>;
    def _c : AForm_RRR<op, sub, asm_op # "c" # " $Rd, $Rs1, $Rs2", 3, mode,
                       slot>,
             Sched<[WriteInt, ReadInt]>;
  }
}

// AForm_RRI with Sign
multiclass AForm_RRI_S<bits<5> op, bits<1> sub, string asm_op, bits<2> cc,
                       bits<4> slot, dag s_inop = (ins), dag u_inop = (ins),
                       list<dag> s_pat = [], list<dag> u_pat = []> {
  def _u : AForm_RRI<op, sub, asm_op # ".u" # " $Rd, $Rs, $imm", cc, 0, slot,
                     u_inop, u_pat>, Sched<[WriteInt, ReadInt]>;
  def _s : AForm_RRI<op, sub, asm_op # ".s" # " $Rd, $Rs, $imm", cc, 1, slot,
                     s_inop, s_pat>, Sched<[WriteInt, ReadInt]>;
}

// AForm_RRI with CC and Sign
multiclass AForm_RRI_CC_S<bits<5> op, bits<1> sub, string asm_op, bits<4> slot,
                          list<dag> s_pat, list<dag> u_pat> {
  let Itinerary = IIC_Scalar_RP2_WP1_L in {
    defm _a : AForm_RRI_S<op, sub, asm_op # "a", 0, slot,
                          (ins GRegs:$Rs, DTUsimm12:$imm),
                          (ins GRegs:$Rs, DTUimm12:$imm), s_pat, u_pat>;
  }
  let Uses = [SCC], Constraints = "$Rd = $Rsp", Itinerary = IIC_Scalar_RP2_RSCC_WP1_L in {
    defm _t : AForm_RRI_S<op, sub, asm_op # "t", 1, slot,
                          (ins GRegs:$Rs, DTUsimm12:$imm, GRegs:$Rsp),
                          (ins GRegs:$Rs, DTUimm12:$imm, GRegs:$Rsp)>;
    defm _f : AForm_RRI_S<op, sub, asm_op # "f", 2, slot,
                          (ins GRegs:$Rs, DTUsimm12:$imm, GRegs:$Rsp),
                          (ins GRegs:$Rs, DTUimm12:$imm, GRegs:$Rsp)>;
    defm _c : AForm_RRI_S<op, sub, asm_op # "c", 3, slot,
                          (ins GRegs:$Rs, DTUsimm12:$imm, GRegs:$Rsp),
                          (ins GRegs:$Rs, DTUimm12:$imm, GRegs:$Rsp)>;
  }
}

// AForm_RRR with CC only
multiclass AForm_RRI5_CC<bits<5> op, bits<3> sub, string asm_op, bits<1> mode,
                         bits<4> slot, list<dag> pattern = []> {
  let Pattern = pattern, Itinerary = IIC_Scalar_RP2_WP1_L in {
    def _a : AForm_RRI5<op, sub, asm_op # "a" # " $Rd, $Rs, $imm", 0, mode,
                        slot>, Sched<[WriteInt, ReadInt]>;
  }
  let Uses = [SCC], Itinerary = IIC_Scalar_RP2_RSCC_WP1_L in {
    let InOperandList = (ins GRegs:$Rs, DTUimm5:$imm, GRegs:$Rsp),
        Constraints = "$Rd = $Rsp" in {
      def _t : AForm_RRI5<op, sub, asm_op # "t" # " $Rd, $Rs, $imm", 1, mode,
                          slot>, Sched<[WriteInt, ReadInt]>;
      def _f : AForm_RRI5<op, sub, asm_op # "f" # " $Rd, $Rs, $imm", 2, mode,
                          slot>, Sched<[WriteInt, ReadInt]>;
      def _c : AForm_RRI5<op, sub, asm_op # "c" # " $Rd, $Rs, $imm", 3, mode,
                          slot>, Sched<[WriteInt, ReadInt]>;
    }
  }
}

} // Defs = [SCC]


// Scalar arithmetic: add, sub, mul, max, min, addi, subi

let Itinerary = IIC_Scalar_RP23_RSCC_WP1_L in {
  defm L_SUB : AForm_RRR_CC_S<0b10001, 0b001, "l.sub", S20_PST_L32.v,
                            [(set GRegs:$Rd, (sub GRegs:$Rs1, GRegs:$Rs2))]>;
}

let Itinerary = IIC_Scalar_RP23_RSCC_WP1_NoBypass in {
  defm L_MUL : AForm_RRR_CC_S<0b10001, 0b010, "l.mul", S20_PST_L32.v,
                            [(set GRegs:$Rd, (mul GRegs:$Rs1, GRegs:$Rs2))]>;
}

let Itinerary = IIC_Scalar_RP23_WP1_L in {
  defm L_MAX : AForm_RRR_CC_S<0b10100, 0b000, "l.max", S20_PST_L32.v,
                              [(set GRegs:$Rd, (smax GRegs:$Rs1, GRegs:$Rs2))],
                              [(set GRegs:$Rd, (umax GRegs:$Rs1, GRegs:$Rs2))]>;
  defm L_MIN : AForm_RRR_CC_S<0b10100, 0b001, "l.min", S20_PST_L32.v,
                              [(set GRegs:$Rd, (smin GRegs:$Rs1, GRegs:$Rs2))],
                              [(set GRegs:$Rd, (umin GRegs:$Rs1, GRegs:$Rs2))]>;
}

let isAdd = 1 in {
  defm L_ADDI : AForm_RRI_CC_S<0b10000, 0b0, "l.addi", S20_PST_L32.v,
                               [(set GRegs:$Rd, (add GRegs:$Rs, DTUsimm12:$imm))],
                               [(set GRegs:$Rd, (add GRegs:$Rs, DTUimm12:$imm))]>;
  let Itinerary = IIC_Scalar_RP23_RSCC_WP1_L in {
    defm L_ADD : AForm_RRR_CC_S<0b10001, 0b000, "l.add", S20_PST_L32.v,
                                 [(set GRegs:$Rd, (add GRegs:$Rs1, GRegs:$Rs2))]>;
  }
}

defm L_SUBI : AForm_RRI_CC_S<0b10000, 0b1, "l.subi", S20_PST_L32.v,
                             [(set GRegs:$Rd, (sub GRegs:$Rs, DTUsimm12:$imm))],
                             [(set GRegs:$Rd, (sub GRegs:$Rs, DTUimm12:$imm))]>;

// Scalar logical: and, or, xor
//        shift: sll, srl, sra, slli, srli, srai

defm L_AND : AForm_RRR_CC<0b10010, 0b011, "l.and", 0, S20_PST_L32.v,
                          [(set GRegs:$Rd, (and GRegs:$Rs1, GRegs:$Rs2))]>;
defm L_OR  : AForm_RRR_CC<0b10010, 0b100, "l.or", 0, S20_PST_L32.v,
                          [(set GRegs:$Rd, (or GRegs:$Rs1, GRegs:$Rs2))]>;
defm L_XOR : AForm_RRR_CC<0b10010, 0b101, "l.xor", 0, S20_PST_L32.v,
                          [(set GRegs:$Rd, (xor GRegs:$Rs1, GRegs:$Rs2))]>;

defm L_SLL : AForm_RRR_CC<0b10001, 0b100, "l.sll", 0, S20_PST_L32.v,
                          [(set GRegs:$Rd, (shl GRegs:$Rs1, GRegs:$Rs2))]>;
defm L_SRL : AForm_RRR_CC<0b10001, 0b101, "l.srl", 0, S20_PST_L32.v,
                          [(set GRegs:$Rd, (srl GRegs:$Rs1, GRegs:$Rs2))]>;
defm L_SRA : AForm_RRR_CC<0b10001, 0b101, "l.sra", 1, S20_PST_L32.v,
                          [(set GRegs:$Rd, (sra GRegs:$Rs1, GRegs:$Rs2))]>;

defm L_SLLI : AForm_RRI5_CC<0b10001, 0b110, "l.slli", 0, S20_PST_L32.v,
                            [(set GRegs:$Rd, (shl GRegs:$Rs, DTUimm5:$imm))]>;
defm L_SRLI : AForm_RRI5_CC<0b10001, 0b111, "l.srli", 0, S20_PST_L32.v,
                            [(set GRegs:$Rd, (srl GRegs:$Rs, DTUimm5:$imm))]>;
defm L_SRAI : AForm_RRI5_CC<0b10001, 0b111, "l.srai", 1, S20_PST_L32.v,
                            [(set GRegs:$Rd, (sra GRegs:$Rs, DTUimm5:$imm))]>;


//--------------------------------------
// JForm - Unconditional Jumps
//--------------------------------------
class JForm_RI<bits<5> op, bits<2> sub, string asm_op, bits<4> slot>
    : BranchInst<op, (outs GRegs:$Rd), (ins b_target:$dst),
                 asm_op # " $Rd, $dst">
{
  bits<5> Rd;
  bits<16> dst;

  let Inst{10-6}  = Rd;
  let Inst{14-11} = dst{3-0};
  let Inst{22-16} = dst{10-4};
  let Inst{29-25} = dst{15-11};

  let Inst{24-23} = sub;

  let TSFlags{3-0} = slot;
}

class JForm_RRI<bits<5> op, bits<2> sub, string asm_op, bits<4> slot>
    : BranchInst<op, (outs GRegs:$Rd), (ins GRegs:$Rs, DTUsimm12:$imm),
                 asm_op # " $Rd, $Rs, $imm">
{
  bits<5> Rd;
  bits<5> Rs;
  bits<12> imm;

  let Inst{10-6}  = Rd;
  let Inst{15-11} = Rs;
  let Inst{22-16} = imm{6-0};
  let Inst{29-25} = imm{11-7};

  let Inst{24-23} = sub;

  let TSFlags{3-0} = slot;
}

def L_JAL : JForm_RI <0b10101, 0b11, "l.jal", S20_PST_L32.v> {
  let isCall = 1;
  let TSFlags{3-0}   = S20_PST_L32.v;
  let Control = 1;
  let Itinerary = IIC_Scalar_WP1;
}

def L_JALR : JForm_RRI <0b10101, 0b10, "l.jalr", S20_PST_L32.v> {
  let isReturn = 1;
  let isTerminator = 1;  // TODO: needed?
  let isBarrier = 1;     // TODO: needed?

  let TSFlags{3-0}   = S20_PST_L32.v;
  let Control = 1;
  let Itinerary = IIC_Scalar_RP2_WP1_L;
}

let isTerminator = 1, isReturn = 1, isBarrier = 1, Uses = [LR] in
def L_PseudoRET :
  PseudoInst32<(outs), (ins variable_ops), "", [(DummyRetFlag)]>,
  PseudoInstExpansion<(L_JALR R0, LR, 0)> { let Itinerary = IIC_Scalar_RP2_WP1_L;
  let TSFlags{3-0} = S20_PST_L32.v; }

class LDInst32<bits<5> opcode, dag outs, dag ins, string asmstr,
               list<dag> pattern = []>
    : DummyInst32<outs, ins, asmstr, pattern> {
  let Inst{5-1}      = opcode;
  let TSFlags{3-0}   = S20_PST_L32.v;
  let mayLoad        = 1;
  let mayStore       = 0;
  let hasSideEffects = 0;
}

class LForm_RM<bits<5> op, bits<1> sub, bits<2> width, bits<1> flush,
               string asm_op, bits<4> slot>
    : LDInst32<op, (outs GRegs:$Rd), (ins MEMri12:$addr),
               asm_op  # " $Rd, [$addr]", []>
{
  bits<5> Rd;
  bits<17> addr;

  let Inst{10-6}  = Rd;
  let Inst{15-11} = addr{4-0};    // Rs

  let Inst{22-21} = width;
  let Inst{23}    = sub;
  let Inst{24}    = flush;

  let Inst{20-16} = addr{9-5};    // Imm5
  let Inst{31-25} = addr{16-10};  // Imm7

  let TSFlags{3-0} = slot;
}

multiclass LForm_RM_F<bits<5> op, bits<1> sub, bits<2> width,
                      string asm_op, bits<4> slot> {
  def NAME : LForm_RM<op, sub, width, 0, asm_op, slot> { let ScalarLDST = 1; }
  def _f   : LForm_RM<op, sub, width, 1, asm_op # ".f", slot> { let ScalarLDST = 1; }
}

multiclass LForm_RM_F_W<bits<5> op, bits<1> sub, string asm_op, bits<4> slot> {
  defm _b : LForm_RM_F<op, sub, 0, asm_op # ".b", slot>;
  defm _h : LForm_RM_F<op, sub, 1, asm_op # ".h", slot>;
  defm _w : LForm_RM_F<op, sub, 2, asm_op # ".w", slot>;
}

defm L_LD     : LForm_RM_F_W<0b11000, 0b0, "l.ld", S20_PST_L32.v>;

// Scalar load
def : Pat<(i32  (load       iADDR12:$src)), (L_LD_w iADDR12:$src)>;

class STInst32<bits<5> opcode, dag outs, dag ins, string asmstr,
               list<dag> pattern = []>
    : DummyInst32<outs, ins, asmstr, pattern> {
  let Inst{5-1}      = opcode;
  let TSFlags{3-0}   = S20_PST_L32.v;
  let mayLoad        = 0;
  let mayStore       = 1;
  let hasSideEffects = 0;
}

class SForm_RM<bits<5> op, bit sub, bits<2> width, bit flush,
               string asm_op, bits<4> slot>
    : STInst32<op, (outs), (ins GRegs:$Rs, MEMri12:$addr),
               asm_op # " $Rs, [$addr]", []> {
  bits<5>  Rs;
  bits<17> addr;

  let Inst{15-11} = addr{4-0};   // Rs1
  let Inst{20-16} = Rs;

  let Inst{22-21} = width;
  let Inst{23}    = sub;
  let Inst{24}    = flush;

  let Inst{10-6}  = addr{9-5};     // Imm5
  let Inst{31-25} = addr{16-10};   // Imm7

  let TSFlags{3-0} = slot;
}

// SForm with flush (sync) or not (async)
multiclass SForm_RM_F<bits<5> op, bits<1> sub, bits<2> width,
                      string asm_op, bits<4> slot> {
  def NAME : SForm_RM<op, sub, width, 0, asm_op, slot> { let ScalarLDST = 1; }
  def _f   : SForm_RM<op, sub, width, 1, asm_op # ".f", slot> { let ScalarLDST = 1; }
}

// SForm with flush and width
multiclass SForm_RM_F_W<bits<5> op, bits<1> sub, string asm_op, bits<4> slot> {
  defm _b : SForm_RM_F<op, sub, 0, asm_op # ".b", slot>;
  defm _h : SForm_RM_F<op, sub, 1, asm_op # ".h", slot>;
  defm _w : SForm_RM_F<op, sub, 2, asm_op # ".w", slot>;
}

defm L_ST     : SForm_RM_F_W<0b11000, 0b1, "l.st", S20_PST_L32.v>;

def : Pat<(store         i32:$val,  iADDR12:$src), (L_ST_w i32:$val,          iADDR12:$src)>;

