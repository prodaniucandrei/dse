#include "systemc.h"

SC_MODULE(addition){
    sc_in<uint> nr1_pointer;
    sc_in<uint> nr2_pointer;
    sc_out<uint> result;

    SC_HAS_PROCESS(addition);

    addition(sc_module_name name);
    void compute();
};