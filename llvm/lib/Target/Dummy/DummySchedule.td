
def WriteImm       : SchedWrite; // LDI_LO/HI, LDI16
def WriteInt       : SchedWrite; // Arith in ALU (as target)
def ReadInt        : SchedRead;  // Arith in ALU (as source)

def WriteLd        : SchedWrite; // Loads
def WriteSt        : SchedWrite; // Stores


// Scalar
def IIC_Scalar_RP2_WP1_L             : InstrItinClass; // RP2 WP1
def IIC_Scalar_RP2_RSCC_WP1_L        : InstrItinClass; // RP2 WP1 Read:SCC
def IIC_Scalar_RP2_WP1_S             : InstrItinClass; // RP2 WP1
def IIC_Scalar_RP23_WP1_L            : InstrItinClass; // RP2 RP3 WP1
def IIC_Scalar_RP23_RSCC_WP1_L       : InstrItinClass; // RP2 RP3 WP1 Read:SCC
def IIC_Scalar_RP23_WP1_S            : InstrItinClass; // RP2 RP3 WP1
def IIC_Scalar_RP2                   : InstrItinClass; // RP2
def IIC_Scalar_RP23                  : InstrItinClass; // RP2 RP3
def IIC_Scalar_RP23_WSCC             : InstrItinClass; // RP2 RP3 Write SCC
def IIC_Scalar_WP1                   : InstrItinClass; // WP1
def IIC_Scalar_WP1_3                 : InstrItinClass; // WP1
def IIC_Scalar_WP1_3_Single          : InstrItinClass; // WP1 SLOT C/M/V/L
def IIC_Scalar_RP2_WP1_EX8           : InstrItinClass; // RP2 WP1 EX8
def IIC_Scalar_RP23_WP1_NoBypass     : InstrItinClass; // RP2 RP3 WP1
def IIC_Scalar_RP23_RSCC_WP1_NoBypass: InstrItinClass; // RP2 RP3 WP1
def IIC_Scalar                       : InstrItinClass; // Trap0, Trap1, Loop0, Loop1

// C SLOT
def IIC_Scalar_WP0_S1_C              : InstrItinClass; // SR:WritePort0
def IIC_Scalar_WP0_S1_TAR_C          : InstrItinClass; // SR:WritePort0 read tar
def IIC_Scalar_WP0_S1_PC_LR_C        : InstrItinClass; // SR:WritePort0 read PC write LR
def IIC_Scalar_RP0_WP0_S1_C          : InstrItinClass; // SR:ReadPort0 WritePort0
def IIC_Scalar_RP0_WP0_S2_C          : InstrItinClass; // SR:ReadPort0 WritePort0
def IIC_Scalar_RP0_WP0_S1_LR_C       : InstrItinClass; // SR:ReadPort0 WritePort0 write LR
def IIC_Scalar_RP0_WP0_C             : InstrItinClass; // SR:ReadPort0 WritePort0
def IIC_Scalar_RP01_C                : InstrItinClass; // SR:ReadPort0 ReadPort1
def IIC_Scalar_RP0_C                 : InstrItinClass; // SR:ReadPort0
def IIC_Scalar_RP0_PC_C              : InstrItinClass; // SR:ReadPort0 read PC
def IIC_Scalar_RP0_TAR_C             : InstrItinClass; // SR:ReadPort0 write tar
def IIC_Scalar_RP01_WP0_C            : InstrItinClass; // SR:ReadPort0 ReadPort1 WritePort0
def IIC_Scalar_VRP0_C                : InstrItinClass; // VR:ReadPort0 write tar
def IIC_Scalar_VRP0_TAR_C            : InstrItinClass; // VR:ReadPort0 read tar write tar
def IIC_Vector_SRP0_VRP0_TAR_C       : InstrItinClass; // VR:ReadPort0 SR:ReadPort0 write tar read tar
def IIC_Vector_SRP0_VRP0_C           : InstrItinClass; // SR:ReadPort0 VR:ReadPort0
def IIC_Vector_SRP0_VRP0_SWP0_C      : InstrItinClass; // SR:ReadPort0 WritePort0 VR:ReadPort0
def IIC_Scalar_C                     : InstrItinClass;
def IIC_Scalar_S                     : InstrItinClass;
def IIC_Scalar_V                     : InstrItinClass;
def IIC_Scalar_M                     : InstrItinClass;

def SLOT_C  : FuncUnit; // C  SLOT
def SLOT_L  : FuncUnit; // L  SLOT
def SLOT_M  : FuncUnit; // M  SLOT
def SLOT_V  : FuncUnit; // V  SLOT
def SLOT_S0 : FuncUnit; // S0 SLOT
def SLOT_S1 : FuncUnit; // S1 SLOT
def SLOT_S  : FuncUnit; // S  SLOT

