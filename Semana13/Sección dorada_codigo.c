#include <stdio.h>
#include <math.h>

// Función objetivo: f(x) = x² - 4x + 4
double f(double x) {
    return x*x - x;
}

// Sección dorada simplificada
void seccion_dorada_simple(double a, double b) {
    double phi = (sqrt(5) - 1) / 2.0;
    double x1, x2, f1, f2;
    int iter = 0;
    int iteracion_error = -1; // Para guardar la iteración donde alcanza el error 0.0001
    
    printf("Buscando minimo en [%.2f, %.2f]...\n", a, b);
    printf("\nIter |      a      |      b      |   Error    |   x_min    |  f(x_min)  \n");
    printf("-----|-------------|-------------|------------|------------|------------\n");
    
    while (fabs(b - a) > 1e-6) {
        iter++;
        x1 = b - phi * (b - a);
        x2 = a + phi * (b - a);
        f1 = f(x1);
        f2 = f(x2);
        
        double error_actual = fabs(b - a);
        double x_min_actual = (a + b) / 2.0;
        
        // Verificar si alcanzamos el error 0.0001 por primera vez
        if (error_actual <= 0.0001 && iteracion_error == -1) {
            iteracion_error = iter;
            printf("%4d | %11.6f | %11.6f | %10.6f | %10.6f | %10.6f  <-- ERROR <= 0.0001\n", 
                   iter, a, b, error_actual, x_min_actual, f(x_min_actual));
        } else {
            printf("%4d | %11.6f | %11.6f | %10.6f | %10.6f | %10.6f\n", 
                   iter, a, b, error_actual, x_min_actual, f(x_min_actual));
        }
        
        if (f1 < f2) {
            b = x2;
        } else {
            a = x1;
        }
    }
    
    double x_min = (a + b) / 2.0;
    
    printf("\n=== RESULTADOS FINALES ===\n");
    printf("x_min = %.6f\n", x_min);
    printf("f(x_min) = %.6f\n", f(x_min));
    printf("Iteraciones totales: %d\n", iter);
    
    if (iteracion_error != -1) {
        printf("El error <= 0.0001 se alcanzo en la iteracion: %d\n", iteracion_error);
        printf("En esa iteracion:\n");
        printf("  - El intervalo era [%.6f, %.6f]\n", a, b);
        printf("  - El error era: %.6f\n", 0.0001);
    } else {
        printf("No se alcanzo el error 0.0001 en las iteraciones realizadas\n");
    }
}

int main() {
    double a, b;
    
    printf("=== SECCION DORADA SIMPLIFICADA ===\n");
    printf("Funcion: f(x) = x^2 - x\n\n");
    
    printf("Ingrese el rango:\n");
    printf("a: "); scanf("%lf", &a);
    printf("b: "); scanf("%lf", &b);
    
    if (a < b) {
        seccion_dorada_simple(a, b);
    } else {
        printf("Error: a debe ser menor que b\n");
    }
    
    return 0;
}