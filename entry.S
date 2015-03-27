#include "param.h"
#include "regs.h"

# Enbale MIPS dependent GAS macros.
.set macro  
.align 2

# By convention, the _start symbol specifies the ELF entry point.
.globl _start
_start:
  mfc0   $t0, $COP0_STATUS
  # Disable interrput
  li     $t1, STATUS_IE
  or     $t0, $t1, $t1
  xor    $t0, $t0, $t1
  # Setup IM registers preparing for enabling interrput
  ori    $t0, $t0, STATUS_IM
  # COP0
  lui    $t1, STATUS_CU0 >> 16
  or     $t0, $t0, $t1
  mtc0   $t0, $COP0_STATUS
  
  # Set up the stack pointer.
  la  $sp, stack + KSTACKSIZE

  # Jump to main()
  la     $t0, main
  jr     $t0
  nop

.comm stack, KSTACKSIZE
