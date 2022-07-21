# Traballho de Redes: Chatroom Application

## Integrantes do grupo:

* Beatriz Aparecida Diniz nUSP: 11925430
* Bruna Azevedo Garcia nUSP: 11381789
* Yann Amado Nunes Costa nUSP: 10746943

## Informações sobre onde foi compilado

Código rodado no Ubuntu versão 22.04 LS.
O compilador utilizado foi o terminal do linux com o gcc.

## Guia para rodar o chat

Para instalar a biblioteca pthread: sudo apt-get install libpthread-stubs0-dev


**Para rodar o servidor:**
	make server

	make runServer


**Para rodar o cliente:**
	make client
	
	make runClient

Para rodar a aplicação, duas janelas de inicial devem ser inicializadas, uma pro servidor e outra pro cliente
Depois, basta dar os comandos make e depois run de seus respectivos.

## Comandos

* /connect - Estabelece a conexão com o servidor;
* /nickname apelidoDesejado - O cliente passa a ser reconhecido pelo apelido especificado;
* /join nomeCanal - Entra no canal;
* /quit ou Ctrl+D - O cliente fecha a conexão e fecha a aplicação;
* /ping - O servidor retorna "pong"assim que receber a mensagem.

## Comandos apenas para administradores de canais:

* /kick nomeUsurio - Fecha a conexão de um usuário especificado
* /mute nomeUsurio - Faz com que um usuário não possa enviar mensagens neste canal;
* /unmute nomeUsurio - Retira o mute de um usuário;
* /whois nomeUsurio - Retorna o endereço IP do usuário **apenas para o administrador**.

## Link para o vídeo mostrando o funcionamento:

https://drive.google.com/file/d/1oUh-cGWJdzKS1DuNRmi_SGrKwKIyJrCE/view?usp=sharing




