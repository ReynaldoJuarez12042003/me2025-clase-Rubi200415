#include <stdio.h>
#include <math.h>

float f(float x) {
    return sin(10*x) - cos(3*x);
}

int main() {
    float a, b;
    float error_max = 0.0001;
    int raiz_num = 1;
    int inter;
    
    printf("Raíces de f(x) = sin(10x) - cos(3x) en [3, 5]\n");
    printf("=============================================\n");
    
    // Buscar raíces en intervalos de 0.1 unidades
    for (float inicio = 3.0; inicio < 5.0; inicio += 0.1) {
        a = inicio;
        b = inicio + 0.1;
        float rango_intervalos;
        
        // Verificar cambio de signo
        if (f(a) * f(b) < 0) {
            inter=0;
            printf("\nRaíz %d en [%.2f, %.2f]:\n", raiz_num, a, b);
            
            float error;
            float medio, medio_anterior;
            // Método de bisección
            do {
                medio_anterior = medio;
                medio = (a + b) / 2;
                
                if (f(medio) * f(a) < 0) {
                    b = medio;
                } else {
                    a = medio;
                }
                rango_intervalos =b-a;
                 error = fabsf(medio - medio_anterior)/medio;
                 float error_iteracion = (rango_intervalos) / powf(2,inter);
                 float error_iteracion_comprobar = (medio - medio_anterior);
        
                printf("%4d | %8.6f | %8.6f | %9.7f |%9.7f | %10.8f\n", 
               inter, medio_anterior, medio, error, error_iteracion, error_iteracion_comprobar);
               
               inter++;

            } while (error > error_max);
            
            printf("x = %.6f, f(x) = %.8f\n", medio, f(medio));
            raiz_num++;
        }
    }
    
    return 0;
}