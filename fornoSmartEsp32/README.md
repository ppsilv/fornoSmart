# fornoSmart
Reforma do forno marca smart,

2 anos após a última compilação eu volto a esse programa

1 - Início
  * Inicialização do lcd.
  * Inicializaçãõ do OTA.
  * Wire class begin.
  * Serial begin.
  * Inicialização do sensor.
  * Inicialização do relógio.
  * Inicialização das resistencias.
  * Inicialização das threads.

2 - No loop
    Chama o gerenciador do OTA
  2.1 - Estado TELAINICIAL
        * Mostra a tela inicial
        * DATA VERSÂO
        * HORA Press *
        * Lê o teclado e verifica
          se houve tecla pressionada
          se sim, verifica se é asterisco
          se sim, estado troca para TELAMENU1 
  2.2 - Estado TELAMENU1
        * Troca a tela para TELAMENU1
        * F1=Timer F2=Temp
        * Esc=Fim  Escolha
        * Se Esc estado= TELAINICIAL
        * Se foi teclado F1 estado = EDICAOTIMER
        * Se foi teclado F2 estado = EDICAOTEMP
  2.3 - Estado EDICAOTIMER
        * Mostra tela edição de temperatura
        * Quanto Tempo?
        * >
        * Edita o valor digitado
        * Quando enter for digitado salva o 
          valor digitado e estado = EDICAOTEMP
  2.4 - Estado EDICAOTEMP
        * Mostra a tela de edição de temperatura
        * Qual temp?
        * >
        * Edita o valor digitado
        * Quando enter for digitado salva o 
          valor digitado e estado = AQUECENDO
  2.5 - Estado AQUECENDO
        * Mostra a tela de aquecimento da temperatura
        * Aguarde:
        * Pisca AQUECENDO
        * Verifica se a temperatura atingiu o patamar
          temperatura_para_assar - (10% de temperatura_para_assar)
        * Se atingiu estado = ASSANDO  
  2.6 - Estado ASSANDO
        * Mostra a tela de cocção( assando a comida).
        * DATA
        * HORA (piscando o tempo restante.)
        * Tempo acabou?
        * Se sim 
            - Desliga as resistências.
            - Apita fim
            - estado = TELAINICIAL
            - tempo_para_assar = 0
            - temperatura_para_assar = 0
            

