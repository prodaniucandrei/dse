#include "start_module.h"
#include "string"

start_module :: start_module(sc_module_name name) : sc_module(name), crypto("crypto"){
    
    for(int i=0;i<64;i++)
        crypto.message_pointer[i](message_ch[i]);

    for(int i=0;i<4;i++)
        crypto.ports[i](signals[i]);

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

void start_module :: start_crypto()
{
    char data[] = "hel";
    
    int datalen = strlen(data);
    int current_count=0;
    char data_to_send[64];
    unsigned long number_of_bits = 0;

    for(int i=0;i<datalen;i++)
    {
        data_to_send[current_count] = data[i];
        current_count++;
        if(current_count == 64)
        {   
            for(int i=0;i<64;i++)
                message_ch[i].write(data_to_send[i]);
            wait(a0_ch.default_event() & b0_ch.default_event() & c0_ch.default_event() & d0_ch.default_event());
            number_of_bits += 512;
            current_count = 0;
        }
    }
    
    if(current_count < 56)
    {
        data_to_send[current_count++] = 0x80;
        while(current_count < 56)
            data_to_send[current_count++] = 0x00;
    }
    else if(current_count >=56)
    {
        data_to_send[current_count++] = 0x80;
        while(datalen < 64)
            data_to_send[current_count++] = 0x00;
        for(int i=0;i<64;i++)
            message_ch[i].write(data_to_send[i]);
        wait(a0_ch.default_event() & b0_ch.default_event() & c0_ch.default_event() & d0_ch.default_event());
        memset(data_to_send, 0, 56);    
    }

    number_of_bits += datalen * 8;
    cout<<"number of bits: "<<number_of_bits<<endl;
    data_to_send[56] = number_of_bits;
    data_to_send[57] = number_of_bits >> 8;
    data_to_send[58] = number_of_bits >> 16;
    data_to_send[59] = number_of_bits >> 24;
    data_to_send[60] = number_of_bits >> 32;
    data_to_send[61] = number_of_bits >> 40;
    data_to_send[62] = number_of_bits >> 48;
    data_to_send[63] = number_of_bits >> 56;
    //int i;
    for(int i=0;i<64;i++)
        message_ch[i].write(data_to_send[i]);
}

void start_module :: print_result(){
    cout<<a0_ch.read()<<b0_ch.read()<<c0_ch.read()<<d0_ch.read()<<endl;
}