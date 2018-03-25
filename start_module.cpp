#include "start_module.h"
#include "string"

start_module :: start_module(sc_module_name name) : sc_module(name), crypto("crypto"){
    
    for(int i=0;i<64;i++)
        crypto.message_pointer[i](message_ch[i]);

    for(int i=0;i<4;i++)
        crypto.ports[i](signals[i]);

    crypto.step_port(step_ch);

    for(int i=0;i<4;i++)
        crypto.result_hash[i](final_hash[i]);
    
    for(int i=0;i<4;i++)
        crypto.vars_port[i](block_vars[i]);

    SC_THREAD(start_crypto);

    SC_METHOD(print_result);
    dont_initialize();
    sensitive << final_hash[0].default_event()<< final_hash[1].default_event()<<final_hash[2].default_event()<<final_hash[3].default_event();

    SC_METHOD(initialize);
    dont_initialize();

}

void start_module :: initialize()
{
    block_vars[0].write(0x67452301);
    block_vars[1].write(0xEFCDAB89);
    block_vars[2].write(0x98BADCFE);
    block_vars[3].write(0x10325476);
}

void start_module :: start_crypto()
{
    char data[] = "hel";
    
    datalen = strlen(data);
    current_count=0;
    number_of_bits = 0;
    
    initialize();
    for(int i=0;i<datalen;i++)
    {
        data_to_send[current_count] = data[i];
        current_count++;
        if(current_count == 64)
        {   
            for(int i=0;i<64;i++)
                message_ch[i].write(data_to_send[i]);
            wait(final_hash[0].default_event() & final_hash[1].default_event() & final_hash[2].default_event() & final_hash[3].default_event());
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
        wait(final_hash[0].default_event() & final_hash[1].default_event() & final_hash[2].default_event() & final_hash[3].default_event());
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
    // block_vars[0].write(final_hash[0].read());
    // block_vars[1].write(final_hash[1].read());
    // block_vars[2].write(final_hash[2].read());
    // block_vars[3].write(final_hash[3].read());
    cout<<final_hash[0].read()<<final_hash[1].read()<<final_hash[2].read()<<final_hash[3].read()<<endl;
}