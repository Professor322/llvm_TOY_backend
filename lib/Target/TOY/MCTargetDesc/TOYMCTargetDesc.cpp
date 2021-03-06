//===-- TOYMCTargetDesc.cpp - TOY Target Descriptions -----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides TOY specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "TOYMCTargetDesc.h"
#include "InstPrinter/TOYInstPrinter.h"
#include "TOYMCAsmInfo.h"
//#include "llvm/MC/MCCodeGenInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_INSTRINFO_MC_DESC
#include "TOYGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "TOYGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "TOYGenRegisterInfo.inc"

using namespace llvm;

static MCInstrInfo *createTOYMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitTOYMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createTOYMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitTOYMCRegisterInfo(X, TOY::LR);
  return X;
}

static MCSubtargetInfo *createTOYMCSubtargetInfo(const Triple& TT, StringRef CPU,
                                                 StringRef FS) {
  //MCSubtargetInfo *X = new MCSubtargetInfo();
  //InitTOYMCSubtargetInfo(X, TT, CPU, FS);
  //return X;
  return createTOYMCSubtargetInfoImpl(TT, CPU, FS);
}

static MCAsmInfo *createTOYMCAsmInfo(const MCRegisterInfo &MRI, const Triple &TT) {
  MCAsmInfo *MAI = new TOYMCAsmInfo(TT);
  return MAI;
}

/*static MCCodeGenInfo *createTOYMCCodeGenInfo(StringRef TT, Reloc::Model RM,
                                             CodeModel::Model CM,
                                             CodeGenOpt::Level OL) {
  MCCodeGenInfo *X = new MCCodeGenInfo();
  if (RM == Reloc::Default) {
    RM = Reloc::Static;
  }
  if (CM == CodeModel::Default) {
    CM = CodeModel::Small;
  }
  if (CM != CodeModel::Small && CM != CodeModel::Large) {
    report_fatal_error("Target only supports CodeModel Small or Large");
  }

  X->InitMCCodeGenInfo(RM, CM, OL);
  return X;
}*/

static MCInstPrinter *
createTOYMCInstPrinter(const Triple &T, unsigned SyntaxVariant,
        const MCAsmInfo &MAI, const MCInstrInfo &MII,
        const MCRegisterInfo &MRI) {
    return new TOYInstPrinter(MAI, MII, MRI);
}

/*static MCStreamer *
createMCAsmStreamer(MCContext &Ctx, formatted_raw_ostream &OS,
        bool isVerboseAsm, bool useDwarfDirectory,
        MCInstPrinter *InstPrint, MCCodeEmitter *CE,
        MCAsmBackend *TAB, bool ShowInst) {
    return createAsmStreamer(Ctx, OS, isVerboseAsm, useDwarfDirectory, InstPrint,
            CE, TAB, ShowInst);
}
*/
/*
static MCStreamer *createMCStreamer(const Target &T, StringRef TT,
        MCContext &Ctx, MCAsmBackend &MAB,
        raw_ostream &OS,
        MCCodeEmitter *Emitter,
        const MCSubtargetInfo &STI,
        bool RelaxAll,
        bool NoExecStack) {
    return createELFStreamer(Ctx, MAB, OS, Emitter, false, NoExecStack);
}
*/

// Force static initialization.
extern "C" void LLVMInitializeTOYTargetMC() {
    // Register the MC asm info.
    TargetRegistry::RegisterMCAsmInfo(getTheTOYTarget(), createTOYMCAsmInfo);

    // Register the MC codegen info.
   // TargetRegistry::RegisterMCCodeGenInfo(TheTOYTarget, createTOYMCCodeGenInfo);

    // Register the MC instruction info.
    TargetRegistry::RegisterMCInstrInfo(getTheTOYTarget(), createTOYMCInstrInfo);

    // Register the MC register info.
    TargetRegistry::RegisterMCRegInfo(getTheTOYTarget(), createTOYMCRegisterInfo);

    // Register the MC subtarget info.
    TargetRegistry::RegisterMCSubtargetInfo(getTheTOYTarget(),
            createTOYMCSubtargetInfo);

    // Register the MCInstPrinter
    TargetRegistry::RegisterMCInstPrinter(getTheTOYTarget(), createTOYMCInstPrinter);

    // Register the ASM Backend.
   // TargetRegistry::RegisterMCAsmBackend(getTheTOYTarget(), createTOYAsmBackend);

    // Register the assembly streamer.
    //TargetRegistry::RegisterAsmStreamer(TheTOYTarget, createMCAsmStreamer);

    // Register the object streamer.
    //TargetRegistry::RegisterMCObjectStreamer(TheTOYTarget, createMCStreamer);

    // Register the MCCodeEmitter
 //   TargetRegistry::RegisterMCCodeEmitter(TheTOYTarget, createTOYMCCodeEmitter);
}
