#ifndef __CRtc_H__
#define __CRtc_H__
#include "fornoSmart.h"
#include "CKeypad.h"

class CRtc {
public:
    static CRtc * getInstancia();
    void acertoRtc();
    void getTime();
    void getDate();

protected:
    CRtc();

private:
    static CRtc * instancia;
    char cdata[11];
    char chora[11];  
    char dow;
    int8_t linha = 0;
    int8_t coluna= 0;
    int8_t key;
    
    void acertaData();
    void acertaHora();
    void acertaDow();
    void gravaRtc();
};

#endif
