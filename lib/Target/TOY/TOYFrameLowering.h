//===-- TOYFrameLowering.h - Frame info for TOY Target ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains TOY frame information that doesn't fit anywhere else
// cleanly...
//
//===----------------------------------------------------------------------===//

#ifndef TOYFRAMEINFO_H
#define TOYFRAMEINFO_H

#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class TOYSubtarget;

class TOYFrameLowering : public TargetFrameLowering {
public:
    explicit TOYFrameLowering(const TOYSubtarget &STI)
        : TargetFrameLowering(StackGrowsDown,
                /*StackAlignment=*/4,
                /*LocalAreaOffset=*/0),
        STI(STI)
       {}


    /// emitProlog/emitEpilog - These methods insert prolog and epilog code into
    /// the function.
    void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
    void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;


    MachineBasicBlock::iterator eliminateCallFramePseudoInstr(MachineFunction &MF,
            MachineBasicBlock &MBB,
            MachineBasicBlock::iterator I) const override;

    bool hasFP(const MachineFunction &MF) const override;

    //! Stack slot size (4 bytes)
    static int stackSlotSize() { return 4; }
protected:
    const TOYSubtarget &STI;

private:
    uint64_t computeStackSize(MachineFunction &MF) const;
};
}

#endif // TOYFRAMEINFO_H

