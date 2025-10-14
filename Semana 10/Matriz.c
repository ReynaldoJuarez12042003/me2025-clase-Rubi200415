#include <math.h>
#include <stdio.h>
#define DIM 3

void mostrar_matriz(const double matriz[DIM][DIM+1]);
int eliminacion_gauss(double matriz[DIM][DIM+1]);
int resolver_sistema(const double matriz[DIM][DIM+1], double solucion[DIM]);

int main()
{
    printf("Sistema de ecuaciones lineales - Metodo de Gauss\n");
    double coeficientes[3][4] = {0};
    
    for (int i = 0; i < 3; i++) {
        printf("Ingresa los valores de la fila %d (coeficientes y termino independiente): ", i+1);
        for (int j = 0; j < 4; j++) {
            scanf("%lf", &coeficientes[i][j]);
        }
    }
    
    printf("\nMatriz aumentada del sistema:\n");
    mostrar_matriz(coeficientes);
    
    if (eliminacion_gauss(coeficientes) != 0) {
        printf("Error: Sistema sin solucion unica\n");
        return 1;
    }

    printf("Matriz triangular superior obtenida:\n");
    mostrar_matriz(coeficientes);

    double solucion[DIM] = {0};
    resolver_sistema(coeficientes, solucion);
    
    printf("\nResultados:\n");
    for (int i = 0; i < DIM; ++i) {
        printf("x%d = %.8f\n", i+1, solucion[i]);
    }

    return 0;
}

int resolver_sistema(const double matriz[DIM][DIM+1], double solucion[DIM]) {
    for (int i = DIM-1; i >= 0; --i) {
        double acumulado = 0.0;
        for (int j = i+1; j < DIM; ++j) {
            acumulado += matriz[i][j] * solucion[j];
        }
        double divisor = matriz[i][i];
        if (fabs(divisor) < 1e-10) return -1;
        solucion[i] = (matriz[i][DIM] - acumulado) / divisor;
    }
    return 0;
}

int eliminacion_gauss(double matriz[DIM][DIM+1]) {
    for (int pivote = 0; pivote < DIM-1; ++pivote) {
        double valor_pivote = matriz[pivote][pivote];
        if (fabs(valor_pivote) < 1e-10) {
            return -1;
        }

        for (int fila = pivote+1; fila < DIM; ++fila) {
            double factor = matriz[fila][pivote] / valor_pivote;
            matriz[fila][pivote] = 0.0;
            for (int columna = pivote+1; columna < DIM+1; ++columna) {
                matriz[fila][columna] -= factor * matriz[pivote][columna];
            }
        }
    }
    return 0;
}

void mostrar_matriz(const double matriz[DIM][DIM+1]) {
    for (int i = 0; i < DIM; ++i) {
        printf("[ ");
        for (int j = 0; j < DIM; ++j) {
            printf("%9.5f ", matriz[i][j]);
        }
        printf("| %9.5f ]\n", matriz[i][DIM]);
    }
}