#ifndef SLIP_H
#define SLIP_H
#include "stdint.h"
#include "Bytes.h"

class Slip : public Bytes
{
public:
    enum Magic {ESC=0xDB,END=0xC0,ESC_ESC=0xDD,ESC_END=0xDC};
    Slip(uint32_t size);
    virtual ~Slip();
    Slip& addCrc();
    Slip& removeCrc();

    Slip& encode();
    Slip& decode();
    bool isGoodCrc();
    Slip& frame();
    bool fill(uint8_t b);
    void reset();
    static uint16_t Fletcher16(uint8_t *begin, int length);
    static void Encode(Bytes& bytes);
    static void Decode(Bytes& bytes);
    static void AddCrc(Bytes&);
    static void Frame(Bytes&);


protected:
private:
    bool _escaped;

};

#endif // SLIP_H


