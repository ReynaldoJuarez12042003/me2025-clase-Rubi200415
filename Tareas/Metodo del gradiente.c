#include <stdio.h>
#include <math.h>

#define N 2  // número de variables

double f(double x[]) {
    // f(x,y) = 2xy + 2x - x^2 - 2y^2
    return 2*x[0]*x[1] + 2*x[0] - x[0]*x[0] - 2*x[1]*x[1];
}

void gradiente(double x[], double grad[]) {
    double h = 1e-6;
    double temp[N];
    
    for(int i = 0; i < N; i++) {
        // Copiar punto actual
        for(int j = 0; j < N; j++) temp[j] = x[j];
        
        // f(x + h)
        temp[i] += h;
        double f_plus = f(temp);
        
        // f(x - h)
        temp[i] = x[i] - h;
        double f_minus = f(temp);
        
        // Derivada numérica
        grad[i] = (f_plus - f_minus) / (2*h);
    }
}

int main() {
    double x[N] = {-1.0, 1.0};  // punto inicial
    double g[N];
    int iter;
    
    printf("Método del Gradiente\n");
    printf("Punto inicial: x0 = %.1f, x1 = %.1f\n\n", x[0], x[1]);
    
    for(iter = 0; iter < 20; iter++) {
        gradiente(x, g);
        
        // Actualizar posición
        for(int i = 0; i < N; i++) {
            x[i] = x[i] + 0.1 * g[i];
        }
        
        printf("Iter %2d: ", iter+1);
        for(int i = 0; i < N; i++) {
            printf("x%d = %7.4f ", i, x[i]);
        }
        printf("f = %7.4f\n", f(x));
    }
    
    return 0;
}