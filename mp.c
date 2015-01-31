// Multiprocessor support
// We do not support multiprocessor now, so we just do some fake initialization.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mp.h"
#include "mips.h"
#include "mmu.h"
#include "proc.h"

struct cpu cpus[NCPU];
struct cpu *cpu = &cpus[0];  // &cpus[cpunum()]
int ismp;
int ncpu;
uchar ioapicid;

void
mpinit(void)
{
  ncpu = 1;
  cpus[ncpu].id = ncpu;
  ioapicid = 0;
  ismp = 0;
}
