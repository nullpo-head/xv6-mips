// Routines to let C code use special MIPS instructions.

#include "regs.h"

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

struct segdesc;

static inline void
lgdt(struct segdesc *p, int size)
{
  /*volatile ushort pd[3];

  pd[0] = size-1;
  pd[1] = (uint)p;
  pd[2] = (uint)p >> 16;

  asm volatile("lgdt (%0)" : : "r" (pd));*/
}

struct gatedesc;

static inline void
lidt(struct gatedesc *p, int size)
{
  /*volatile ushort pd[3];

  pd[0] = size-1;
  pd[1] = (uint)p;
  pd[2] = (uint)p >> 16;

  asm volatile("lidt (%0)" : : "r" (pd));*/
}

static inline void
ltr(ushort sel)
{
  //asm volatile("ltr %0" : : "r" (sel));
}

static inline uint
readeflags(void)
{
  uint eflags;
  //asm volatile("pushfl; popl %0" : "=r" (eflags));
  return eflags;
}

static inline uint
readcop0(uint regnum)
{
  uint val;
  asm volatile("mfc0 %0, %1" : "=r" (val) : "r" (regnum));
  return val;
}

static inline void
writecop0(uint regnum, uint val)
{
  asm volatile("mtc0 %0, %1" : : "r" (val), "r" (regnum));
}

static inline void
loadgs(ushort v)
{
  //asm volatile("movw %0, %%gs" : : "r" (v));
}

static inline void
cli(void)
{
  //asm volatile("cli");
}

static inline void
disableinterrupt(void)
{
  writecop0(COP0_STATUS, readcop0(COP0_STATUS)&(~COP0_STATUS_IE));
}

static inline void
enableinterrupt(void)
{
  writecop0(COP0_STATUS, readcop0(COP0_STATUS)|COP0_STATUS_IE);
}

static inline void
sti(void)
{
  //asm volatile("sti");
}

static inline uint
xchg(volatile uint *addr, uint newval)
{
  uint result;
  
  // The + in "+m" denotes a read-modify-write operand.
  /*asm volatile("lock; xchgl %0, %1" :
               "+m" (*addr), "=a" (result) :
               "1" (newval) :
               "cc");*/
  return result;
}

// Return old val
static inline uint
atomic_swap(volatile uint *addr, uint newval)
{
  uint oldval;
  asm volatile("ll %0, 0(%1)\n\t"
               "addu $8, $0, %2\n\t"
               "sc $8, 0(%1)" :
               "=r" (oldval) :
               "r" (addr), "r" (newval) :
               "8");
  return oldval;
}


static inline uint
rcr2(void)
{
  uint val;
  //asm volatile("movl %%cr2,%0" : "=r" (val));
  return val;
}

static inline void
lcr3(uint val) 
{
  //asm volatile("movl %0,%%cr3" : : "r" (val));
}

//PAGEBREAK: 36
// Layout of the trap frame built on the stack by the
// hardware and by trapasm.S, and passed to trap().
struct trapframe {
  // registers as pushed by pusha
  uint edi;
  uint esi;
  uint ebp;
  uint oesp;      // useless & ignored
  uint ebx;
  uint edx;
  uint ecx;
  uint eax;

  // rest of trap frame
  ushort gs;
  ushort padding1;
  ushort fs;
  ushort padding2;
  ushort es;
  ushort padding3;
  ushort ds;
  ushort padding4;
  uint trapno;

  // below here defined by x86 hardware
  uint err;
  uint eip;
  ushort cs;
  ushort padding5;
  uint eflags;

  // below here only when crossing rings, such as from user to kernel
  uint esp;
  ushort ss;
  ushort padding6;
};
