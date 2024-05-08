#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "glad_loader/include/glad/glad.h"
#include "SDL.h"

#include "includes/cpu.h"
#include "includes/ppu.h"
#include "includes/cartridge.h"
#include "includes/log.h"
#include "includes/display.h"

bool nestest_log_flag = false;

int main(int argc, char *argv[])
{
   (void) argc;
   (void) argv;

   if ( !display_init() || !cartridge_load( argv[1] ) || !ppu_load_palettes("./ntscpalette.pal") )
   {
      return EXIT_FAILURE;
   }

   cpu_init();
   nestest_log_open();

   bool done = false;
   while (!done)
   {
      display_clear();
      display_process_event(&done);   

      for (int i = 0; i < 60; ++i) 
      {
         cpu_emulate_instruction();
      }

      display_render(); 
      display_update();
   }
 
   nestest_log_close();
   
   cartridge_free_memory();
   display_shutdown();

   return 0;
}
