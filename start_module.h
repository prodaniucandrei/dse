#include "systemc.h"
#include "crypto_module.h"

SC_MODULE(start_module){
    sc_signal<char> message_ch[64];
    sc_signal<uint> signals[4];
    sc_signal<int> step_ch;

    //final hash channel
    sc_signal<uint> a0_ch;
    sc_signal<uint> b0_ch;
    sc_signal<uint> c0_ch;
    sc_signal<uint> d0_ch;


    crypto_module crypto;

    SC_HAS_PROCESS(start_module);

    start_module(sc_module_name name);
    
    void start_crypto();
    void print_result();

};