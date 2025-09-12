//Ejercicio de funcion  f(c)= gm/c*(1-e^(-c/m*t))-v con el metodo de la falsa posiciòn.



#include <stdio.h>
#include <math.h>

float f(float c) {
    return (9.81f * 68.1f) / c * (1 - expf(-(c / 68.1f) * 10)) - 40.0f;
}

int main() {
    float a =4, b = 20;
    float medio = 0.0f, medio_anterior = 0.0;
    float error;
    float error_max = 0.0001;
    int iter = 0;
    float error_iteracion;
    float error_iteracion_comprobar;
    float rango_inicial= b-a;


    do {
        medio_anterior = medio;
        medio = (f(b)*a - f(a)*b)/(f(b)- f(a));

        if (f(medio) * f(a) < 0.0) {
            b = medio;
        } else {
            a = medio;
        }

        error = fabsf(medio - medio_anterior)/medio;
        //error_iteracion = (rango_inicial) / powf(2,iter);
        error_iteracion_comprobar = (medio - medio_anterior);
        
        
        printf("%4d | %8.6f | %8.6f | %9.7f | %10.8f\n", 
               iter, medio_anterior, medio, error, error_iteracion_comprobar);

        iter++;

    } while (error > error_max);

    printf("Raiz encontrada: %f\n", medio);
    printf("f(raiz) = %f\n", f(medio));
    //printf("Error entre iteraciones = %f\n",error_iteracion);
    printf("Error comprobado =%f\n", error_iteracion_comprobar);

    return 0;

}
