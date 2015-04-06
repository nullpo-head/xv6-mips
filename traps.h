// MIPS trap and interrupt constants.
//
// Processor-defined:
#define EXC_INT          0         // Interrupt
#define EXC_MOD          (1 << 2)  // TLB modification exception
#define EXC_TLBL         (2 << 2)  // TLB exception (load or instruction fetch)
#define EXC_TLBS         (3 << 2)  // TLB exception (strore)
#define EXC_SYSCALL      (8 << 2)  // system call
#define EXC_OV           (12 << 2) // Arithmetic overflow exception

#define T_IRQ0          32      // IRQ 0 corresponds to int T_IRQ

#define IRQ_TIMER        0
#define IRQ_KBD          1
#define IRQ_CASCADE      2
#define IRQ_COM1         4
#define IRQ_SPURIOUS_MS  7
#define IRQ_IDE         14
#define IRQ_ERROR       19
#define IRQ_SPURIOUS_SL 31

// Exception entry points
#define EV_TLBREFILL  0x80000000
#define EV_OTHERS     0x80000180 
