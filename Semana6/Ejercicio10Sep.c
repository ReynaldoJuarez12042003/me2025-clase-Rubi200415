#include <stdio.h>
#include <math.h>

float f(float x) {
    return sin(10*x) - cos(3*x);
}

int main() {
    float a = 3.05, b = 3.20; // Primer intervalo
    float err_relativo, err_inte=0.0001;
    float c, c_anterior;
    int iteraciones = 0;
    
    printf("Buscando raiz en [%.f, %.f]\n", a, b);
    
    do {
        iteraciones++;
        c = (a + b) / 2.0;
        
        if (f(c) == 0.0) break;
        else if (f(c) * f(a) < 0) b = c;
        else a = c;
        
    } while (fabs(b - a) > err_inte);
    
    printf("Raiz encontrada: %.6f\n", c);
    printf("f(raiz) = %.6f\n", f(c));
    printf("Iteraciones: %d\n\n", iteraciones);
    
    return 0;
}