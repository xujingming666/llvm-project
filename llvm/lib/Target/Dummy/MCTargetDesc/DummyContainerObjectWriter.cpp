//===-- DirectXContainerObjectWriter.cpp - DX object writer ----*- C++ -*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains DXContainer object writers for the DirectX backend.
//
//===----------------------------------------------------------------------===//

#include "DummyContainerObjectWriter.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/BinaryFormat/ELF.h"

using namespace llvm;

namespace {
class DummyContainerObjectWriter : public MCELFObjectTargetWriter {
public:
  DummyContainerObjectWriter(uint8_t _OSABI) : MCELFObjectTargetWriter(false, _OSABI, ELF::EM_DUMMY, true) {}
  unsigned getRelocType(MCContext &ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override {
    return 0;
  }
};
} // namespace

std::unique_ptr<MCObjectTargetWriter>
llvm::createDummyContainerTargetObjectWriter(uint8_t _OSABI) {
  return std::make_unique<DummyContainerObjectWriter>(_OSABI);
}
