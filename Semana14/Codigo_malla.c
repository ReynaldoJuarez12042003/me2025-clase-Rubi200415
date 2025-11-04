#include <stdio.h>
#include <math.h>

double f(double x, double y) {
    return y - x - 2*x*x - 2*x*y - y*y;
}

int main() {
    double mejor_x = -2, mejor_y = 1, mejor_f = f(-2, 1);
    int iter_error = -1, iter = 0;
    
    printf("Optimizando f(x,y) = y - x - 2x² - 2xy - y²\n");
    printf("Dominio: x∈[-2,2], y∈[1,3]\n");
    printf("BUSCANDO MAXIMO\n\n");
    
    for(double x = -2; x <= 2; x += 0.1) {
        for(double y = 1; y <= 3; y += 0.1) {
            iter++;
            double actual = f(x, y);
            
            // CAMBIO: Buscar máximo (actual > mejor_f)
            if(actual > mejor_f) {
                double mejora = actual - mejor_f;  // CAMBIO: actual - mejor_f
                if(mejora <= 0.0001 && iter_error == -1) {
                    iter_error = iter;
                }
                mejor_f = actual;
                mejor_x = x;
                mejor_y = y;
            }
        }
    }
    
    printf("Resultado:\n");
    printf("MAXIMO en: (%.2f, %.2f)\n", mejor_x, mejor_y);
    printf("f_max = %.4f\n", mejor_f);  // CAMBIO: f_max en lugar de f_min
    printf("Iteraciones: %d\n", iter);
    if(iter_error != -1) {
        printf("Mejora <= 0.0001 en iter: %d\n", iter_error);
    }
    
    return 0;
}
