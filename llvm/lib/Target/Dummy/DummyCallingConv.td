
def CC_DUMMY : CallingConv<[
  // The first 4 integer arguments are passed in integer registers.
  CCIfType<[i32, f32], CCAssignToReg<[R5, R6, R7, R8, R9, R10, R11, R12]>>,
]>;

def CC_DUMMY_RET : CallingConv<[
  // The first 4 integer arguments are passed in integer registers.
  CCIfType<[i32, f32], CCAssignToReg<[R5, R6, R7, R8]>>,
]>;

