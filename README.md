# Compressor

Compressor e Descompressor de Imagens, elaborado como projeto da disciplina Multimídia.

O objetivo principal do projeto foi colocar em prática conceitos teóricos abordados em sala de aula relacionados a técnicas de codificação de mídias.

Como saída, o compressor gera um arquivo binário que representa a imagem de modo comprimido e a taxa de compressão atingida.
O descompressor deve recebe como entrada  um arquivo binário  armazenado em disco, produzido pelo compressor.   
Como saída, o descompressor produz  uma imagem com qualidade similar à imagem original.

 As técnicas de compressão desenvolvidas foram:
 - compressão com perdas, com autilização de transformada DCT
 - compressão sem perdas utilizando codificação diferencial e por entropia.
 
 
Instrucoes de uso (linux)
----------
Compressor
----------
-> Tendo aberto o terminal/prompt, acesse o diretorio do projeto do compressor, cujo nome eh "compressor";
-> Para compilar, tendo instalado o compilador gcc, execute: 
	gcc main.c codificacao.c dct.c gerencia_memoria.c leitura_escrita.c quantizacao.c visualizacao.c ycbcr.c -lm -o comprime

-> Para executar: ./comprime "nome do arquivo".bmp
	-> Exemplo: ./comprime bird.bmp

-> Sera gerado um arquivo .bin de mesmo nome do passado por linha de comando
	-> No caso do exemplo, sera gerado o arquivo bird.bin

-> Sera exibida tambem a taxa de compressao atingida.

-------------
Descompressor
-------------
-> Tendo aberto o terminal/prompt, acesse o diretorio do projeto do descompressor, cujo nome é "descompressor";
-> Execute: gcc main.c codificacao.c desquantizacao.c gerencia_memoria.c leitura_escrita.c ycbcr.c -lm -o descomprime

	OBS: a etapa da descompressao nao compila corretamente por linha de comando. 
	     Nao conseguimos encontrar o erro, mesmo apos diversos testes.
	
	OBS 2: o código porém compila e executa corretamente quando executado a partir do CodeBlocks.



---------------------------------------------------
Descricao do metodo utilizado e Decisoes de Projeto
---------------------------------------------------

Inicialmente, os cabecalhos foram extraidos da imagem carregada e os dados referentes a imagem propriamente dita foram dividos em 
seus canais R, G e B, os quais foram convertidos para os canais Y, Cb e Cr.
Os canais Cb e Cr foram, então, subamostrados utilizando o algoritmo 4:2:0. Neste tipo de subamostragem, para cada 4 valores de Cb,
apenas 1 é mantido. Em nosso caso, escolhemos manter apenas o valor de posicao (0,0) em cada matriz 2x2 possível de ser extraída dos 
canais Cb's (sem intersecção). Cabe observar que a informacao referente aos canais Cx pode ser amostrada (comprimida) sem grandes perdas
na qualidade do resultado final pois representam a informacao de cromancia da imagem, à qual o olho humano nao eh tao sensivel. Por um 
motivo oposto o canal Y é mantido completo, dado que representa a informacao de luminancia da imagem, à qual somos sensíveis mesmo a pequenas
variações.
Após isso, o canal Y, integralmente, e os canais Cb e Cr amostrados, foram divididos em blocos 8x8. Essa dimensão foi escolhida pois,
como dito em aula, representam uma boa relação custoxbenefício entre o resultado final e o tempo de execucao.
O passo seguinte foi a aplicacao da transformada DCT, uma transformada que idealmente nao apresenta perdas, mas que, por questoes de
truncamento particulares as maquinas, podem apresentar pequenas falhas no resultado final.
Após esta etapa, foi aplicada a quantizacao, utilizando as tabelas default do padrao JPEG, nos blocos referentes à DCT, eliminando valores AC (alta energia espacial).
A etapa seguinte foi a codificacao. Os valores DC foram codificados por diferenca, enquanto os ACs foram codificados por entropia.
Finalmente, utilizando as tabelas de Huffman e de codificação AC (presente no material fornecido em aula - PDF), geramos os codigos referentes aos 
valores anteriormente calculados, assim como salvamos os mesmos, juntamente com os cabecalhos originais, no arquivo binario de saída.

A lógica pensada para a descompressao foi o inverso da lógica descrita acima.


Utilizamos principalmente estruturas de matrizes e cubos (ponteiros duplos e triplos) para manipular as matrizes e conjuntos de blocos durante o processamento.


A IDE utilizada no desenvolvimento dos projetos foi o CodeBlocks.

--------------------
Estrutura do projeto
--------------------

O projeto encontra-se dividido em 2 subprojetos: um referente ao compressor e um ao descompressor.

	-> Estrutura de arquivos do compressor:
		-> main.c: arquivo principal do subprojeto;
		-> codificacao.c: contém funcoes e tabelas referentes ao processo de codificacao;
		-> dct.c: funcoes referentes ao calculo da transoformada DCT;
		-> gerencia_memoria.c: funcoes de alocacao de memoria;
		-> leitura_escrita.c: contem funcoes para leitura e escrita de arquivos;
		-> quantizacao.c: funcoes para auxilio à etapa de quantizacao;
		-> visualizacao.c: funcoes auxiliares de visualizacao dos dados lidos;
		-> ycbcr.c: funcoes para conversao entre os espacos de cores RGB e YCbCr.

	-> Estrutura de arquivos do descompressor (contem funcoes com a logica inversa às do compressor):
		-> codificacao.c;
		-> desquantizacao.c;
		-> gerencia_memoria.c;
		-> leitura_escrita.c;
		-> main.c;
		-> ycbcr.c

OBS: todos os arquivos descritos apresentam seus correspondentes arquivos .h

-------
Taxa de compressão
-------

A taxa de compressão alcançada foi próxima de 30:1.
Porém, isso resulta em uma imagem de baixa qualidade após a descompressão.
