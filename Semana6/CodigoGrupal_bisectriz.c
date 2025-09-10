/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <math.h>

float f(float c) {
    return (9.81f * 68.1f) / c * (1 - expf(-(c / 68.1f) * 10)) - 40.0f;
}

int main() {
    float a = 14.5f, b = 15.5f;
    float medio = 0.0f, medio_previo = 0.0f;
    float error;
    const float tol = 1e-6f;
    int iter = 0;
    
    do {
        medio_previo = medio;
        medio = (a + b) * 0.5f;
        
        if (f(medio) * f(a) < 0.0f) {
            b = medio;
        } else {
            a = medio;
        }
        
        error = fabsf(medio - medio_previo);
        iter++;
        
    } while (error > tol);
    
    printf("Raiz encontrada: %f\n", medio);
    printf("f(raiz) = %f\n", f(medio));
    
    return 0;
}