#include "param.h"
#include "regs.h"

# Enbale MIPS dependent GAS macros.
.set macro  
.align 2

# By convention, the _start symbol specifies the ELF entry point.
.globl _start
_start:
  # Disable interrput
  mfc0   $t0, $COP0_STATUS
  li     $t1, STATUS_IE
  li     $t2, -1
  xor    $t1, $t1, $t2
  and    $t0, $t0, $t1

  # Setup IM registers preparing for enabling interrput
  li     $t1, STATUS_IM
  or     $t0, $t0, $t1
  mtc0   $t0, $COP0_STATUS
  
  # Set up the stack pointer.
  la  $sp, stack + KSTACKSIZE

  # Jump to main()
  la     $t0, main
  jr     $t0
  nop

.comm stack, KSTACKSIZE
