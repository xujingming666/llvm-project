#include "InputFiles.h"
#include "Symbols.h"
#include "Target.h"
#include "lld/Common/ErrorHandler.h"
#include "llvm/Object/ELF.h"
#include "llvm/Support/Endian.h"
#include <iostream>

using namespace llvm;
using namespace llvm::object;
using namespace llvm::support::endian;
using namespace llvm::ELF;
using namespace lld;
using namespace lld::elf;

namespace {
  class Dummy final : public TargetInfo {
  public:
    void relocate(uint8_t *loc, const Relocation &rel, uint64_t val) const override;
    RelExpr getRelExpr(RelType Type, const Symbol &S,
                       const uint8_t *Loc) const override;
    Dummy() {
      defaultImageBase = 0;
    }
  };
} // namespace

void Dummy::relocate(uint8_t *Loc, const Relocation &Rel, uint64_t Val) const {
}

RelExpr Dummy::getRelExpr(RelType Type, const Symbol &S,
                           const uint8_t *Loc) const {
    return R_ABS;
}

TargetInfo *elf::getDummyTargetInfo() {
    static Dummy Target;
    return &Target;
}
