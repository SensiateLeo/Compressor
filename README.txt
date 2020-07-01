*****************************************
|	      Integrantes:		|
*****************************************
|   Felipe Manfio Barbosa - 9771640	|
|     Leonardo Sensiate - 9771571	|
*****************************************

	       ***********
	       | Projeto |
*****************************************
|     Compressor/Descompressor JPEG     |
*****************************************

Compressor e Descompressor de Imagens, elaborado como projeto da disciplina Multimídia.

O objetivo principal do projeto foi colocar em prática conceitos teóricos abordados em sala de aula relacionados a técnicas de codificação de mídias.

Como saída, o compressor gera um arquivo binário que representa a imagem de modo comprimido e exibe a taxa de compressão atingida.
O descompressor deve receber como entrada um arquivo binário armazenado em disco, produzido pelo compressor.
Como saída, o descompressor produz uma imagem com extensão bmp, resultante da descompressão do arquivo binário.


=========================
Instrucoes de uso (linux)
=========================
----------
Compressor
----------
-> Tendo aberto o terminal/prompt, acesse o diretório do projeto do compressor, cujo nome é "compressor";
-> Para compilar, tendo instalado o compilador gcc, execute:
	gcc main.c codificacao.c dct.c gerencia_memoria.c leitura_escrita.c quantizacao.c visualizacao.c ycbcr.c -lm -o comprime

-> Para executar: ./comprime "nome do arquivo".bmp
	-> Exemplo: ./comprime imagem.bmp

-> Será gerado um arquivo .bin de mesmo nome do passado por linha de comando
	-> No caso do exemplo, será gerado o arquivo "imagem.bin"

-> Será exibida tambám a taxa de compressão atingida.

-------------
Descompressor
-------------
-> Tendo aberto o terminal/prompt, acesse o diretório do projeto do descompressor, cujo nome é "descompressor";
-> Execute: gcc main.c codificacao.c desquantizacao.c gerencia_memoria.c leitura_escrita.c ycbcr.c -lm -o descomprime

-> Para executar: ./descomprime "nome do arquivo".bin
	-> Exemplo: ./descomprime imagem.bin

-> Será gerado um arquivo .bmp de mesmo nome do passado por linha de comando
	-> No caso do exemplo, será gerado o arquivo imagem.bmp


===================================================
Descrição do método utilizado e Decisões de Projeto
===================================================

Inicialmente, os cabeçalhos foram extraídos da imagem carregada e os dados referentes a imagem propriamente dita foram divididos em
seus canais R, G e B. Posteriormente, foi realizada a conversão destes para a representação em componentes de luminância (Y) e cromância (Cb e Cr).
Os canais Cb e Cr foram, então, subamostrados utilizando o algoritmo 4:2:0. Neste tipo de subamostragem, para cada 4 valores de cromância (canais Cb e Cr), apenas 1 é mantido. Em nosso caso, escolhemos manter apenas o valor de posição (0,0) em cada matriz 2x2 (sem intersecção) possível de ser extraída dos canais de cromância. Cabe observar que a informação referente aos canais Cx pode ser amostrada (comprimida) sem grandes perdas
na qualidade do resultado final pois o olho humano não é tão sensível à informação de cromância. Pelo
motivo oposto, o canal Y é mantido completo, dado que representa a informação de luminância da imagem, à qual somos sensíveis, mesmo a pequenas
variações.
Após isso, o canal Y, integralmente, e os canais Cb e Cr amostrados, foram divididos em blocos 8x8. Essa dimensão foi escolhida pois,
como dito em aula, representam uma boa relação CUSTOxBENEFÍCIO entre o resultado final e o tempo de execução.
O passo seguinte foi a aplicação da transformada DCT, uma transformada que idealmente não apresenta perdas, mas que, por questões de
truncamento particulares às maquinas, podem apresentar perdas no resultado final.
Após esta etapa, foi aplicada a quantização nos blocos referentes à DCT, utilizando as tabelas default do padrão JPEG.
O processo de quantização elimina os valores AC dos blocos, transformando os mesmos em zeros.
Valores AC podem ser eliminados por representam regiões de informação visual com grande variação. Como nossos olhos são pouco sensíveis a mudanças em
regiões com alta taxa de variação, não somos sensíveis ao resultado final após a remoção dos mesmos.
A etapa seguinte foi a codificação. Os valores DC foram codificados por diferença, enquanto os ACs foram codificados por entropia. Utilizando as tabelas de Huffman e de codificação AC (presente no material fornecido em aula - PDF), geramos os códigos referentes aos valores anteriormente calculados.
Finalmente, salvamos tais códigos, juntamente com os cabeçalhos originais, no arquivo binário de saída.

