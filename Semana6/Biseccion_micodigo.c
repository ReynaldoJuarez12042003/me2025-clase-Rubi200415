#include <stdio.h>
#include <math.h>

float calcular(float c) {
    return (9.81 * 68.1) / c * (1 - exp(-(c / 68.1) * 10)) - 40;
}

int main() {
    float inicio = 14.5, fin = 15.5;
    float tol = 0.000001;
    float raiz, raiz_ant;
    int cont = 0;
    float err_rel, err_int;

    printf("=== METODO BISECCION ===\n");
    printf("Desde [%.2f] hasta [%.2f]\n", inicio, fin);
    printf("Margen error: %f\n\n", tol);

    printf("Iter | Raiz Ant | Raiz Act | Error Rel | Error Iter\n");
    printf("---------------------------------------------------\n");

    raiz_ant = (inicio + fin) / 2;

    while (fabs(fin - inicio) > tol) {
        cont++;
        raiz = (inicio + fin) / 2;

        if (cont > 1) {
            err_rel = fabs((raiz - raiz_ant) / raiz);
            err_int = fabs(raiz - raiz_ant);
        } else {
            err_rel = 0;
            err_int = 0;
        }

        printf("%4d | %8.6f | %8.6f | %9.7f | %10.8f\n", 
               cont, raiz_ant, raiz, err_rel, err_int);

        if (calcular(raiz) == 0) break;
        
        if (calcular(raiz) * calcular(inicio) < 0) {
            fin = raiz;
        } else {
            inicio = raiz;
        }
        
        raiz_ant = raiz;
    }

    printf("\nResultado final: %.6f\n", raiz);
    printf("Valor funcion: %.6f\n", calcular(raiz));
    printf("Total iteraciones: %d\n", cont);
    printf("Ultimo error: %.8f\n", fabs(raiz - raiz_ant));

    return 0;
}