def WP0_SR  : FuncUnit; // SR Write Port0
def WP1_SR  : FuncUnit; // SR Write Port1
def WP0_VR  : FuncUnit; // VR Write Port0
def WP1_VR  : FuncUnit; // VR Write Port1
def WP2_VR  : FuncUnit; // VR Write Port2
def WP0_IV  : FuncUnit; // IVR Write Port0
def WP1_IV  : FuncUnit; // IVR Write Port1
def WP0_VA  : FuncUnit; // VA Write Port0
def WP1_VA  : FuncUnit; // VA Write Port1
def RP0_VA  : FuncUnit; // VA Read Port0
def RP1_VA  : FuncUnit; // VA Read Port1
def RP0_SR  : FuncUnit; // SR Read Port0
def RP1_SR  : FuncUnit; // SR Read Port1
def RP2_SR  : FuncUnit; // SR Read Port2
def RP3_SR  : FuncUnit; // SR Read Port3
def RP4_SR  : FuncUnit; // SR Read Port4
def RP0_VR  : FuncUnit; // VR Read Port0
def RP1_VR  : FuncUnit; // VR Read Port1
def RP2_VR  : FuncUnit; // VR Read Port2
def RP0_IV  : FuncUnit; // IV Read Port0
def RP1_IV  : FuncUnit; // IV Read Port1
def RP2_IV  : FuncUnit; // IV Read Port2

def NoFU   : FuncUnit; // Stage not require FU

