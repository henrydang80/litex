#ifndef __HW_MEM_H
#define __HW_MEM_H

#define FLASH_OFFSET_STANDBY_BITSTREAM	0x00000000 /* 640k */

#define FLASH_OFFSET_RESCUE_BITSTREAM	0x000A0000 /* 1536k */
#define FLASH_OFFSET_RESCUE_BIOS	0x00220000 /* 128k */
#define FLASH_OFFSET_RESCUE_APP		0x00240000 /* 4736k */

#define FLASH_OFFSET_REGULAR_BITSTREAM	0x006E0000 /* 1536k */
#define FLASH_OFFSET_REGULAR_BIOS	0x00860000 /* 128k */
#define FLASH_OFFSET_REGULAR_APP	0x00880000 /* remaining space (24064k) */

#define FLASH_OFFSET_MAC_ADDRESS	0x002200E0 /* within rescue BIOS */

#define SDRAM_BASE			0x40000000

#endif /* __HW_MEM_H */
