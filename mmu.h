// This file contains definitions for the 
// MIPS memory management unit (MMU).

//PAGEBREAK!
// A virtual address 'la' has a three-part structure as follows:
//
// +--------10------+-------09-----+1+---------12----------+
// | Page Directory |   Page Table | | Offset within Page  |
// |      Index     |      Index   | |                     |
// +----------------+--------------+-+---------------------+
//  \--- PDX(va) --/ \-- PTX(va) -/ 

// page directory index
#define PDX(va)         (((uint)(va) >> PDXSHIFT) & 0x3FF)

// page table index
#define PTX(va)         (((uint)(va) >> PTXSHIFT) & 0x1FF)

// EntryLo 0 or EntryLo1 ?
#define ELX(va)         ((uint)(((uint)(va) >> ELXSHIFT) & 0x1))

// construct virtual address from indexes and offset
#define PGADDR(d, t, o) ((uint)((d) << PDXSHIFT | (t) << PTXSHIFT | (o)))

// construct PTE from EntryLo0 and EntryLo1
#define PTE(e0, e1)     ((ulonglong) (((ulonglong)(e0) << 32) | (e1)))

// Page directory and page table constants.
#define NPDENTRIES      1024    // # directory entries per page directory
#define NPTENTRIES      1024    // # PTEs per page table
#define PGSIZE          4096    // bytes mapped by a page

#define PGSHIFT         12      // log2(PGSIZE)
#define PTXSHIFT        13      // offset of PTX in a virtual address
#define PDXSHIFT        22      // offset of PDX in a virtual address
#define ELXSHIFT        12      // offset of ELX in a virtual address

#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE-1))

// EntryLo register flags
#define ELO_G           0x001   // Global
#define ELO_V           0x002   // Vaild
#define ELO_D           0x004   // Dirty
#define ELO_C           0x018   // Cache Control

#define ELO_ADDR(elo)   (((uint)(elo) & 0x03FFFFC0) << 6)
#define ELO_FLAGS(elo)  ((uint)(elo) & 0x3F)

#define PDE_ADDR(pde)   ((pde) & ~0xFFF)
#define EHI_ASID(ehi)   ((uchar)(ehi) & 0xFF)

// Extract EntryLoN from PTE
#define PTE_ELO(pte, n) ((uint)((pte) >> (32 * (1 - (n)))) & 0xFFFFFFFF)

#define ASID_MATCH(asid, pde, pte) (EHI_ASID(pde) == (asid) || (((pte) & ELO_G) && ((pte >> 32) & (ELO_G))))
