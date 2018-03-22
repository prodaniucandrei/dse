#include "crypto_module.h"

crypto_module :: crypto_module (sc_module_name name): sc_module(name)
{
  a0_pointer.initialize(0x67452301);
  b0_pointer.initialize(0xEFCDAB89);
  c0_pointer.initialize(0x98BADCFE);
  d0_pointer.initialize(0x10325476);

  SC_METHOD(round_crypto);
  dont_initialize();
  sensitive << message_pointer << step_port;
}

void crypto_module::round_crypto()
{
    uint F;
    int g;
    uint A = a0_pointer.read();
    uint B = b0_pointer.read();
    uint C = c0_pointer.read();
    uint D = d0_pointer.read();
    int step = step_port.read();

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
        a0.write(a0_pointer.read() + A);
        b0.write(b0_pointer.read() + B);
        c0.write(c0_pointer.read() + C);
        d0.write(d0_pointer.read() + D);
    }
    else
    {
        a0_pointer.write(a0_pointer.read() + A);
        b0_pointer.write(b0_pointer.read() + B);
        c0_pointer.write(c0_pointer.read() + C);
        d0_pointer.write(d0_pointer.read() + D);
        step_port.write(step_port.read() + 1);
    }

}