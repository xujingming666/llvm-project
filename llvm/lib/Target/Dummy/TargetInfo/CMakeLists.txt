add_llvm_component_library(LLVMDummyInfo
  DummyTargetInfo.cpp

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  Dummy
)

add_dependencies(LLVMDummyInfo DummyCommonTableGen)
