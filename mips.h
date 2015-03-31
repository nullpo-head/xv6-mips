// Routines to let C code use special MIPS instructions.

#include "regs.h"

#define MAXASID 255

static int io_port_base = 0xb4000000;

static inline uchar
inb(ushort port)
{
  return *((uchar *) (io_port_base + port));
}

static inline void
insl(int port, void *addr, int cnt)
{
  /*asm volatile("cld; rep insl" :
               "=D" (addr), "=c" (cnt) :
               "d" (port), "0" (addr), "1" (cnt) :
               "memory", "cc");*/
}

static inline void
outb(ushort port, uchar data)
{
  *((uchar *) (io_port_base + port)) = data;
}

static inline void
outw(ushort port, ushort data)
{
  *((ushort *) (io_port_base + port)) = data;
}

static inline void
outsl(int port, const void *addr, int cnt)
{
  /*asm volatile("cld; rep outsl" :
               "=S" (addr), "=c" (cnt) :
               "d" (port), "0" (addr), "1" (cnt) :
               "cc");*/
}

static inline void
stosb(void *addr, int data, int cnt)
{
  int i;
  for (i = 0; i < cnt; i++) {
    *(((char *) addr) + i) = (char) data;
  }
}

static inline void
stosl(void *addr, int data, int cnt)
{
  int i;
  for (i = 0; i < cnt; i++) {
    *(((int *) addr) + i) = data;
  }
}

static inline uint
read_cop0_status(void)
{
  uint val;
  asm volatile("mfc0 %0, $12" : "=r" (val));
  return val;
}

static inline void
write_cop0_status(uint val)
{
  asm volatile("mtc0 %0, $12" : : "r" (val));
}

static inline uint
read_cop0_bad(void)
{
  uint val;
  asm volatile("mfc0 %0, $8" : "=r" (val));
  return val;
}

static inline int
is_interruptible(void)
{
  return (read_cop0_status() & (STATUS_IE | STATUS_EXL | STATUS_ERL)) == STATUS_IE;
}

static inline void
disableinterrupt(void)
{
  if (is_interruptible()) {
    write_cop0_status(read_cop0_status() | STATUS_EXL); // Assert EXL bit so that ERET can re-enable interrupt
  }
}

static inline void
enableinterrupt(void)
{
  if (!is_interruptible()) {
    write_cop0_status((read_cop0_status() & ~STATUS_KSU & ~STATUS_EXL & ~STATUS_ERL) | STATUS_IE);
  }
}

// Return old val
static inline uint
atomic_swap(volatile uint *addr, uint newval)
{
  uint oldval;
  asm volatile("ll %0, 0(%1)\n\t"
               "move $t0, %2\n\t"
               "sc $t0, 0(%1)" :
               "=&r" (oldval) :
               "r" (addr), "r" (newval) :
               "t0");
  return oldval;
}

static inline void
tlbwi(pde_t entry_hi, pte_t entry_lo01)
{
  asm volatile("mtc0 %0, $10\n\t" // COP0_ENTRYHI
               "mtc0 %1, $2\n\t"  // COP0_ENTRYLO0
               "mtc0 %2, $3\n\t"  // COP0_ENTRYLO1
               "ehb\n\t"
               "tlbwr\n\t" : :
               "r" (entry_hi), "r" ((entry_lo01 >> 32) & 0xffffffff), "r" (entry_lo01 & 0xffffffff)
               );
}

static inline void
tlbp(uint entry_hi, pte_t entry_lo01)
{
  asm volatile("mtc0 %0, $10\n\t" // COP0_ENTRYHI
               "mtc0 %1, $2\n\t"  // COP0_ENTRYLO0
               "mtc0 %2, $3\n\t"  // COP0_ENTRYLO1
               "ehb\n\t"
               "tlbp\n\t" : :
               "r" (entry_hi), "r" ((entry_lo01 >> 32) & 0xffffffff), "r" (entry_lo01 & 0xffffffff)
               );
}

static inline void
tlbr(uint *entry_hi, uint *entry_lo0, uint *entry_lo1)
{
  asm volatile("ehb\n\t"
               "tlbr\n\t"
               "mfc0 %0, $10\n\t" // COP0_ENTRYHI
               "mfc0 %1, $2\n\t"  // COP0_ENTRYL1
               "mfc0 %2, $3\n\t"  // COP0_ENTRYLO1
               "ehb" :
               "=r" (*entry_hi), "=r" (*entry_lo0), "=r" (*entry_lo1) :);
}

//PAGEBREAK: 36
// Layout of the trap frame built on the stack
struct trapframe {
  // general registers
  uint at;
  uint v0;
  uint v1;
  uint a0;
  uint a1;
  uint a2;
  uint a3;
  uint t0;
  uint t1;
  uint t2;
  uint t3;
  uint t4;
  uint t5;
  uint t6;
  uint t7;
  uint s0;
  uint s1;
  uint s2;
  uint s3;
  uint s4;
  uint s5;
  uint s6;
  uint s7;
  uint t8;
  uint t9;
  uint k0;
  uint k1;
  uint gp;
  uint sp;
  uint fp;
  uint ra;
  
  // Mul / Div special registers
  uint hi;
  uint lo;

  uint epc;
  uint error_epc;
  uint cause;
  uint status;
};
