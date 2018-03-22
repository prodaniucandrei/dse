#include "start_module.h"
#include "string"

start_module :: start_module(sc_module_name name) : sc_module(name), crypto("crypto"){
    crypto.message_pointer(message_ch);
    crypto.a0_pointer(a_ch);
    crypto.b0_pointer(b_ch);
    crypto.c0_pointer(c_ch);
    crypto.d0_pointer(d_ch);
    crypto.step_port(step_ch);
    crypto.a0(a0_ch);
    crypto.b0(b0_ch);
    crypto.c0(c0_ch);
    crypto.d0(d0_ch);
    
    SC_THREAD(start_crypto);

    SC_METHOD(print_result);
    dont_initialize();
    sensitive << a0_ch.default_event()<< b0_ch.default_event()<<c0_ch.default_event()<<d0_ch.default_event();

}

void start_module :: start_crypto(){
    message_ch.write('c');
    
    print_result();    
}

void start_module :: print_result(){
    cout<< sc_time_stamp()<<a0_ch.read()<<b0_ch.read()<<c0_ch.read()<<d0_ch.read()<<endl;
}