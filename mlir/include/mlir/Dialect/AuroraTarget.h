#ifndef AURORA_TARGET_H_
#define AURORA_TARGET_H_

#define MPU_TARGET   "aurora_m"
#define SPU_TARGET   "aurora_s"

#define MPU_KERNEL   "mpu_kernel"
#define SPU_KERNEL   "spu_kernel"

static const char *gMpuLayout = "e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i64:32-f64:32-v1024:1024-v8192:1024-a:0:32-n32";
static const char *gSpuLayout = "e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i64:32-f64:32-v1024:1024-v8192:1024-a:0:32-n32";


#endif // AURORA_TARGET_H_
