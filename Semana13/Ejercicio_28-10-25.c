#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    int m, n, i, j, k;
    
    printf("CALCULO DE MATRIZ INVERSA GENERALIZADA\n");
    printf("METODO: FACTORIZACION LU CON DESCOMPOSICION DETALLADA\n\n");
    
    printf("Ingrese filas de la matriz: ");
    scanf("%d", &m);
    printf("Ingrese columnas de la matriz: ");
    scanf("%d", &n);
    
    // Declarar matrices
    double A[m][n], AT[n][m], ATA[n][n], ATA_inv[n][n];
    double L[n][n], U[n][n];
    double Y[n], X[n];
    double inversa_generalizada[n][m];
    double identidad[n][n];
    
    // Inicializar matrices
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            L[i][j] = 0;
            U[i][j] = 0;
            ATA[i][j] = 0;
            ATA_inv[i][j] = 0;
            identidad[i][j] = (i == j) ? 1 : 0;
        }
    }
    
    // Leer matriz A
    printf("\nIngrese la matriz A (%dx%d):\n", m, n);
    for(i = 0; i < m; i++) {
        printf("Fila %d: ", i+1);
        for(j = 0; j < n; j++) {
            scanf("%lf", &A[i][j]);
        }
    }
    
    // Mostrar matriz original
    printf("\nMATRIZ A ORIGINAL (%dx%d):\n", m, n);
    for(i = 0; i < m; i++) {
        for(j = 0; j < n; j++) {
            printf("%8.3f ", A[i][j]);
        }
        printf("\n");
    }
    
    // Calcular A^T y A^T * A
    for(i = 0; i < m; i++) {
        for(j = 0; j < n; j++) {
            AT[j][i] = A[i][j];
        }
    }
    
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            for(k = 0; k < m; k++) {
                ATA[i][j] += AT[i][k] * A[k][j];
            }
        }
    }
    
    printf("\nMATRIZ A^T * A (%dx%d):\n", n, n);
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            printf("%8.3f ", ATA[i][j]);
        }
        printf("\n");
    }
    
    // FACTORIZACION LU DETALLADA - CORREGIDA
    printf("\n=== DESCOMPOSICION LU DETALLADA ===\n");
    
    // Inicializar L con 1's en la diagonal
    for(i = 0; i < n; i++) {
        L[i][i] = 1.0;
    }
    
    // Paso 1: Primera columna de L y primera fila de U
    printf("\n--- PASO 1 ---\n");
    
    // Primera fila de U
    for(j = 0; j < n; j++) {
        U[0][j] = ATA[0][j];
        printf("U[0][%d] = d1%d = ATA[0][%d] = %.4f\n", j+1, j+1, j+1, U[0][j]);
    }
    
    // Primera columna de L
    for(i = 1; i < n; i++) {
        L[i][0] = ATA[i][0] / U[0][0];
        printf("L[%d][0] = d%d1 = ATA[%d][0] / U[0][0] = %.4f / %.4f = %.4f\n", 
               i, i+1, i, ATA[i][0], U[0][0], L[i][0]);
    }
    
    // Paso 2: Segunda columna de L y segunda fila de U
    printf("\n--- PASO 2 ---\n");
    
    // Segunda fila de U
    for(j = 1; j < n; j++) {
        double sum = 0;
        for(k = 0; k < 1; k++) {
            sum += L[1][k] * U[k][j];
        }
        U[1][j] = ATA[1][j] - sum;
        printf("U[1][%d] = d2%d = ATA[1][%d] - (L[1][0]*U[0][%d]) = %.4f - (%.4f*%.4f) = %.4f\n", 
               j+1, j+1, j+1, j+1, ATA[1][j], L[1][0], U[0][j], U[1][j]);
    }
    
    // Segunda columna de L
    for(i = 2; i < n; i++) {
        double sum = 0;
        for(k = 0; k < 1; k++) {
            sum += L[i][k] * U[k][1];
        }
        L[i][1] = (ATA[i][1] - sum) / U[1][1];
        printf("L[%d][1] = d%d2 = (ATA[%d][1] - (L[%d][0]*U[0][1])) / U[1][1] = (%.4f - (%.4f*%.4f)) / %.4f = %.4f\n", 
               i, i+1, i, i, ATA[i][1], L[i][0], U[0][1], U[1][1], L[i][1]);
    }
    
    // Paso 3: Tercera columna de L y tercera fila de U (si n >= 3)
    if(n >= 3) {
        printf("\n--- PASO 3 ---\n");
        
        // Tercera fila de U
        for(j = 2; j < n; j++) {
            double sum = 0;
            for(k = 0; k < 2; k++) {
                sum += L[2][k] * U[k][j];
            }
            U[2][j] = ATA[2][j] - sum;
            printf("U[2][%d] = d3%d = ATA[2][%d] - (L[2][0]*U[0][%d] + L[2][1]*U[1][%d]) = %.4f - (%.4f*%.4f + %.4f*%.4f) = %.4f\n", 
                   j+1, j+1, j+1, j+1, j+1, ATA[2][j], L[2][0], U[0][j], L[2][1], U[1][j], U[2][j]);
        }
        
        // Tercera columna de L (para n > 3)
        for(i = 3; i < n; i++) {
            double sum = 0;
            for(k = 0; k < 2; k++) {
                sum += L[i][k] * U[k][2];
            }
            L[i][2] = (ATA[i][2] - sum) / U[2][2];
            printf("L[%d][2] = d%d3 = (ATA[%d][2] - (L[%d][0]*U[0][2] + L[%d][1]*U[1][2])) / U[2][2] = (%.4f - (%.4f*%.4f + %.4f*%.4f)) / %.4f = %.4f\n", 
                   i, i+1, i, i, i, ATA[i][2], L[i][0], U[0][2], L[i][1], U[1][2], U[2][2], L[i][2]);
        }
    }
    
    // Paso 4: Para matrices 4x4
    if(n >= 4) {
        printf("\n--- PASO 4 ---\n");
        
        // Cuarta fila de U
        for(j = 3; j < n; j++) {
            double sum = 0;
            for(k = 0; k < 3; k++) {
                sum += L[3][k] * U[k][j];
            }
            U[3][j] = ATA[3][j] - sum;
            printf("U[3][%d] = d4%d = ATA[3][%d] - (L[3][0]*U[0][%d] + L[3][1]*U[1][%d] + L[3][2]*U[2][%d]) = %.4f\n", 
                   j+1, j+1, j+1, j+1, j+1, j+1, U[3][j]);
        }
    }
    
    // Mostrar matrices L y U completas
    printf("\n=== MATRIZ L COMPLETA ===\n");
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            printf("%10.6f ", L[i][j]);
        }
        printf("\n");
    }
    
    printf("\n=== MATRIZ U COMPLETA ===\n");
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            printf("%10.6f ", U[i][j]);
        }
        printf("\n");
    }
    
    // VERIFICACION: L * U debe ser igual a ATA
    printf("\n=== VERIFICACION L * U ===\n");
    double LU[n][n];
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            LU[i][j] = 0;
            for(k = 0; k < n; k++) {
                LU[i][j] += L[i][k] * U[k][j];
            }
            printf("%10.6f ", LU[i][j]);
        }
        printf("\n");
    }
    
    // CALCULAR INVERSA DE A^T * A
    printf("\n=== CALCULO DE LA INVERSA ===\n");
    
    for(int col = 0; col < n; col++) {
        // Resolver LY = I(:,col)
        for(i = 0; i < n; i++) {
            Y[i] = identidad[i][col];
            for(j = 0; j < i; j++) {
                Y[i] -= L[i][j] * Y[j];
            }
        }
        
        // Resolver UX = Y
        for(i = n-1; i >= 0; i--) {
            X[i] = Y[i];
            for(j = i + 1; j < n; j++) {
                X[i] -= U[i][j] * X[j];
            }
            X[i] /= U[i][i];
            ATA_inv[i][col] = X[i];
        }
    }
    
    printf("\nINVERSA DE A^T * A:\n");
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            printf("%10.6f ", ATA_inv[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}