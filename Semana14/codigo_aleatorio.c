#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

double f(double x, double y) {
    return y - x - 2*x*x - 2*x*y - y*y;
}

int main() {
    srand(time(NULL));
    
    double mejor_x, mejor_y, mejor_f;
    int iter_error = -1;
    
    printf("Busqueda Aleatoria - Maximo\n");
    printf("f(x,y) = y - x - 2x² - 2xy - y²\n\n");
    
    // Punto inicial aleatorio
    mejor_x = -2 + (double)rand()/RAND_MAX * 4;
    mejor_y = 1 + (double)rand()/RAND_MAX * 2;
    mejor_f = f(mejor_x, mejor_y);
    
    for(int i = 1; i <= 500; i++) {
        double x = -2 + (double)rand()/RAND_MAX * 4;
        double y = 1 + (double)rand()/RAND_MAX * 2;
        double actual = f(x, y);
        
        if(actual > mejor_f) {  // Buscar máximo
            double mejora = actual - mejor_f;
            if(mejora <= 0.0001 && iter_error == -1) {
                iter_error = i;
            }
            mejor_f = actual;
            mejor_x = x;
            mejor_y = y;
        }
    }
    
    printf("=== RESULTADO ===\n");
    printf("Maximo en: (%.4f, %.4f)\n", mejor_x, mejor_y);
    printf("f_max = %.6f\n", mejor_f);
    printf("Iteraciones: 500\n");
    if(iter_error != -1) {
        printf("Mejora <= 0.0001 en iter: %d\n", iter_error);
    }
    
    return 0;
}