//Ejercicio de funcion  f(x)=(x^10) -1 con el metodo de la falsa posiciòn.



#include <stdio.h>
#include <math.h>

float f(float x) {
    return powf(x,10)-1;
}

int main() {
    float a =0, b = 1.3;
    float medio = 0.0f, medio_anterior = 0.0;
    float error;
    float error_max = 0.0001;
    int iter = 0;
    float error_iteracion;
    float error_iteracion_comprobar;
    float rango_inicial= b-a;
    
    
    printf("Iter | Raiz Ant | Raiz Act | Error Rel | Error Iter\n");


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
