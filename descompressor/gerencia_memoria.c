#include <stdlib.h>
#include <stdio.h>

//Função para alocar uma matriz de unsigned char
unsigned char **aloca_matriz (int m, int n)
{
  unsigned char** matriz;  /* ponteiro para a matriz */
  int   i;    /* variavel auxiliar      */
  if (n < 1 || m < 1) { /* verifica parametros recebidos */
     printf ("** Erro: Parametro invalido **\n");
     return (NULL);
     }
  /* aloca as linhas da matriz */
  matriz = (unsigned char **) calloc (m, sizeof(unsigned char *));
  if (matriz == NULL) {
     printf ("** Erro: Memoria Insuficiente **");
     return (NULL);
     }
  /* aloca as colunas da matriz */
  for ( i = 0; i < m; i++ ) {
      matriz[i] = (unsigned char *) calloc (n, sizeof(unsigned char));
      if (matriz[i] == NULL) {
         printf ("** Erro: Memoria Insuficiente **");
         return (NULL);
         }
      }
  return (matriz); /* retorna o ponteiro para a matriz */
}


//Função para alocar uma matriz de doubles
double ***aloca_matriz_double (int num_blocos, int n, int m)
{
    double*** blocos;

    blocos = (double ***) calloc (num_blocos, sizeof(double **));
    if (blocos == NULL) {
     printf ("** Erro: Memoria Insuficiente **");
     return (NULL);
    }

    for (int i = 0; i < num_blocos; i++) {
        blocos[i] = (double**) calloc (n, sizeof(double *));
        if (blocos[i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
            return (NULL);
        }
    }

    for (int i = 0; i < num_blocos; i++) {
      for (int j = 0; j < n; j++){
        blocos[i][j] = (double*) calloc (m, sizeof(double));
        if (blocos[i][j] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
            return (NULL);
        }
      }
    }

    return blocos;
}

//Aloca um cubo de inteiros
int ***aloca_cubo_int (int num_blocos, int n, int m)
{
    int*** blocos;

    blocos = (int ***) calloc (num_blocos, sizeof(int **));
    if (blocos == NULL) {
     printf ("** Erro: Memoria Insuficiente **");
     return (NULL);
    }

    for (int i = 0; i < num_blocos; i++) {
        blocos[i] = (int**) calloc (n, sizeof(int *));
        if (blocos[i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
            return (NULL);
        }
    }

    for (int i = 0; i < num_blocos; i++) {
      for (int j = 0; j < n; j++){
        blocos[i][j] = (int*) calloc (m, sizeof(int));
        if (blocos[i][j] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
            return (NULL);
        }
      }
    }

    return blocos;
}

int **malloc_mat_int(int n, int m)
{
  int **matriz;  /* ponteiro para a matriz */
  int   i;    /* variavel auxiliar      */
  if (n < 1 || m < 1) { /* verifica parametros recebidos */
     printf ("** Erro: Parametro invalido **\n");
     return (NULL);
     }
  /* aloca as linhas da matriz */
  matriz = (int **) calloc (m, sizeof(int *));
  if (matriz == NULL) {
     printf ("** Erro: Memoria Insuficiente **");
     return (NULL);
     }
  /* aloca as colunas da matriz */
  for ( i = 0; i < m; i++ ) {
      matriz[i] = (int*) calloc (n, sizeof(int));
      if (matriz[i] == NULL) {
         printf ("** Erro: Memoria Insuficiente **");
         return (NULL);
         }
      }
  return (matriz); /* retorna o ponteiro para a matriz */
}

