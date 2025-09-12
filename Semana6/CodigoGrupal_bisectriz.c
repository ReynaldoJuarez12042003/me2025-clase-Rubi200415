#include <stdio.h>
#include <math.h>

float f(float c) {
    return (9.81f * 68.1f) / c * (1 - expf(-(c / 68.1f) * 10)) - 40.0f;
}

int main() {
    float a = 14.5, b = 15.5;
    float medio = 0.0f, medio_anterior = 0.0;
    float error;
    float error_max = 0.001;
    int iter = 0;
    float error_iteracion;
    float error_iteracion_comprobar;
    float rango_inicial= b-a;


    do {
        medio_anterior = medio;
        medio = (a + b) * 0.5;

        if (f(medio) * f(a) < 0.0) {
            b = medio;
        } else {
            a = medio;
        }

        error = fabsf(medio - medio_anterior)/medio;
        error_iteracion = (rango_inicial) / powf(2,iter);
        error_iteracion_comprobar = (medio - medio_anterior);

        iter++;

    } while (error > error_max);

    printf("Raiz encontrada: %f\n", medio);
    printf("f(raiz) = %f\n", f(medio));
    printf("Error entre iteraciones = %f\n",error_iteracion);
    printf("Error comprobado =%f\n", error_iteracion_comprobar);

    return 0;
}
