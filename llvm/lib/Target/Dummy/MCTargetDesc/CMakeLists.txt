add_llvm_component_library(LLVMDummyDesc
  DummyContainerObjectWriter.cpp
  DummyMCTargetDesc.cpp

  LINK_COMPONENTS
  DummyInfo
  Support

  ADD_TO_COMPONENT
  Dummy
  )
