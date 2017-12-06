#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bigint.h"

char *readline() {
    int size = 16;      // set minimum size of string to 16 bytes
    char *str = malloc(sizeof(char) * size);
    if (str == NULL) {
        printf("Memory allocation failed");
        exit(13859);
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
                exit(13859);
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
    bigInt aSumB = BigInt("");  // create empty variable
    bigInt aSubB = BigInt("");
    bigInt aMulB = BigInt("");
    bigInt aDivB = BigInt("");

    printf("a = ");
    a.print(&a);
    printf("b = ");
    b.print(&b);
    aSumB = sum(&a, &b);
    printf("a + b = ");
    aSumB.print(&aSumB);
    aSubB = sub(&a, &b);
    printf("a - b = ");
    aSubB.print(&aSubB);
    aMulB = mul(&a, &b);
    printf("a * b = ");
    aMulB.print(&aMulB);
    aDivB = bdiv(&a, &b);
    printf("a / b = ");
    aDivB.print(&aDivB);

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
