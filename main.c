#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bigint.h"

char *readline() {
    int size = 16;      // set minimum size of string to 16 bytes
    char *str = malloc(sizeof(char) * size);
    if (str == NULL) {
        printf("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    int c;
    size_t len = 0;
    while ((c = getchar()) && c != '\r' && c != '\n') {
        *(str + len) = c;
        ++len;
        if (len == size) {     // need to allocate more memory
            size*= 2;
            str = (char *)realloc(str, sizeof(str)* size);
            if (str == NULL) {
                printf("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
        }
    }
    *(str + len) = '\n';
    *(str + len + 1) = '\0';
    return str;
}

int main() {
    char* aString;
    char* bString;
    printf("Enter a = ");
    aString = readline();
    printf("Enter b = ");
    bString = readline();

    bigInt a = BigInt(aString);
    bigInt b = BigInt(bString);

    printf("a = ");
    print(&a);
    printf("b = ");
    print(&b);
    bigInt aSumB = sum(&a, &b);
    printf("a + b = ");
    print(&aSumB);
    bigInt aSubB = sub(&a, &b);
    printf("a - b = ");
    print(&aSubB);
    bigInt aMulB = mul(&a, &b);
    printf("a * b = ");
    print(&aMulB);
    bigInt aDivB = bdiv(&a, &b);
    printf("a / b = ");
    print(&aDivB);

    free(aString);
    free(bString);
    DelBigInt(&a);
    DelBigInt(&b);
    DelBigInt(&aSumB);
    DelBigInt(&aSubB);
    DelBigInt(&aMulB);
    DelBigInt(&aDivB);
    return 0;
}
