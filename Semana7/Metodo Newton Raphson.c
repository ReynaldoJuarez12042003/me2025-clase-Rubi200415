//Paola Rubi Hernández Floreano "NEWTON-RAPHSON"


#include <stdio.h>
#include <math.h>

float f(float x) {
    return exp(-x) - x;
}

float df(float x) {
    return -exp(-x) - 1;
}

int main() {
    float x0 = 0.0;
    float tol = 0.000001;
    float raiz, raiz_ant;
    int cont = 0;
    float err_rel, err_int;

    printf("=== METODO NEWTON-RAPHSON ===\n");
    printf("x0 = %.1f\n", x0);
    printf("Margen error: %f\n\n", tol);

    printf("Iter | Raiz Ant | Raiz Act | Error Rel | Error Iter\n");
    printf("---------------------------------------------------\n");
    
    raiz_ant = x0;
    raiz = x0 - f(x0) / df(x0);
    cont = 1;
    
    err_rel = fabs((raiz - raiz_ant) / raiz);
    err_int = fabs(raiz - raiz_ant);
    
    printf("%4d | %8.6f | %8.6f | %9.7f | %10.8f\n", 
           cont, raiz_ant, raiz, err_rel, err_int);
    
    while (fabs(f(raiz)) > tol) {
        cont++;
        raiz_ant = raiz;
        raiz = raiz - f(raiz) / df(raiz);
        
        err_rel = fabs((raiz - raiz_ant) / raiz);
        err_int = fabs(raiz - raiz_ant);
        
        printf("%4d | %8.6f | %8.6f | %9.7f | %10.8f\n", 
               cont, raiz_ant, raiz, err_rel, err_int);
        
        if (cont > 50) break;
    }

    printf("\nResultado final: %.6f\n", raiz);
    printf("Valor funcion: %.8f\n", f(raiz));
    printf("Total iteraciones: %d\n", cont);
    printf("Ultimo error: %.8f\n", err_int);

    return 0;
}