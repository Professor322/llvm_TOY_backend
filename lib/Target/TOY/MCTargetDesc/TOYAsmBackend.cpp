//===-- TOYAsmBackend.cpp - TOY Assembler Backend -------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/TOYMCTargetDesc.h"
#include "MCTargetDesc/TOYFixupKinds.h"
#include "llvm/ADT/APInt.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCDirectives.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCFixupKindInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
class TOYELFObjectWriter : public MCELFObjectTargetWriter {
public:
  TOYELFObjectWriter(uint8_t OSABI)
      : MCELFObjectTargetWriter(/*Is64Bit*/ false, OSABI, /*ELF::EM_TOY*/ ELF::EM_ARM,
                                /*HasRelocationAddend*/ false) {}
};

class TOYAsmBackend : public MCAsmBackend {
 uint8_t OSABI;
 bool Is64Bit;

public:
  TOYAsmBackend(uint8_t OSABI, bool Is64Bit) : MCAsmBackend() {}

  ~TOYAsmBackend() override {}

  unsigned getNumFixupKinds() const override {
    return TOY::NumTargetFixupKinds;
  }

  std::unique_ptr<MCObjectWriter>
  createObjectWriter(raw_pwrite_stream &OS) const override {
                    return std::unique_ptr<MCObjectWriter>(createTOYELFObjectWriter(OS, OSABI));
                }

  const MCFixupKindInfo &getFixupKindInfo(MCFixupKind Kind) const override {
    const static MCFixupKindInfo Infos[TOY::NumTargetFixupKinds] = {
      // This table *must* be in the order that the fixup_* kinds are defined in
      // TOYFixupKinds.h.
      //
      // Name                      Offset (bits) Size (bits)     Flags
      { "fixup_toy_mov_hi16_pcrel", 0, 32, MCFixupKindInfo::FKF_IsPCRel },
      { "fixup_toy_mov_lo16_pcrel", 0, 32, MCFixupKindInfo::FKF_IsPCRel },
    };

    if (Kind < FirstTargetFixupKind) {
      return MCAsmBackend::getFixupKindInfo(Kind);
    }

    assert(unsigned(Kind - FirstTargetFixupKind) < getNumFixupKinds() &&
           "Invalid kind!");
    return Infos[Kind - FirstTargetFixupKind];
  }

  /// processFixupValue - Target hook to process the literal value of a fixup
  /// if necessary.
  //void processFixupValue(const MCAssembler &Asm, const MCAsmLayout &Layout,
    //                     const MCFixup &Fixup, const MCFragment *DF,
      //                   const MCValue &Target, uint64_t &Value,
        //                 bool &IsResolved) override;

  void applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
          const MCValue &Target, MutableArrayRef<char> Data,
          uint64_t Value, bool IsResolved) const override;  

  bool mayNeedRelaxation(const MCInst &Inst) const override { return false; }

  bool fixupNeedsRelaxation(const MCFixup &Fixup, uint64_t Value,
          const MCRelaxableFragment *DF,
          const MCAsmLayout &Layout) const override {
      return false;
  }

  void relaxInstruction(const MCInst &Inst, const MCSubtargetInfo &STI,
          MCInst &Res) const override {

      report_fatal_error("TOYAsmBackend::relaxInstruction() unimplemented");
  }
  bool writeNopData(uint64_t Count, MCObjectWriter *OW) const override {
      if (Count == 0) {
          return true;
      }
      return false;
  }

  unsigned getPointerSize() const { return 4; }
};
} // end anonymous namespace

static unsigned adjustFixupValue(const MCFixup &Fixup, uint64_t Value,
        MCContext *Ctx = NULL) {
    unsigned Kind = Fixup.getKind();
    switch (Kind) {
        default:
            llvm_unreachable("Unknown fixup kind!");
        case TOY::fixup_toy_mov_hi16_pcrel:
            Value >>= 16;
            // Intentional fall-through
        case TOY::fixup_toy_mov_lo16_pcrel:
            unsigned Hi4  = (Value & 0xF000) >> 12;
            unsigned Lo12 = Value & 0x0FFF;
            // inst{19-16} = Hi4;
            // inst{11-0} = Lo12;
            Value = (Hi4 << 16) | (Lo12);
            return Value;
    }
    return Value;
}
/*
void TOYAsmBackend::processFixupValue(const MCAssembler &Asm,
        const MCAsmLayout &Layout,
        const MCFixup &Fixup,
        const MCFragment *DF,
        const MCValue &Target, uint64_t &Value,
        bool &IsResolved) {
    // We always have resolved fixups for now.
    IsResolved = true;
    // At this point we'll ignore the value returned by adjustFixupValue as
    // we are only checking if the fixup can be applied correctly.
    (void)adjustFixupValue(Fixup, Value, &Asm.getContext());
}
*/
void TOYAsmBackend::applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
        const MCValue &Target, MutableArrayRef<char> Data,
        uint64_t Value, bool IsResolved) const {
    unsigned NumBytes = 4;
    Value = adjustFixupValue(Fixup, Value);
    if (!Value) {
        return; // Doesn't change encoding.
    }

    unsigned Offset = Fixup.getOffset();
    assert(Offset + NumBytes <= Data.size() && "Invalid fixup offset!");

    // For each byte of the fragment that the fixup touches, mask in the bits from
    // the fixup value. The Value has been "split up" into the appropriate
    // bitfields above.
    for (unsigned i = 0; i != NumBytes; ++i) {
        Data[Offset + i] |= uint8_t((Value >> (i * 8)) & 0xff);
    }
}

namespace {

    class ELFTOYAsmBackend : public TOYAsmBackend {
        public:
            uint8_t OSABI;
            ELFTOYAsmBackend(uint8_t OSABI, bool Is64Bit)
                : TOYAsmBackend(OSABI, Is64Bit), OSABI(OSABI) {}
    };

} // end anonymous namespace

namespace llvm {
MCAsmBackend *createTOYAsmBackend(const Target &T,
        const MCSubtargetInfo &STI,
        const MCRegisterInfo &MRI,
        const MCTargetOptions &Options) {
    const Triple &TT = STI.getTargetTriple();
    uint8_t OSABI = MCELFObjectTargetWriter::getOSABI(TT.getOS());
    return new TOYAsmBackend(OSABI, false);
}
}

