#ifndef GERENCIA_MEMORIA_H
#define GERENCIA_MEMORIA_H

unsigned char **aloca_matriz (int n, int m);
double ***aloca_matriz_double (int num_blocos, int n, int m);
int **malloc_mat_int(int n, int m);
int ***aloca_cubo_int (int num_blocos, int n, int m);

#endif // GERENCIA_MEMORIA
