let TargetPrefix = "dummy" in {
def int_dummy_test : Intrinsic<[], [],
                          [IntrHasSideEffects], "llvm.dummy.test">;
}