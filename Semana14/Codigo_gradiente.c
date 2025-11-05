#include <stdio.h>
#include <math.h>

// Función: f(x,y) = 2xy + 2x - x^2 - 2y^2
double f(double x, double y) {
    return 2*x*y + 2*x - x*x - 2*y*y;
}

// Método del gradiente
void gradiente(double x0, double y0) {
    double x = x0, y = y0;
    double df_dx, df_dy;
    int i;
    
    printf("Punto inicial: x=%.1f, y=%.1f\n", x0, y0);
    
    for(i = 0; i < 10; i++) {
        // Calcular gradiente
        df_dx = 2*y + 2 - 2*x;  // ∂f/∂x
        df_dy = 2*x - 4*y;      // ∂f/∂y
        
        // Actualizar posición
        x = x + 0.1 * df_dx;
        y = y + 0.1 * df_dy;
        
        printf("Iter %d: x=%.3f, y=%.3f, f=%.3f\n", 
               i+1, x, y, f(x,y));
    }
}

int main() {
    // Punto inicial: x0 = -1, y0 = 1
    gradiente(-1.0, 1.0);
    return 0;
}