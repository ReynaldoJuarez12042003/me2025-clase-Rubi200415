#include <stdio.h>
#include <math.h>

int main(void) {
    float Xh = 0;
    float Xi = 0.5;
    float Xj = 1;
    float fh = 1.2;
    float fi = 0.925;
    float fj = 0.2;
    float ValorVerdaderofi = -0.9125;

    float DerivadaA, DerivadaC, DerivadaD, ErrorA, ErrorC, ErrorD;

    DerivadaA = (fj - fi)/ (Xj - Xi);
    DerivadaC = (fj - fh)/(Xj - Xh);
    DerivadaD = (fi - fh)/(Xi - Xh);

    ErrorA = fabs ( ValorVerdaderofi - DerivadaA) *100;
    ErrorC = fabs (ValorVerdaderofi - DerivadaC) *100;
    ErrorD = fabs (ValorVerdaderofi - DerivadaD) *100;

    printf("Derivada hacia adelante: %f \n", DerivadaA);
    printf("Derivada central: %f \n", DerivadaC);
    printf("Derivada hacia detras: %f \n", DerivadaD);
    printf("Error de derivada hacia adelante: %f % \n", ErrorA);
    printf("Error de derivada central: %f % \n", ErrorC);
    printf("Error de derivada hacia detras: %f % \n", ErrorD);


    return 0;
}