# Context switch
#
#   void swtch(struct context **old, struct context *new);
# 
# Save current register context in old
# and then load register context from new.

.globl swtch
swtch:
/*  movl 4(%esp), %eax
  movl 8(%esp), %edx

  # Save old callee-save registers
  pushl %ebp
  pushl %ebx
  pushl %esi
  pushl %edi

  # Switch stacks
  movl %esp, (%eax)
  movl %edx, %esp

  # Load new callee-save registers
  popl %edi
  popl %esi
  popl %ebx
  popl %ebp
  ret*/
  addiu $sp, $sp, -40

  # Save old callee-save registers and the link register
  sw $ra, 36($sp)
  sw $s8, 32($sp)
  sw $s7, 28($sp)
  sw $s6, 24($sp)
  sw $s5, 20($sp)
  sw $s4, 16($sp)
  sw $s3, 12($sp)
  sw $s2, 8($sp)
  sw $s1, 4($sp)
  sw $s0, 0($sp)

  # Switch stacks
  sw $sp, 0($a0)
  move $sp, $a1

  # Load new callee-save registers 
  lw $ra, 36($sp)
  lw $s8, 32($sp)
  lw $s7, 28($sp)
  lw $s6, 24($sp)
  lw $s5, 20($sp)
  lw $s4, 16($sp)
  lw $s3, 12($sp)
  lw $s2, 8($sp)
  lw $s1, 4($sp)
  lw $s0, 0($sp)
  addiu $sp, $sp, 40
  jr $ra
  nop
