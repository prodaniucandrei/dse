#include "addition_module.h"

addition :: addition(sc_module_name name):sc_module(name)
{
    result.initialize((uint)0);
    
    SC_METHOD(compute);
    dont_initialize();
    sensitive << nr1_pointer << nr2_pointer;
}

void addition::compute()
{
    result.write(nr1_pointer.read() + (uint)5);
}