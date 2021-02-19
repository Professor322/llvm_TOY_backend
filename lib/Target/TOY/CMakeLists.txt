set(LLVM_TARGET_DEFINITIONS TOY.td)

tablegen(LLVM TOYGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM TOYGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM TOYGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM TOYGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM TOYGenCallingConv.inc -gen-callingconv)
tablegen(LLVM TOYGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM TOYGenMCCodeEmitter.inc -gen-emitter)
add_public_tablegen_target(TOYCommonTableGen)

add_llvm_target(TOYCodeGen
  TOYAsmPrinter.cpp  #fixed
  TOYFrameLowering.cpp #fixed
  TOYInstrInfo.cpp #fixed
  TOYISelDAGToDAG.cpp #fixed
  TOYISelLowering.cpp #fixed
  TOYMCInstLower.cpp #fixed
  TOYSubtarget.cpp #fixed
  TOYTargetMachine.cpp #fixed
  TOYSelectionDAGInfo.cpp #fixed
  TOYRegisterInfo.cpp
  )

add_subdirectory(InstPrinter)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
