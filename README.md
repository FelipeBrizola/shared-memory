# shared-memory
Comunicação entre processos com memória compartilhada.

Crie dois processos que se comuniquem através de memória compartilhada no Linux. A
implementação seguirá o modelo cliente/servidor. O processo servidor irá criar um segmento de memória compartilhada usando a chamada de sistema shmget() e ficará esperando o envio de mensagens de uma aplicação cliente. O servidor deverá utilizar um semáforo (sem_t) para realizar a espera da mensagem, evitando espera ocupada. O cliente deverá sinalizar o servidor do envio da mensagem que será exibida na tela. O servidor deve ficar em loop infinito aguardando e exibindo as mensagens, enquanto que o cliente envia uma mensagem e é encerrado. Maiores explicações sobre o uso da chamada shmget() pode ser visto no link abaixo. Ainda, o link fornece um exemplo cliente/servidor que pode ser usado como ponto de partida.
