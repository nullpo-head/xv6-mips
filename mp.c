// Multiprocessor support
// Search memory for MP description structures.
// http://developer.intel.com/design/pentium/datashts/24201606.pdf

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mp.h"
#include "mips.h"
#include "mmu.h"
#include "proc.h"

struct cpu cpus[NCPU];
int ismp;
int ncpu;
uchar ioapicid;

void
mpinit(void)
{
  // Didn't like what we found; fall back to no MP.
  ncpu = 1;
  cpus[ncpu].id = ncpu;
  // ncpu++;
  ioapicid = 0;
}
