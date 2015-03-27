#include "regs.h"
#include "mmu.h"

// TODO: add noat
# Enable MIPS dependent GAS macros.
.set macro  

  # TLB miss handling
.globl tlbrefill
tlbrefill:
  .set   push
  .set   noat
  mfc0   $k0, $COP0_BADVADDR
  lui    $k1, %hi(curpgdir)
  lw     $k1, %lo(curpgdir)($k1)
  srl    $k0, $k0, 22
  sll    $k0, $k0, 2
  addu   $k1, $k0, $k1
  lw     $k1, 0($k1)
  srl    $k1, $k1, 12
  sll    $k1, $k1, 12
  mfc0   $k0, $COP0_BADVADDR
  srl    $k0, $k0, 13 - 4
  andi   $k0, $k0, 0x1ff0
  addu   $k1, $k0, $k1
  lw     $k0, 0($k1)
  mtc0   $k0, $COP0_ENTRYLO1
  lw     $k0, 4($k1)
  mtc0   $k0, $COP0_ENTRYLO0
  ehb
  tlbwr
  eret
  .set   pop

.globl tlbrefill_end
tlbrefill_end:

  # General exceptions, which will be copied to offset + 0x180 in tvinit
.globl gentraps
gentraps:
  .set push
  .set noat
  # just jump to jtrap
  mfc0 $k1, $COP0_EPC
  la $k0, jtrap
  jr $k0
  nop
  .set pop

.globl gentraps_end
gentraps_end:

.globl jtrap
jtrap:
  .set push
  .set noat
  # switch stack if there is proc
  addu $k1, $zero, $sp      # save current sp in k1
  lui  $k0, %hi(proc)
  lw   $k0, %lo(proc)($k0)
  beq  $k0, $zero, build_tf # if proc == 0 then go to build_tf
  lw   $k0, 8($k0)          # $k0 = proc->kstack
  addu $sp, $zero, $k0

build_tf:
  # Build trap frame.
  addiu $sp, $sp, -148
  sw $at, 0($sp)
  sw $v0, 4($sp)
  sw $v1, 8($sp)
  sw $a0, 12($sp)
  sw $a1, 16($sp)
  sw $a2, 20($sp)
  sw $a3, 24($sp)
  sw $t0, 28($sp)
  sw $t1, 32($sp)
  sw $t2, 36($sp)
  sw $t3, 40($sp)
  sw $t4, 44($sp)
  sw $t5, 48($sp)
  sw $t6, 52($sp)
  sw $t7, 56($sp)
  sw $s0, 60($sp)
  sw $s1, 64($sp)
  sw $s2, 68($sp)
  sw $s3, 72($sp)
  sw $s4, 76($sp)
  sw $s5, 80($sp)
  sw $s6, 84($sp)
  sw $s7, 88($sp)
  sw $t8, 92($sp)
  sw $t9, 96($sp)
  sw $gp, 108($sp)
  sw $k1, 112($sp)          # previous stack pointer is stored in k1
  sw $fp, 116($sp)
  sw $ra, 120($sp)

  mfhi $k0
  mflo $k1
  sw $k0, 124($sp)
  sw $k1, 128($sp)

  mfc0 $t0, $COP0_EPC
  mfc0 $t1, $COP0_ERROREPC
  mfc0 $t2, $COP0_CAUSE
  mfc0 $t3, $COP0_STATUS
  sw $t0, 132($sp)
  sw $t1, 136($sp)
  sw $t2, 140($sp)
  sw $t3, 144($sp)
  
  # Call trap(tf), where tf=%esp
  move $a0, $sp
  jal trap
  nop

  # Return falls through to trapret...
.globl trapret
trapret:
  # No need to restore status, k0 and k1. 
  lw $t0, 132($sp)
  lw $t1, 136($sp)
  mtc0 $t0, $COP0_EPC
  mtc0 $t1, $COP0_ERROREPC

  lw $at, 0($sp)
  lw $v0, 4($sp)
  lw $v1, 8($sp)
  lw $a0, 12($sp)
  lw $a1, 16($sp)
  lw $a2, 20($sp)
  lw $a3, 24($sp)
  lw $t0, 28($sp)
  lw $t1, 32($sp)
  lw $t2, 36($sp)
  lw $t3, 40($sp)
  lw $t4, 44($sp)
  lw $t5, 48($sp)
  lw $t6, 52($sp)
  lw $t7, 56($sp)
  lw $s0, 60($sp)
  lw $s1, 64($sp)
  lw $s2, 68($sp)
  lw $s3, 72($sp)
  lw $s4, 76($sp)
  lw $s5, 80($sp)
  lw $s6, 84($sp)
  lw $s7, 88($sp)
  lw $t8, 92($sp)
  lw $t9, 96($sp)
  lw $gp, 108($sp)
  lw $fp, 116($sp)
  lw $ra, 120($sp)

  lw $sp, 112($sp)

  eret
  .set pop
