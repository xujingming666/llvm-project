add_llvm_component_group(Dummy)

set(LLVM_TARGET_DEFINITIONS Dummy.td)

tablegen(LLVM DummyGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM DummyGenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM DummyGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM DummyGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM DummyGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM DummyGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM DummyGenCallingConv.inc -gen-callingconv)
tablegen(LLVM DummyGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM DummyGenMCCodeEmitter.inc -gen-emitter)
add_public_tablegen_target(DummyCommonTableGen)

add_llvm_target(DummyCodeGen
  DummyPassConfig.cpp
  DummyInstrInfo.cpp
  DummyRegisterInfo.cpp
  DummyFrameLowering.cpp
  DummyTargetLowering.cpp
  DummyDAGToDAGISel.cpp
  DummyTargetMachine.cpp
  DummySubtarget.cpp
  DummyAsmPrinter.cpp
  
  LINK_COMPONENTS
  Analysis
  AsmPrinter
  Core
  IPO
  CodeGen
  MC
  SelectionDAG
  Support
  Target
  TransformUtils
  GlobalISel

  ADD_TO_COMPONENT
  Dummy
  )

add_subdirectory(MCTargetDesc)
add_subdirectory(AsmParser)
add_subdirectory(Disassembler)
add_subdirectory(TargetInfo)