#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "mips.h"
#include "traps.h"
#include "spinlock.h"

// Interrupt descriptor table (shared by all CPUs).
struct gatedesc idt[256];
extern uint vectors[];  // in vectors.S: array of 256 entry pointers
struct spinlock tickslock;
uint ticks;

void
tvinit(void)
{
  // Set vector base address.
  write_cop0_status(read_cop0_status() & ~STATUS_BEV);

  // TLB refill
  memmove((void *)EV_TLBREFILL, tlbrefill, ((uint)tlbrefill_end - (uint)tlbrefill));

  // general traps
  memmove((void *)EV_OTHERS, gentraps, ((uint)gentraps_end - (uint)gentraps));
  
  initlock(&tickslock, "time");
}

//PAGEBREAK: 41
void
trap(struct trapframe *tf)
{
  uint exccode = tf->cause & CAUSE_EXC;
  if(exccode == EXC_SYSCALL){
    if(proc->killed)
      exit();
    proc->tf = tf;
    syscall();
    if(proc->killed)
      exit();
    return;
  }

  if(exccode == EXC_INT){
    uint irq = picgetirq();
    if (irq != IRQ_TIMER) {
        cprintf("interrupt:%d\n", irq);
    }
    switch(irq){
    case IRQ_TIMER:
      if(cpu->id == 0){
        acquire(&tickslock);
        ticks++;
        wakeup(&ticks);
        release(&tickslock);
      }
      break;
    case IRQ_IDE:
      ideintr();
      break;
    case IRQ_IDE+1:
      // Bochs generates spurious IDE1 interrupts.
      break;
    case IRQ_KBD:
      kbdintr();
      break;
    case IRQ_COM1:
      uartintr();
      break;
    case IRQ_SPURIOUS_MS:
    case IRQ_SPURIOUS_SL:
      cprintf("cpu%d: spurious interrupt at %x\n",
          cpu->id, tf->epc);
      break;
    }
    picsendeoi(irq);
  } else {
    if(proc == 0 || (tf->status & STATUS_KSU) == 0){
      // In kernel, it must be our mistake.
      cprintf("unexpected trap %d from cpu %d eip %x (cr2=0x%x)\n",
              tf->cause, cpu->id, tf->epc, rcr2());
      panic("trap");
    }
    // In user space, assume process misbehaved.
    cprintf("pid %d %s: trap %d on cpu %d "
            "eip 0x%x addr 0x%x--kill proc\n",
            proc->pid, proc->name, tf->cause, cpu->id, tf->epc, 
            rcr2());
    proc->killed = 1;
  }

  // Force process exit if it has been killed and is in user space.
  // (If it is still executing in the kernel, let it keep running 
  // until it gets to the regular system call return.)
  if(proc && proc->killed && (tf->status & STATUS_KSU))
    exit();

  // Force process to give up CPU on clock tick.
  // If interrupts were on while locks held, would need to check nlock.
  if(proc && proc->state == RUNNING && tf->cause == T_IRQ0+IRQ_TIMER)
    yield();

  // Check if the process has been killed since we yielded
  if(proc && proc->killed && (tf->status & STATUS_KSU))
    exit();
}
