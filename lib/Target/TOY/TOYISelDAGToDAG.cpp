//===-- TOYISelDAGToDAG.cpp - A dag to dag inst selector for TOY ------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines an instruction selector for the TOY target.
//
//===----------------------------------------------------------------------===//

#include "TOY.h"
#include "TOYTargetMachine.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/raw_ostream.h"

#include "TOYInstrInfo.h"

using namespace llvm;

#define DEBUG_TYPE "toy-isel"
/// TOYDAGToDAGISel - TOY specific code to select TOY machine
/// instructions for SelectionDAG operations.
///
namespace {
class TOYDAGToDAGISel final : public SelectionDAGISel {
  const TOYSubtarget *Subtarget;

public:
  explicit TOYDAGToDAGISel(TOYTargetMachine &TM, CodeGenOpt::Level OptLevel)
      : SelectionDAGISel(TM) {}
 

  void Select(SDNode *N) override;

  bool SelectAddr(SDValue Addr, SDValue &Base, SDValue &Offset);

  bool runOnMachineFunction(MachineFunction &MF) override {
      Subtarget = &MF.getSubtarget<TOYSubtarget>();
      return SelectionDAGISel::runOnMachineFunction(MF);
    }

  StringRef getPassName() const override {
    return "TOY DAG->DAG Pattern Instruction Selection";
  }

//private:
  void SelectMoveImmediate(SDNode *N);

// Include the pieces autogenerated from the target description.
#include "TOYGenDAGISel.inc"
};
} // end anonymous namespace

bool TOYDAGToDAGISel::SelectAddr(SDValue Addr, SDValue &Base, SDValue &Offset) {
  /*if (FrameIndexSDNode *FIN = dyn_cast<FrameIndexSDNode>(Addr)) {
    Base = CurDAG->getTargetFrameIndex(FIN->getIndex(),
                                       getTargetLowering()->getPointerTy());
    Offset = CurDAG->getTargetConstant(0, MVT::i32);
    return true;
  }
  if (Addr.getOpcode() == ISD::TargetExternalSymbol ||
      Addr.getOpcode() == ISD::TargetGlobalAddress ||
      Addr.getOpcode() == ISD::TargetGlobalTLSAddress) {
    return false; // direct calls.
  }

  Base = Addr;
  Offset = CurDAG->getTargetConstant(0, MVT::i32);*/
  return true;
}

void TOYDAGToDAGISel::SelectMoveImmediate(SDNode *N) {
  // Make sure the immediate size is supported.
  ConstantSDNode *ConstVal = cast<ConstantSDNode>(N);
  uint64_t ImmVal = ConstVal->getZExtValue();
  uint64_t SupportedMask = 0xfffffffff;
  if ((ImmVal & SupportedMask) != ImmVal) {
        SelectCode(N);
  }

  // Select the low part of the immediate move.
  uint64_t LoMask = 0xffff;
  uint64_t HiMask = 0xffff0000;
  uint64_t ImmLo = (ImmVal & LoMask);
  uint64_t ImmHi = (ImmVal & HiMask);
  //SDValue ConstLo = CurDAG->getTargetConstant(ImmLo, MVT::i32);
 // MachineSDNode *Move =
   //   CurDAG->getMachineNode(TOY::MOVLOi16, N, MVT::i32, ConstLo);

  // Select the low part of the immediate move, if needed.
/*  if (ImmHi) {
    SDValue ConstHi = CurDAG->getTargetConstant(ImmHi >> 16, MVT::i32);
    Move = CurDAG->getMachineNode(TOY::MOVHIi16, N, MVT::i32, SDValue(Move, 0),
                                  ConstHi);
  }
*/
  return /*Move*/;
}

void TOYDAGToDAGISel::Select(SDNode *Node) {
	 // Dump information about the Node being selected.
	DEBUG(dbgs() << "Selecting: "; Node->dump(CurDAG); dbgs() << "\n");


	if (Node->isMachineOpcode()) {
	    DEBUG(dbgs() << "== custom Opcode "; Node->dump(CurDAG); dbgs() << "\n");
	    //Node->setNodeId(-1);
	    return;
	  }
	/*switch (N->getOpcode()) {
  case ISD::Constant:
   // return SelectMoveImmediate(N);
   return ;
  } */

  SelectCode(Node);
}

/// createTOYISelDag - This pass converts a legalized DAG into a
/// TOY-specific DAG, ready for instruction scheduling.
///
FunctionPass *llvm::createTOYISelDag(TOYTargetMachine &TM,
                                     CodeGenOpt::Level OptLevel) {
  return new TOYDAGToDAGISel(TM, OptLevel);
}
