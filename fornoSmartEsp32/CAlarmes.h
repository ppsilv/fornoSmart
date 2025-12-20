#ifndef __CALARMES_H__
#define __CALARMES_H__
#include "fornoSmart.h"
#include "CTimer.h"

class CAlarme {
  public:
    static CAlarme * getInstancia();
    uint8_t criaAlarme();
    bool verificaAlarme();
    void rodaAlarme();
    bool getAlarmeStatus();
    char * getHoradoAlarme();
    void setAlarm(bool status);

    const char iconeAlarme = 'A';
    const uint8_t iconeLinha=1;
    const uint8_t iconeColuna=14;
    void piscaIcone(uint8_t status);

protected:
  CAlarme();    

private:
    static CAlarme * instancia;
    CTimer alarmeTimeout;
    char calarme[6];
    uint32_t qtd_minutos=0;

    //unsigned long alarmeTimeout=0;
    bool bflagAlarme=false;
    char  horaDoAlarme[9]={0};
    //unsigned long  lalarme=0;
    int key,linha,coluna;
    void setAlarme(uint16_t alarme);
};

#endif
