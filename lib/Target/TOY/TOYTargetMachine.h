//===-- TOYTargetMachine.h - Define TargetMachine for TOY ---*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the TOY specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef TOYTARGETMACHINE_H
#define TOYTARGETMACHINE_H

#include "TOY.h"
#include "TOYFrameLowering.h"
#include "TOYISelLowering.h"
#include "TOYInstrInfo.h"
#include "TOYSelectionDAGInfo.h"
#include "TOYSubtarget.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class TOYTargetMachine : public LLVMTargetMachine {
    std::unique_ptr<TargetLoweringObjectFile> TLOF;
    TOYSubtarget Subtarget;


    public:
    TOYTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
            StringRef FS, const TargetOptions &Options,
            Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
            CodeGenOpt::Level OL, bool JIT);    
    const TOYSubtarget *getSubtargetImpl(const Function&) const override { return &Subtarget; }

    // Pass Pipeline Configuration
    TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

    void addAnalysisPasses(PassManagerBase &PM);
    
    TargetLoweringObjectFile *getObjFileLowering() const override {
        return TLOF.get();
    }

};

} // end namespace llvm

#endif
