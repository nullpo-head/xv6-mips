// This file contains definitions for the 
// MIPS register related constants.

// Coprocessor 0 register names
#define COP0_STATUS    12
#define COP0_CAUSE     13
#define COP0_EPC       14
#define COP0_ERROREPC  30
#define COP0_BADVADDR  8
#define COP0_ENTRYLO0  2
#define COP0_ENTRYLO1  3


// Status register flags
#define STATUS_IE   0x00000001
#define STATUS_EXL  0x00000002
#define STATUS_ERL  0x00000004
#define STATUS_KSU  0x00000018
#define STATUS_IM   0x0000ff00
#define STATUS_BEV  0x00400000

// Cause register flags
#define CAUSE_EXC   0x0000007c
