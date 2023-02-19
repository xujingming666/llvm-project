//===-- DirectXInstrInfo.cpp - InstrInfo for DirectX -*- C++ ------------*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the DirectX specific subclass of TargetInstrInfo.
//
//===----------------------------------------------------------------------===//

#include "DummyInstrInfo.h"

#define GET_INSTRINFO_CTOR_DTOR
#include "DummyGenInstrInfo.inc"

using namespace llvm;

DummyInstrInfo::~DummyInstrInfo() {}
bool DummyInstrInfo::expandPostRAPseudo(MachineInstr &MI) const {
  MachineBasicBlock &MBB = *MI.getParent();
  MBB.erase(MI);
  
  return true;
}

unsigned DummyInstrInfo::getInstSizeInBytes(const MachineInstr &MI) const {
  return 8;
}