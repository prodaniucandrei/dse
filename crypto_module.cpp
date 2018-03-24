#include "crypto_module.h"

crypto_module :: crypto_module (sc_module_name name): sc_module(name)
{
    ports[0].initialize(0x67452301);
    ports[1].initialize(0xEFCDAB89);
    ports[2].initialize(0x98BADCFE);
    ports[3].initialize(0x10325476);

    SC_METHOD(round_crypto);
    dont_initialize();
    sensitive << step_port;;
    for(int i=0;i<64;i++)
        sensitive << message_pointer[i];
}

void crypto_module::round_crypto()
{
    uint F;
    int g;
    uint A = ports[0].read();
    uint B = ports[1].read();
    uint C = ports[2].read();
    uint D = ports[3].read();
    int step = step_port.read();
    uint m[16], i, j;
    
    if(step == 0)
    {
        for(int z=0;z<64;z++)
            data[z] = message_pointer[z].read();
            
        cout<<"Asta ar trebui sa fie 3:"<<strlen(data)<<endl;
        int datalen = strlen(data);
       
        for (i = 0, j = 0; i < 16; ++i, j += 4)
            m[i] = (data[j]) + (data[j + 1] << 8) + (data[j + 2] << 16) + (data[j + 3] << 24);
        cout<<"Asta ar trebui sa fie mai mult:"<<strlen(data)<<endl<<m<<endl;
    }

    if(step >=0 && step<=15)
    {
        F = ((B & C) | (~B & D));
        g = step;
    }
    else if(step >=16 && step<=31)
    {
        F = ((D & B) | (~D & C));
        g = (5 * step + 1) % 16;
    }
    else if(step >=32 && step <=47)
    {
        F = B ^ C ^ D;
        g = (3 * step + 5) % 16;
    }
    else if(step>=48 && step<=63)
    {
        F = C ^ (B | ~D);
        g = (7 * step) % 16;
    }

    F = F + A + k[step] + m[g];
    A = D;
    D = C;
    C = B;
    B = B + ((F << s[step]) | (F >> (32 - s[step])));

    cout << step<<endl;

    if(step == 63)
    {
        cout<<"finish"<<endl;
        a0.write(ports[0].read() + A);
        b0.write(ports[1].read() + B);
        c0.write(ports[2].read() + C);
        d0.write(ports[3].read() + D);
    }
    else
    {
        ports[0].write(ports[0].read() + A);
        ports[1].write(ports[1].read() + B);
        ports[2].write(ports[2].read() + C);
        ports[3].write(ports[3].read() + D);
        step_port.write(step_port.read() + 1);
    }

}