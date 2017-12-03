#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "bigint.h"


int main() {
    char aString[10000];
    char bString[10000];
    printf("Enter a = ");
    fgets(aString, sizeof(aString), stdin);
    printf("Enter b = ");
    fgets(bString, sizeof(bString), stdin);

    bigInt a = BigInt(aString);
    bigInt b = BigInt(bString);
    bigInt c = BigInt("");  // create empty variable
    bigInt d = BigInt("");
    bigInt e = BigInt("");
    bigInt f = BigInt("");

    printf("a = ");
    a.print(&a);
    printf("b = ");
    b.print(&b);
    c = sum(&a, &b);
    printf("a + b = ");
    c.print(&c);
    d = sub(&a, &b);
    printf("a - b = ");
    d.print(&d);
    e = mul(&a, &b);
    printf("a * b = ");
    e.print(&e);
    f = bdiv(&a, &b);
    printf("a / b = ");
    f.print(&f);
    return 0;
}
