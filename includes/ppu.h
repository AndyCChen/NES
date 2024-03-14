#ifndef PPU_H
#define PPU_H

#include <stdint.h>

#define PPU_RAM_SIZE  1024 * 2

// addresses of PPU registers at 0x2000 - 0x2007 and 0x4041

#define PPU_CONTROL 0x2000
#define PPU_MASK    0x2001
#define PPU_STATUS  0x2002
#define OAM_ADDR    0x2003
#define OAM_DATA    0x2004
#define PPU_SCROLL  0x2005
#define PPU_ADDR    0x2006
#define PPU_DATA    0x2007
#define OAM_DMA     0x4041

// memory mapped addresses used by ppu to access cartridge space

#define PPU_CARTRIDGE_PATTERN_TABLE_END 0x1FFF

/**
 * Lets cpu write data to a ppu register
 * @param position address of ppu register to write to
 * @param data content that is to be written to memory
*/
void ppu_cpu_write(uint16_t position, uint8_t data);

/**
 * Lets cpu read data from a ppu register
 * @param position address of ppu register to read from
 * @returns data that is read from ppu register
*/
uint8_t ppu_cpu_read(uint16_t position);

#endif