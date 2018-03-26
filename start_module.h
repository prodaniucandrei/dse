#include "systemc.h"
#include "crypto_module.h"

SC_MODULE(start_module){
    sc_signal<char> message_ch[64];
    sc_signal<uint> signals[4];
    sc_signal<int> step_ch;

    sc_signal<uint> block_vars[4];
    //final hash channel
    sc_signal<uint> final_hash[4];

    //ctx
    int datalen, current_count;
    unsigned char data_to_send[64];
    unsigned long long number_of_bits; 

    crypto_module crypto;

    SC_HAS_PROCESS(start_module);

    start_module(sc_module_name name);
    
    void start_crypto();
    void print_result();
    void initialize();
};