def DummyItineraries : ProcessorItineraries<
[SLOT_C, SLOT_L, SLOT_M, SLOT_V, SLOT_S0, SLOT_S1, SLOT_S,
 WP0_SR, WP1_SR, WP0_VR, WP1_VR, WP2_VR, WP0_IV, WP1_IV, WP0_VA,
 WP1_VA, RP0_VA, RP1_VA, RP0_SR, RP1_SR, RP2_SR, RP3_SR, RP4_SR,
 RP0_VR, RP1_VR, RP2_VR, RP0_IV, RP1_IV, RP2_IV, NoFU], [], [

  InstrItinData<IIC_Scalar, [InstrStage<1, [SLOT_L], 0>,
                             InstrStage<1, [SLOT_S0], 0>,
                             InstrStage<1, [SLOT_S1]>], [1, 1],
                                     []>,

  InstrItinData<IIC_Scalar_RP2_WP1_L, [InstrStage<1, [SLOT_L], 0>,
                                     InstrStage<1, [SLOT_S0], 0>,
                                     InstrStage<1, [RP2_SR], 0>,
                                     InstrStage<1, [SLOT_S1]>,
                                     InstrStage<2, [NoFU]>,
                                     InstrStage<1, [WP1_SR]>], [1, 1],
                                     []>,

  InstrItinData<IIC_Scalar_RP2_RSCC_WP1_L, [InstrStage<1, [SLOT_L], 0>,
                                     InstrStage<1, [SLOT_S0], 0>,
                                     InstrStage<1, [RP2_SR], 0>,
                                     InstrStage<1, [SLOT_S1]>,
                                     InstrStage<2, [NoFU]>,
                                     InstrStage<1, [WP1_SR]>], [1, 1, 1, 1, 1, 2/*use scc*/],
                                     []>,

  InstrItinData<IIC_Scalar_RP2_WP1_S, [InstrStage<1, [SLOT_S1], 0>,
                                     InstrStage<1, [RP2_SR]>,
                                     InstrStage<1, [WP1_SR]>,
                                     InstrStage<1, [NoFU]>], [1, 0],
                                     []>,

  InstrItinData<IIC_Scalar_WP1, [InstrStage<1, [SLOT_L], 0>,
                                     InstrStage<1, [SLOT_S0], 0>,
                                     InstrStage<1, [SLOT_S1]>,
                                     InstrStage<2, [NoFU]>,
                                     InstrStage<1, [WP1_SR]>], [1],
                                     []>,

  InstrItinData<IIC_Scalar_WP1_3, [InstrStage<1, [SLOT_L], 0>,
                                     InstrStage<1, [SLOT_S0], 0>,
                                     InstrStage<1, [SLOT_S1]>,
                                     InstrStage<3, [WP1_SR]>], [3],
                                     []>,

  // Workaround hardware bug(PAVORTL-173).
  InstrItinData<IIC_Scalar_WP1_3_Single, [InstrStage<1, [SLOT_L], 0>,
                                     InstrStage<1, [SLOT_S0], 0>,
                                     InstrStage<1, [SLOT_M], 0>,
                                     InstrStage<1, [SLOT_V], 0>,
                                     InstrStage<1, [SLOT_C], 0>,
                                     InstrStage<1, [SLOT_S1]>,
                                     InstrStage<3, [WP1_SR]>], [3],
                                     []>,

  InstrItinData<IIC_Scalar_RP2_WP1_EX8, [InstrStage<1, [SLOT_L], 0>,
                                     InstrStage<1, [SLOT_S0], 0>,
                                     InstrStage<1, [RP2_SR], 0>,
                                     InstrStage<1, [SLOT_S1]>,
                                     InstrStage<7, [NoFU]>,
                                     InstrStage<1, [WP1_SR]>], [8, 1],
                                     []>,

  InstrItinData<IIC_Scalar_RP0_WP0_C, [InstrStage<1, [SLOT_C], 0>,
                                     InstrStage<1, [RP0_SR]>], [8, 0, /*implicit use*/0],
                                     []>,

  InstrItinData<IIC_Scalar_RP0_WP0_S1_C, [InstrStage<1, [SLOT_C], 0>,
                                     InstrStage<1, [RP0_SR]>], [1, 0, 0],
                                     []>,

  InstrItinData<IIC_Scalar_RP0_WP0_S1_LR_C, [InstrStage<1, [SLOT_C], 0>,
                                     InstrStage<1, [RP0_SR]>], [0, /*implicit def*/1],
                                     []>,

  InstrItinData<IIC_Scalar_RP0_WP0_S2_C, [InstrStage<1, [SLOT_C], 0>,
                                     InstrStage<1, [RP0_SR]>], [0, /*implicit def*/1, /*implicit use*/0],
                                     []>,

  InstrItinData<IIC_Scalar_RP01_WP0_C, [InstrStage<1, [SLOT_C], 0>,
                                     InstrStage<1, [RP0_SR], 0>,
                                     InstrStage<1, [RP1_SR]>], [1, 0, 0],
                                     []>,

  InstrItinData<IIC_Scalar_VRP0_C, [InstrStage<1, [SLOT_C]>,
                                     InstrStage<1, [RP0_VR]>], [2, 1],
                                     []>,

  InstrItinData<IIC_Scalar_VRP0_TAR_C, [InstrStage<1, [SLOT_C]>,
                                     InstrStage<1, [RP0_VR]>], [2, 1, 1, 1],
                                     []>,

  InstrItinData<IIC_Vector_SRP0_VRP0_TAR_C, [InstrStage<1, [SLOT_C], 0>,
                                     InstrStage<1, [RP0_SR]>,
                                     InstrStage<1, [RP0_VR]>], [1, 0, /*implicit def*/ 2, /*implicit use*/ 1],
                                     []>,

  InstrItinData<IIC_Vector_SRP0_VRP0_C, [InstrStage<1, [SLOT_C], 0>,
                                     InstrStage<1, [RP0_SR]>,
                                     InstrStage<1, [RP0_VR]>], [1, 0],
                                     []>,

  InstrItinData<IIC_Vector_SRP0_VRP0_SWP0_C, [InstrStage<1, [SLOT_C], 0>,
                                     InstrStage<1, [RP0_SR]>,
                                     InstrStage<1, [RP0_VR]>], [1, 1, 0],
                                     []>,

  InstrItinData<IIC_Scalar_WP0_S1_C, [InstrStage<1, [SLOT_C]>], [1],
                                     []>,

  InstrItinData<IIC_Scalar_WP0_S1_TAR_C, [InstrStage<1, [SLOT_C]>], [1, 1, 1],
                                     []>,

  InstrItinData<IIC_Scalar_WP0_S1_PC_LR_C, [InstrStage<1, [SLOT_C]>], [0, /*implicit def*/1],
                                     []>,

  InstrItinData<IIC_Scalar_C, [InstrStage<1, [SLOT_C]>], [0],
                                     []>,

  InstrItinData<IIC_Scalar_S, [InstrStage<1, [SLOT_S]>], [0],
                                     []>,

  InstrItinData<IIC_Scalar_V, [InstrStage<1, [SLOT_V]>], [0],
                                     []>,

  InstrItinData<IIC_Scalar_M, [InstrStage<1, [SLOT_M]>], [0],
                                     []>,

  InstrItinData<IIC_Scalar_RP01_C, [InstrStage<1, [SLOT_C], 0>,
                                     InstrStage<1, [RP0_SR], 0>,
                                     InstrStage<1, [RP1_SR]>], [0, 0, 0],
                                     []>,

  InstrItinData<IIC_Scalar_RP0_C, [InstrStage<1, [SLOT_C], 0>,
                                   InstrStage<1, [RP0_SR]>], [0],
                                     []>,

  InstrItinData<IIC_Scalar_RP0_TAR_C, [InstrStage<1, [SLOT_C], 0>,
                                   InstrStage<1, [RP0_SR]>], [2, 0, 0, 0],
                                     []>,

  InstrItinData<IIC_Scalar_RP0_PC_C, [InstrStage<1, [SLOT_C], 0>,
                                   InstrStage<1, [RP0_SR]>], [0, 0],
                                     []>,

  InstrItinData<IIC_Scalar_RP2, [InstrStage<1, [SLOT_L], 0>,
                                  InstrStage<1, [SLOT_S0], 0>,
                                  InstrStage<1, [RP2_SR], 0>,
                                  InstrStage<1, [SLOT_S1]>,
                                     ], [1],
                                     []>,

  InstrItinData<IIC_Scalar_RP23, [InstrStage<1, [SLOT_L], 0>,
                                  InstrStage<1, [SLOT_S0], 0>,
                                  InstrStage<1, [RP2_SR], 0>,
                                  InstrStage<1, [RP3_SR], 0>,
                                  InstrStage<1, [SLOT_S1]>,
                                     ], [1, 1],
                                     []>,

  InstrItinData<IIC_Scalar_RP23_WSCC, [InstrStage<1, [SLOT_L], 0>,
                                  InstrStage<1, [SLOT_S0], 0>,
                                  InstrStage<1, [RP2_SR], 0>,
                                  InstrStage<1, [RP3_SR], 0>,
                                  InstrStage<1, [SLOT_S1]>], [1, 1, 2/*def scc*/],
                                  []>,

  InstrItinData<IIC_Scalar_RP23_WP1_L, [InstrStage<1, [SLOT_L], 0>,
                                     InstrStage<1, [SLOT_S0], 0>,
                                     InstrStage<1, [RP2_SR], 0>,
                                     InstrStage<1, [RP3_SR], 0>,
                                     InstrStage<1, [SLOT_S1]>,
                                     InstrStage<2, [NoFU]>,
                                     InstrStage<1, [WP1_SR]>],
                                     [1, 1, 1, 1, 1],
                                     []>,

  InstrItinData<IIC_Scalar_RP23_RSCC_WP1_L, [InstrStage<1, [SLOT_L], 0>,
                                     InstrStage<1, [SLOT_S0], 0>,
                                     InstrStage<1, [RP2_SR], 0>,
                                     InstrStage<1, [RP3_SR], 0>,
                                     InstrStage<1, [SLOT_S1]>,
                                     InstrStage<2, [NoFU]>,
                                     InstrStage<1, [WP1_SR]>],
                                     [1, 1, 1, 1, 1, 2/* use scc*/],
                                     []>,

  InstrItinData<IIC_Scalar_RP23_WP1_S, [InstrStage<1, [SLOT_S1], 0>,
                                     InstrStage<1, [RP2_SR], 0>,
                                     InstrStage<1, [RP3_SR]>,
                                     InstrStage<1, [WP1_SR]>,
                                     InstrStage<1, [NoFU]>], [1, 0, 0], //WR.SCC.Carry
                                     []>,

  InstrItinData<IIC_Scalar_RP23_WP1_NoBypass, [InstrStage<1, [SLOT_L], 0>,
                                     InstrStage<1, [SLOT_S0], 0>,
                                     InstrStage<1, [RP2_SR], 0>,
                                     InstrStage<1, [RP3_SR], 0>,
                                     InstrStage<1, [SLOT_S1]>,
                                     InstrStage<2, [NoFU]>,
                                     InstrStage<1, [WP1_SR]>], [3, 1, 1],
                                     []>,

  InstrItinData<IIC_Scalar_RP23_RSCC_WP1_NoBypass, [InstrStage<1, [SLOT_L], 0>,
                                     InstrStage<1, [SLOT_S0], 0>,
                                     InstrStage<1, [RP2_SR], 0>,
                                     InstrStage<1, [RP3_SR], 0>,
                                     InstrStage<1, [SLOT_S1]>,
                                     InstrStage<2, [NoFU]>,
                                     InstrStage<1, [WP1_SR]>], [3, 1, 1, 1, 1, 2],
                                     []>,
]>;

def DummyModel : SchedMachineModel {
  let IssueWidth = 5;
  let Itineraries = DummyItineraries;
  let CompleteModel = 0;
}

