# Initial process execs /init.

#include "syscall.h"
#include "traps.h"

.set macro

# exec(init, argv)
.globl start
start:
  la $a0, init
  la $a1, argv
  li $v0, SYS_exec
  syscall

# for(;;) exit();
exit:
  li $v0, SYS_exit
  syscall
  j exit

# char init[] = "/init\0";
init:
  .string "/init\0"

# char *argv[] = { init, 0 };
.align 2
argv:
  .long init
  .long 0

