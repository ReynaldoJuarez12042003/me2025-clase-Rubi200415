#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    int n, m;
    
    printf("RESOLUCION DE SISTEMAS DE ECUACIONES\n");
    printf("Metodo de Gauss Simple\n\n");
    
    // Leer dimensiones
    printf("Numero de ecuaciones: ");
    scanf("%d", &n);
    printf("Numero de incognitas: ");
    scanf("%d", &m);
    
    // Crear matriz aumentada
    double **S = (double **)malloc(n * sizeof(double *));
    for(int i = 0; i < n; i++) {
        S[i] = (double *)malloc((m + 1) * sizeof(double));
    }
    
    // Leer datos
    printf("\nIngrese los coeficientes:\n");
    for(int i = 0; i < n; i++) {
        printf("Ecuacion %d:\n", i+1);
        for(int j = 0; j < m; j++) {
            printf("  Coeficiente x%d: ", j+1);
            scanf("%lf", &S[i][j]);
        }
        printf("  Termino independiente: ");
        scanf("%lf", &S[i][m]);
    }
    
    // Mostrar sistema original
    printf("\nSISTEMA ORIGINAL:\n");
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            printf("%8.2f", S[i][j]);
        }
        printf("  |%8.2f\n", S[i][m]);
    }
    
    // Gauss Simple - Eliminacion hacia adelante
    printf("\nELIMINACION HACIA ADELANTE:\n");
    for(int i = 0; i < n; i++) {
        // Pivote
        double pivote = S[i][i];
        
        if(fabs(pivote) < 1e-10) {
            printf("Pivote cero en fila %d\n", i+1);
            continue;
        }
        
        // Normalizar fila
        for(int j = i; j <= m; j++) {
            S[i][j] = S[i][j] / pivote;
        }
        
        printf("\nPaso %d (Pivote: %.2f):\n", i+1, pivote);
        for(int k = 0; k < n; k++) {
            for(int j = 0; j <= m; j++) {
                if(j == m) printf(" | ");
                printf("%8.3f", S[k][j]);
            }
            printf("\n");
        }
        
        // Eliminar debajo del pivote
        for(int k = i + 1; k < n; k++) {
            double factor = S[k][i];
            for(int j = i; j <= m; j++) {
                S[k][j] = S[k][j] - factor * S[i][j];
            }
        }
    }
    
    // Mostrar matriz triangular
    printf("\nMATRIZ TRIANGULAR:\n");
    for(int i = 0; i < n; i++) {
        for(int j = 0; j <= m; j++) {
            if(j == m) printf(" | ");
            printf("%8.3f", S[i][j]);
        }
        printf("\n");
    }
    
    // Sustitucion hacia atras
    printf("\nSUSTITUCION HACIA ATRAS:\n");
    double X[m];
    
    for(int i = n-1; i >= 0; i--) {
        X[i] = S[i][m];
        for(int j = i + 1; j < m; j++) {
            X[i] = X[i] - S[i][j] * X[j];
        }
        printf("x%d = %.4f\n", i+1, X[i]);
    }
    
    // Mostrar solucion
    printf("\nSOLUCION:\n");
    for(int i = 0; i < m; i++) {
        printf("x%d = %8.4f\n", i+1, X[i]);
    }
    
    // Verificar
    printf("\nVERIFICACION:\n");
    for(int i = 0; i < n; i++) {
        double suma = 0;
        for(int j = 0; j < m; j++) {
            suma += S[i][j] * X[j];
        }
        printf("Ecuacion %d: %.2f = %.2f\n", i+1, suma, S[i][m]);
    }
    
    // Liberar memoria
    for(int i = 0; i < n; i++) {
        free(S[i]);
    }
    free(S);
    
    return 0;
}