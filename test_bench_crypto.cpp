#include <systemc.h>
#include "start_module.h"

int sc_main(int argc, char **argv)
{
  start_module start_module("start111");
  
  sc_start(10000, SC_MS);

  return 0;
}

