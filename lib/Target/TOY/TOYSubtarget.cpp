//===-- TOYSubtarget.cpp - TOY Subtarget Information ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the TOY specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "TOYSubtarget.h"
#include "TOY.h"
#include "llvm/Support/TargetRegistry.h"

#define DEBUG_TYPE "toy-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "TOYGenSubtargetInfo.inc"

using namespace llvm;

TOYSubtarget &TOYSubtarget::initializeSubtargetDependencies(StringRef CPU,
        StringRef FS,
        bool Is64Bit = false) {
    //Determine default and user-specified characteristics
        std::string CPUName = CPU;
    if (CPUName.empty())
        CPUName = Is64Bit ? "generic-rv64" : "generic-rv32";
    ParseSubtargetFeatures(CPUName, FS);
  //  if (Is64Bit) {
    //    XLenVT = MVT::i64;
      //  XLen = 64;
    //}
    return *this;
}

void TOYSubtarget::anchor() {}

TOYSubtarget::TOYSubtarget(const Triple &TT, const std::string &CPU,
        const std::string &FS, const TargetMachine &TM)
: TOYGenSubtargetInfo(TT, CPU, FS),
    FrameLowering(initializeSubtargetDependencies(CPU, FS)),
    InstrInfo(), 
    RegInfo(), 
    TLInfo(TM, *this)  {}
