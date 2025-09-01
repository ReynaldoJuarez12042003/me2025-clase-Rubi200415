#include <stdio.h>
#include <math.h>

int main(void) {
    double x = 0;
    double Es1;
    double Es2 = 500;
    double VV;
    double VC = 0; // Declarada aquí para que esté disponible al final
    int funcion;

    printf("Las posibles funciones que realiza el programa son: \n 1)e^x\n 2)Cos(x)\n 3)1/(1-x)\n 4)arctan(x)\n 5)Sen(x)\n");
    printf("Ingrese la funcion que quiere realizar: ");
    scanf("%d", &funcion);
    printf("Ingrese los valores correspondientes para X y el error porcentual esperado:\n");
    printf("Formato: x, error (ejemplo: 2, 0.1): ");
    scanf("%lf, %lf", &x, &Es1);

    if (funcion == 1) {
        VV = exp(x);
        VC = 1;
        double factorial = 1;
        for (int i = 1; i <= 100; i++) {
            factorial *= i;
            VC += pow(x, i) / factorial;
            Es2 = fabs((VC - VV) / VV) * 100;
            printf("Iteración %d: VC = %f, Error = %f%%\n", i, VC, Es2);

            if (Es2 < Es1) {
                printf("Error alcanzado en iteración %d\n", i);
                break;
            }
        }
    }

    else if (funcion == 2) {
        VV = cos(x);
        VC = 1;
        double factorial = 1;

        for (int i = 1; i <= 100; i++) {
            factorial *= (2 * i - 1) * (2 * i); // Factorial para (2i)!
            double termino = pow(-1, i) * pow(x, 2 * i) / factorial;
            VC += termino;
            Es2 = fabs((VC - VV) / VV) * 100;
            printf("Iteración %d: VC = %f, Error = %f%%\n", i, VC, Es2);

            if (Es2 < Es1) {
                printf("Error alcanzado en iteración %d\n", i);
                break;
            }
        }
    }


    else if (funcion == 3) {
        if (fabs(x) >= 1) {
            printf("No se puede aplicar esa función (|x| debe ser < 1)\n");
            return 1;
        }
        VV = 1.0 / (1 - x);
        VC = 1;
        for (int i = 1; i <= 100; i++) {
            VC += pow(x, i);
            Es2 = fabs((VC - VV) / VV) * 100;
            printf("Iteración %d: VC = %f, Error = %f%%\n", i, VC, Es2);

            if (Es2 < Es1) {
                printf("Error alcanzado en iteración %d\n", i);
                break;
            }
        }
    }

    else if (funcion == 4) {
        if (fabs(x) >= 1) {
            printf("No se puede aplicar esa función (|x| debe ser < 1)\n");
            return 1;
        }
        VV = atan(x);
        VC = x;
        for (int i = 1; i <= 100; i++) {
            double termino = pow(-1, i) * pow(x, 2 * i + 1) / (2 * i + 1);
            VC += termino;
            Es2 = fabs((VC - VV) / VV) * 100;
            printf("Iteración %d: VC = %f, Error = %f%%\n", i, VC, Es2);

            if (Es2 < Es1) {
                printf("Error alcanzado en iteración %d\n", i);
                break;
            }
        }
    }

    else if (funcion == 5) {
        VV = sin(x);
        VC = x;
        double factorial = 1;
        for (int i = 1; i <= 100; i++) {
            factorial *= (2 * i) * (2 * i + 1); // Factorial para (2i+1)!
            double termino = pow(-1, i) * pow(x, 2 * i + 1) / factorial;
            VC += termino;
            Es2 = fabs((VC - VV) / VV) * 100;
            printf("Iteración %d: VC = %f, Error = %f%%\n", i, VC, Es2);

            if (Es2 < Es1) {
                printf("Error alcanzado en iteración %d\n", i);
                break;
            }
        }
    }
    else {
        printf("Función no válida\n");
        return 1;
    }

    printf("\nResultados finales\n");
    printf("Valor calculado: %f\n", VC);
    printf("Valor verdadero: %f\n", VV);
    printf("Error porcentual: %f%%\n", Es2);

    return 0;
}