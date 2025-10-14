#include <stdio.h>
#include <math.h>
#define N 2

int main() {
    double A[N][N+1] = {
        {1.0, 2.0, 10.0},
        {1.1, 2.0, 10.4}
    };
    double x[N];

    printf("Matriz aumentada:\n");
    for (int i = 0; i < N; i++) {
        printf("[ %5.1f %5.1f | %5.1f ]\n", A[i][0], A[i][1], A[i][2]);
    }

    // Eliminación gaussiana con pivoteo parcial
    for (int p = 0; p < N-1; p++) {
        // Buscar pivote máximo
        int max_i = p;
        for (int i = p+1; i < N; i++)
            if (fabs(A[i][p]) > fabs(A[max_i][p])) max_i = i;

        if (fabs(A[max_i][p]) < 1e-10) {
            printf("Sistema singular.\n");
            return 1;
        }

        // Intercambiar filas si es necesario
        if (max_i != p)
            for (int j = 0; j <= N; j++) {
                double t = A[p][j];
                A[p][j] = A[max_i][j];
                A[max_i][j] = t;
            }

        // Eliminación
        for (int i = p+1; i < N; i++) {
            double f = A[i][p] / A[p][p];
            for (int j = p; j <= N; j++)
                A[i][j] -= f * A[p][j];
        }
    }

    // Sustitución hacia atrás
    for (int i = N-1; i >= 0; i--) {
        double s = A[i][N];
        for (int j = i+1; j < N; j++) s -= A[i][j] * x[j];
        x[i] = s / A[i][i];
    }

    printf("\nSolución:\n");
    printf("x1 = %.6f\n", x[0]);
    printf("x2 = %.6f\n", x[1]);

    return 0;
}