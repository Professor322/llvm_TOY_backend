//===-- TOYSubtarget.h - Define Subtarget for the TOY -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the TOY specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#ifndef TOYSUBTARGET_H
#define TOYSUBTARGET_H

#include "TOY.h"
#include "TOYFrameLowering.h"
#include "TOYISelLowering.h"
#include "TOYInstrInfo.h"
#include "TOYSelectionDAGInfo.h"
#include "TOYSubtarget.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include <string>

#define GET_SUBTARGETINFO_HEADER
#include "TOYGenSubtargetInfo.inc"

namespace llvm {
class StringRef;

class TOYSubtarget : public TOYGenSubtargetInfo {
  virtual void anchor();

private:
 // const DataLayout DL;       // Calculates type size & alignment.
  TOYFrameLowering FrameLowering;
  TOYInstrInfo InstrInfo;
  TOYRegisterInfo RegInfo;
  TOYTargetLowering TLInfo;
  SelectionDAGTargetInfo TSInfo;
  //InstrItineraryData InstrItins;
 
  TOYSubtarget &initializeSubtargetDependencies(StringRef CPU, StringRef FS,
                                                          bool Is64Bit);

public:
  /// This constructor initializes the data members to match that
  /// of the specified triple.
  ///
  TOYSubtarget(const Triple &TT, const std::string &CPU,
               const std::string &FS, const TargetMachine &TM);

  /// getInstrItins - Return the instruction itineraries based on subtarget
  /// selection.
//  const InstrItineraryData *getInstrItineraryData() const override {
  //  return &InstrItins;
 // }
  const TOYInstrInfo *getInstrInfo() const override { return &InstrInfo; }
  const TOYRegisterInfo *getRegisterInfo() const override {
    return &InstrInfo.getRegisterInfo();
  }
  const TOYTargetLowering *getTargetLowering() const override {
    return &TLInfo;
  }
  const TOYFrameLowering *getFrameLowering() const override {
    return &FrameLowering;
  }
  const SelectionDAGTargetInfo *getSelectionDAGInfo() const override {
    return &TSInfo;
  }
  //const DataLayout *getDataLayout() const override { return &DL; }

  /// ParseSubtargetFeatures - Parses features string setting specified
  /// subtarget options.  Definition of function is auto generated by tblgen.
  void ParseSubtargetFeatures(StringRef CPU, StringRef FS);
};
} // End llvm namespace

#endif

