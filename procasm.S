#include "regs.h"

# Enable MIPS dependent GAS macros.
.set macro  

# A fork child's very first scheduling by scheduler()
# will swtch here.  "Return" to user space.
.globl forkret
forkret:
  jal  finalizefork
  nop
  j    trapret
  nop
