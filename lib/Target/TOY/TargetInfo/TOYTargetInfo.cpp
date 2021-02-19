#include "TOY.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

namespace llvm {
Target &getTheTOYTarget() {
      static Target TheTOYTarget;
        return TheTOYTarget;
}
}
extern "C" void LLVMInitializeTOYTargetInfo() {
      RegisterTarget<Triple::toy,
                           /*HasJIT=*/false> X(getTheTOYTarget(), "toy", "TOY",
                                                                        "TOY");
}

