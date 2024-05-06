#include "../includes/mappers/mapper_000.h"

#define CPU_CARTRIDGE_PRG_RAM_START 0x6000
#define CPU_CARTRIDGE_PRG_RAM_END   0x7FFF
#define CPU_CARTRIDGE_PRG_ROM_START 0x8000

#define PPU_CARTRIDGE_PATTERN_TABLE_END 0x1FFF

#define NAMETABLE_0_END   0x23FF
#define NAMETABLE_1_START 0x2400
#define NAMETABLE_1_END   0x27FF
#define NAMETABLE_2_START 0x2800
#define NAMETABLE_2_END   0x2BFF
#define NAMETABLE_3_START 0x2C00
#define NAMETABLE_3_END   0x2FFF


cartridge_access_mode_t mapper000_cpu_read(nes_header_t *header, uint16_t position, uint16_t *mapped_addr)
{
   cartridge_access_mode_t mode;

   if ( position >= CPU_CARTRIDGE_PRG_ROM_START )
   {
      *mapped_addr = (header->prg_rom_size > 1) ? position & 0x7FFF : position & 0x3FFF ; // rom size for mapper 0 is 32kb or 16kb
      mode = ACCESS_PRG_ROM;
   }
   else if ( position >= CPU_CARTRIDGE_PRG_RAM_START && position <= CPU_CARTRIDGE_PRG_RAM_END )
   {
      *mapped_addr = position & 0x1FFF;
      mode = ACCESS_PRG_RAM;
   }
   else
   {
      mode = NO_CARTRIDGE_DEVICE;
   }

   return mode;
}

cartridge_access_mode_t mapper000_ppu_read(nes_header_t *header, uint16_t position, uint16_t *mapped_addr)
{
   cartridge_access_mode_t mode;
   position = position & 0x3FFF; // ppu addresses only a 14 bit address space

   if ( position <= PPU_CARTRIDGE_PATTERN_TABLE_END )
   {
      *mapped_addr = position;
      mode = ACCESS_CHR_MEM;
   }
   else
   {
      position = position & 0x2FFF; // addresses 0x3000 - 0x3EFF are mirrors of 0x2000 - 0x2EFF

      if ( header->nametable_arrangement )
      {
         // vertical mirroring
         position = position & ~(0x0800);


      }
      else
      {
         // horizontal mirroring
         if (position >= 0x2000 && position <= 0x27FF)
         {
            position = position & ~(0x0400);
         }
         else
         {
            position = ( position & ~(0x0C00) ) | 0x400;
         }
      }

      *mapped_addr = position & 0x7FF;
      mode = ACCESS_VRAM;
   }

   return mode;
}

cartridge_access_mode_t mapper000_cpu_write(nes_header_t *header, uint16_t position, uint16_t *mapped_addr)
{
   cartridge_access_mode_t mode;
   (void) header;

   if ( position >= CPU_CARTRIDGE_PRG_RAM_START && position <= CPU_CARTRIDGE_PRG_RAM_END )
   {
      *mapped_addr = position & 0x1FFF;
      mode = ACCESS_PRG_RAM;
   }
   else
   {
      mode = NO_CARTRIDGE_DEVICE;
   }

   return mode;
}

cartridge_access_mode_t mapper000_ppu_write(nes_header_t *header, uint16_t position, uint16_t *mapped_addr)
{
   cartridge_access_mode_t mode;
   position = position & 0x3FFF; // ppu addresses only a 14 bit address space
   if ( position <= PPU_CARTRIDGE_PATTERN_TABLE_END )
   {  
      if ( header->chr_rom_size == 0 ) // 0 size chr-rom means chr-ram is used instead so writes are allowed
      {
         *mapped_addr = position;
         mode = ACCESS_CHR_MEM;
      }
      else
      {
         mode = NO_CARTRIDGE_DEVICE;
      }
   }
   else
   {
      position = position & 0x2FFF; // addresses 0x3000 - 0x3EFF are mirrors of 0x2000 - 0x2EFF

      if ( header->nametable_arrangement )
      {
         // vertical mirroring
         position = position & ~(0x0800);
      }
      else
      {
         // horizontal mirroring
         if (position >= 0x2000 && position <= 0x27FF)
         {
            position = position & ~(0x0400);
         }
         else
         {
            position = ( position & ~(0x0C00) ) | 0x400;
         }
      }

      *mapped_addr = position & 0x7FF;
      mode = ACCESS_VRAM;
   }

   return mode;
}