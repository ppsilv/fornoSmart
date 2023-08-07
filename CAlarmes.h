#ifndef __CALARMES_H__
#define __CALARMES_H__
#include "fornoSmart.h"

class CAlarme {
  public:
    static CAlarme * getInstancia();
    uint8_t criaAlarme();
    bool verificaAlarme();
    void rodaAlarme();


    const unsigned long bflagAlarmeSignTimeout = 300;
    const char iconeAlarme = 'A';
    const uint8_t iconeLinha=1;
    const uint8_t iconeColuna=14;

protected:
  CAlarme();    

private:
    static CAlarme * instancia;
    char calarme[6];
    char charVal[16];
    unsigned long bflagAlarmeSignTime = 0;
    unsigned long alarmeTimeout=0;
    bool bflagAlarme=false;
    char  horaDoAlarme[9]={0};
    unsigned long  lalarme=0;
    int8_t key,linha,coluna;
    void setAlarme(char * alarme);
    void piscaIcone();
};

#endif
