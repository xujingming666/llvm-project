//===-- DirectXSubtarget.cpp - DirectX Subtarget Information --------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file implements the DirectX-specific subclass of TargetSubtarget.
///
//===----------------------------------------------------------------------===//

#include "DummySubtarget.h"
#include "DummyTargetMachine.h"

using namespace llvm;

#define DEBUG_TYPE "dummy-subtarget"

#define GET_SUBTARGETINFO_CTOR
#define GET_SUBTARGETINFO_TARGET_DESC
#include "DummyGenSubtargetInfo.inc"

DummySubtarget::DummySubtarget(const Triple &TT, StringRef CPU,
                                   StringRef FS, const DummyTargetMachine &TM)
    : DummyGenSubtargetInfo(TT, CPU, CPU, FS),
      FL(*this),
      InstrInfo(),
      InstrItins(getInstrItineraryForCPU(CPU)),
      RegInfo(),
      TL(TM, *this) {}

void DummySubtarget::anchor() {}
