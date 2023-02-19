
class DummyReg<string n> : Register<n> {
  let Namespace = "Dummy";
}

class DummyReg1<bits<1> enc, string n> : DummyReg<n> {
  let HWEncoding{0} = enc;
}

class DummyReg5<bits<5> enc, string n> : DummyReg<n> {
  let HWEncoding{4-0} = enc;
}

class GPR<bits<5> enc, string n>   : DummyReg5<enc, n>;   // Scalar Integer

// Special used GPR
def R0              : GPR<0, "r0">,                DwarfRegNum<[0]>;
def R1              : GPR<1, "r1">,                DwarfRegNum<[1]>;
def SP              : GPR<2, "sp">,                DwarfRegNum<[2]>;
def LR              : GPR<3, "lr">,                DwarfRegNum<[3]>;
def SCC             : GPR<4, "scc">,               DwarfRegNum<[4]>;

// General Purpose (Integer Scalar) Registers
foreach I = 5 - 31 in
  def R # I         : GPR<I, "r" # I>,             DwarfRegNum<[I]>;

def GRegs  : RegisterClass<"Dummy", [i32], 32,
                           (add R0, R1, SP, LR, SCC, (sequence "R%u", 5, 31))>;