A lógica pensada para a descompressão foi o inverso da lógica descrita acima, e é resumida a seguir:
1 - Leitura/decodificação do cabeçalho e códigos presentes no arquivo comprimido;
2 - Estruturação dos valores lidos em blocos 8x8;
3 - Desquantização dos blocos (multiplicação pelas matrizes de quantização);
4 - Aplicação da transformada DCT inversa (IDCT);
5 - Conversão dos blocos para a estrutura de matrizes dos canais Y, Cb e Cr;
6 - Conversão da representação em luminância (Y) e cromância (Cb e Cr) para o espaço de cores RGB;
7 - Escrita do arquivo bmp resultante da recuperação (descompressão).


Utilizamos principalmente structs, matrizes(ponteiros duplos) e cubos (ponteiros triplos) para armazenar e manipular tabelas, matrizes e conjuntos de blocos durante o processamento.
Por exemplo:
 -> conjuntos de blocos foram armazenados e manipulados em estruturas de cubos (vetores triplos) durante a execução;
 -> tabelas de codificação foram armazenadas vetores de structs;
 -> cabeçalhos foram armazenados em structs.

Cabe observar ainda que para a recuperação da informação (descompressão), a lógica utilizada foi a leitura buffer a buffer do arquivo binário, com posterior conversão dos mesmos para
strings representando os bits presentes no buffer. A lógica de decodificação foi então principalmente baseada na manipulação de tais strings, pois consideramos ser uma solução mais intuitiva.

A IDE utilizada no desenvolvimento dos projetos foi o CodeBlocks.

====================
Estrutura do projeto
====================

O projeto encontra-se dividido em 2 subprojetos: um referente ao compressor e um ao descompressor.

	-> Estrutura de arquivos do compressor:
		-> main.c: arquivo principal do subprojeto;
		-> codificacao.c: contém funções e tabelas referentes ao processo de codificação;
		-> dct.c: funcoes referentes ao calculo da transoformada DCT;
		-> gerencia_memoria.c: funções de alocação de memoria;
		-> leitura_escrita.c: contem funções para leitura e escrita de arquivos;
		-> quantizacao.c: funções para auxilio à etapa de quantização;
		-> visualizacao.c: funções auxiliares de visualização dos dados lidos;
		-> ycbcr.c: funções para conversão entre os espaços de cores RGB e YCbCr, assim como amostragem de canais.

	-> Estrutura de arquivos do descompressor (contem funções com a logica inversa às do compressor):
		-> codificacao.c;
		-> desquantizacao.c;
		-> gerencia_memoria.c;
		-> leitura_escrita.c;
		-> main.c;
		-> ycbcr.c

OBS: todos os arquivos descritos apresentam seus correspondentes arquivos .h

==================
Taxa de compressão
==================

A taxa de compressão alcançada foi próxima de 30:1.
Porém, isso resulta em uma imagem de baixa qualidade após a descompressão, em que a estrutura da imagem é presentada,
mas as cores não.

Esta relação de compressão, segundo o material fornecido durante a disciplina (PDF - COMPRESSÃO DE IMAGENS: PADRÃO JPEG),
a qualidade da imagem reproduzida é classificada entre "usável" e de "boa qualidade".
Consideramos que "usável" se aplica ao resultado obtido, sendo coerente com a classificação prevista para a taxa de compressão atingida.
