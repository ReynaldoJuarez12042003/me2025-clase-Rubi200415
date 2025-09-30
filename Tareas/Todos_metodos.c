#include <stdio.h>
#include <math.h>

// f(c) = (9.81 * 68.1)/c * (1 - exp(-(c/68.1)*10)) - 40
float f(float c) {
    return (9.81f * 68.1f) / c * (1.0f - expf(-(c / 68.1f) * 10.0f)) - 40.0f;
}

// Derivada de la función 
float df(float c) {
    float term1 = - (9.81f * 68.1f) / (c * c) * (1.0f - expf(-(c / 68.1f) * 10.0f));
    float term2 = (9.81f * 68.1f) / c * (10.0f / 68.1f) * expf(-(c / 68.1f) * 10.0f);
    return term1 + term2;
}

// Función para el método de punto fijo
float g(float c) {
    return (9.81f * 68.1f) / 40.0f * (1.0f - expf(-(c / 68.1f) * 10.0f));
}

// Función exponencial usando serie de Taylor
float exp_taylor(float x, int terminos) {
    float resultado = 1.0f;
    float factorial = 1.0f;
    float potencia = 1.0f;
    
    for (int i = 1; i <= terminos; i++) {
        factorial *= i;
        potencia *= x;
        resultado += potencia / factorial;
    }
    
    return resultado;
}


int main(void) {
    double error_max = 0.00001;
    printf("Metodo | Numero de iteraciones | Raiz | Error Relativo\n");
    printf("------------------------------------------------------\n");

    //************************************************************************
    // Método de Bisección
    float a = 10.0f, b = 20.0f;
    float medio = 0.0f, medio_anterior = 0.0f;
    float error;
    int iter = 0;
    
    do {
        medio_anterior = medio;
        medio = (a + b) * 0.5f;

        if (f(medio) * f(a) < 0.0f) {
            b = medio;
        } else {
            a = medio;
        }

        error = fabsf(medio - medio_anterior);
        if (fabsf(medio) > 0.0001f) {
            error = error / fabsf(medio);
        }
        
        iter++;

    } while (error > error_max && iter < 1000);

    printf("Biseccion   | %15d | %8.6f | %9.7f \n", iter, medio, error);

    //************************************************************************
    // Método de Falsa Posición
    a = 10.0f;
    b = 20.0f;
    medio = 0.0f;
    medio_anterior = 0.0f;
    iter = 0;
    
    do {
        medio_anterior = medio;
        medio = (f(b)*a - f(a)*b)/(f(b)- f(a));

        if (f(medio) * f(a) < 0.0f) {
            b = medio;
        } else {
            a = medio;
        }

        error = fabsf(medio - medio_anterior);
        if (fabsf(medio) > 0.0001f) {
            error = error / fabsf(medio);
        }
        
        iter++;

    } while (error > error_max && iter < 1000);

    printf("Falsa Pos.  | %15d | %8.6f | %9.7f \n", iter, medio, error);

    //************************************************************************
    // Método de la Secante
    float x0 = 10.0f, x1 = 20.0f, x2;
    iter = 0;
    error = 1.0f;
    
    do {
        x2 = x1 - f(x1) * (x1 - x0) / (f(x1) - f(x0));
        error = fabsf(x2 - x1);
        if (fabsf(x2) > 0.0001f) {
            error = error / fabsf(x2);
        }
        x0 = x1;
        x1 = x2;
        iter++;
        
    } while (error > error_max && iter < 1000);

    printf("Secante     | %15d | %8.6f | %9.7f \n", iter, x2, error);

    //************************************************************************
    // Método de Newton-Raphson
    float x = 15.0f;
    iter = 0;
    error = 1.0f;
    
    do {
        float x_anterior = x;
        x = x - f(x) / df(x);
        error = fabsf(x - x_anterior);
        if (fabsf(x) > 0.0001f) {
            error = error / fabsf(x);
        }
        iter++;
        
    } while (error > error_max && iter < 1000);

    printf("Newton-Raph.| %15d | %8.6f | %9.7f \n", iter, x, error);

    //************************************************************************
    // Método de Punto Fijo
    x = 15.0f;
    iter = 0;
    error = 1.0f;
    
    do {
        float x_anterior = x;
        x = g(x_anterior);
        error = fabsf(x - x_anterior);
        if (fabsf(x) > 0.0001f) {
            error = error / fabsf(x);
        }
        iter++;
        
    } while (error > error_max && iter < 1000);

    printf("Punto Fijo  | %15d | %8.6f | %9.7f \n", iter, x, error);

    //************************************************************************
    // Serie de Taylor 
    float c_taylor = 14.5f;  
    float VV = f(c_taylor);  
    float VC;
    float Es1 = 0.1f;        
    float Es2 = 100.0f;     
    int iter_taylor = 0;
    
    
    for (int terminos = 1; terminos <= 20; terminos++) {
        float argumento = -(c_taylor / 68.1f) * 10.0f;
        float exp_aprox = exp_taylor(argumento, terminos);
        VC = (9.81f * 68.1f) / c_taylor * (1.0f - exp_aprox) - 40.0f;
        
        Es2 = fabs((VC - VV) / VV) * 100.0f;
        iter_taylor = terminos;
        
        if (Es2 < Es1) {
            break;
        }
    }

    printf("Serie Taylor| %15d | %8.6f | %9.7f \n", iter_taylor, VC, Es2);

    //************************************************************************
    // Verificación final de todas las raíces
    printf("\n");
    printf("======================================================\n");
    printf("VERIFICACION FINAL DE RAICES\n");
    printf("======================================================\n");
    printf("Biseccion:   f(%8.6f) = %9.7f\n", medio, f(medio));
    printf("Falsa Pos.:  f(%8.6f) = %9.7f\n", medio, f(medio));
    printf("Secante:     f(%8.6f) = %9.7f\n", x2, f(x2));
    printf("Newton-Raph: f(%8.6f) = %9.7f\n", x, f(x));
    printf("Punto Fijo:  f(%8.6f) = %9.7f\n", x, f(x));
    printf("Serie Taylor: f(%8.6f) = %9.7f\n", c_taylor, VC);

    return 0;
}