include "llvm/Target/Target.td"

def DummyInstrInfo : InstrInfo;

def Dummy : Target {
    let InstructionSet = DummyInstrInfo;
}

def FeatureStd: SubtargetFeature<"dummy", "UseStd", "true", "Use standard target feature">;

include "DummyOperators.td"
include "DummySchedule.td"
include "DummyRegisterInfo.td"
include "DummyInstrInfo.td"
include "DummyCallingConv.td"

def : ProcessorModel<"dummy", DummyModel, [FeatureStd]>;


