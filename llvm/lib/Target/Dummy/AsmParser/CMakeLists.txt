add_llvm_component_library(LLVMDummyAsmParser
  DummyAsmParser.cpp
)

add_dependencies(LLVMDummyAsmParser DummyCommonTableGen)
