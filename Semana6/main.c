/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <math.h>

int main(void) {
    double a=-10;
    double b=10;
    double ErrorCalculado=500;
    double ErrorMaX= 0.005;
    double h=0.05; 
    //fUNCIÓN EJEMPLO x³ - x - 2
    

    if (ErrorMaX < ErrorCalculado) {
        for (int c = -10; c <= 10; c+h) {
            k = (a+b)/2;
            VC += pow(c, 3)- c -2;
            Es2 = fabs((VC - VV) / VV) * 100;
            printf("Iteración %d: VC = %f, Error = %f%%\n", i, VC, Es2);
            
            if (Es2 < Es1) {
                printf("Error alcanzado en iteración %d\n", c);
                break;
            }
        }
    }

    printf("\nResultado final:\n");
    printf("Valor verdadero (exp(%f)): %f\n", x, VV);
    printf("Valor calculado: %f\n", VC);
    printf("Error porcentual: %f%%\n", Es2);

    return 0;
}