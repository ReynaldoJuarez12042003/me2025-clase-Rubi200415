#include <stdio.h>
#include <math.h>

// Función objetivo: f(x) = x² - 4x + 4
double f(double x) {
    return x*x - 4*x + 4;
}

void interpolacion_cuadratica(double x0, double x1, double x2) {
    double f0, f1, f2;
    double x3, f3;
    int iter = 0;
    int iteracion_error = -1;
    double error_anterior = 1e10;
    
    printf("=== METODO DE INTERPOLACION CUADRATICA ===\n");
    printf("Funcion: f(x) = x^2 - 4x + 4\n");
    printf("Puntos iniciales: x0=%.2f, x1=%.2f, x2=%.2f\n\n", x0, x1, x2);
    
    printf("Iter |     x0      |     x1      |     x2      |     x3      |   Error    |   f(x3)   \n");
    printf("-----|-------------|-------------|-------------|-------------|------------|------------\n");
    
    do {
        iter++;
        
        // Recalcular f0, f1, f2 con los valores actuales de x0, x1, x2
        f0 = f(x0);
        f1 = f(x1);
        f2 = f(x2);
        
        // Fórmula CORRECTA de interpolación cuadrática
        double numerador = f0*(x1*x1 - x2*x2) + f1*(x2*x2 - x0*x0) + f2*(x0*x0 - x1*x1);
        double denominador = 2.0 * (f0*(x1 - x2) + f1*(x2 - x0) + f2*(x0 - x1));
        
        if (fabs(denominador) < 1e-15) {
            printf("Advertencia: Denominador muy pequeño, terminando...\n");
            break;
        }
        
        x3 = numerador / denominador;
        f3 = f(x3);
        
        double error = (iter > 1) ? fabs(x3 - error_anterior) : fabs(x3 - x1);
        error_anterior = x3;
        
        if (error <= 0.0001 && iteracion_error == -1) {
            printf("%4d | %11.6f | %11.6f | %11.6f | %11.6f | %10.6f | %10.6f  <-- ERROR <= 0.0001\n", 
                   iter, x0, x1, x2, x3, error, f3);
        } else {
            printf("%4d | %11.6f | %11.6f | %11.6f | %11.6f | %10.6f | %10.6f\n", 
                   iter, x0, x1, x2, x3, error, f3);
        }
        
        // Reemplazar el punto con MAYOR valor de función
        if (f0 >= f1 && f0 >= f2) {
            x0 = x3;
        } else if (f1 >= f0 && f1 >= f2) {
            x1 = x3;
        } else {
            x2 = x3;
        }
        
        // >>>>>>>>> NO reordenes los puntos <<<<<<<<<
        // La correspondencia entre x y f se mantiene al recalcular f en cada iteración
        
        if (error < 1e-10) {
            break;
        }
        
    } while (iter < 20);
    
    // Evaluar el mejor x entre los últimos puntos
    double puntos[3] = {x0, x1, x2};
    double mejor_x = x3;
    double mejor_f = f3;
    for (int i = 0; i < 3; i++) {
        double val = f(puntos[i]);
        if (val < mejor_f) {
            mejor_f = val;
            mejor_x = puntos[i];
        }
    }
    
    printf("\n=== RESULTADOS FINALES ===\n");
    printf("Mejor x encontrado: %.8f\n", mejor_x);
    printf("f(mejor_x) = %.8f\n", mejor_f);
    printf("Iteraciones totales: %d\n", iter);
    
    if (iteracion_error != -1) {
        printf("El error <= 0.0001 se alcanzo en la iteracion: %d\n", iteracion_error);
    } else {
        printf("No se alcanzo el error 0.0001 en las iteraciones realizadas\n");
    }
}

int main() {
    double x0, x1, x2;
    
    printf("=== INTERPOLACION CUADRATICA ===\n");
    printf("Funcion: f(x) = x^2 - 4x + 4\n\n");
    
    printf("Ingrese tres puntos iniciales:\n");
    printf("x0: "); scanf("%lf", &x0);
    printf("x1: "); scanf("%lf", &x1);
    printf("x2: "); scanf("%lf", &x2);
    
    if (x0 == x1 || x1 == x2 || x0 == x2) {
        printf("Error: Los puntos deben ser distintos.\n");
        return 1;
    }
    
    interpolacion_cuadratica(x0, x1, x2);
    
    return 0;
}