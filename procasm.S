#include "regs.h"

# Enable MIPS dependent GAS macros.
.set macro  

# A fork child's very first scheduling by scheduler()
# will swtch here.  "Return" to user space.
.globl forkret
forkret:
  jal  finalizefork
  nop
  mfc0  $t0, $COP0_STATUS
  ori   $t0, $t0, 0x10        # Set KSU to user level.
  ori   $t0, $t0, STATUS_EXL  # Set EXL bit to keep the kernel privilege until eret.
  ori   $t0, $t0, STATUS_IE
  mtc0  $t0, $COP0_STATUS 
  j    trapret
  nop
