#ifndef BIGINT_H
#define BIGINT_H

#include <stdbool.h>
#include <ctype.h>
#include <string.h>

typedef struct bigInt {
    int amount;     //quantity of digits
    int *digits;
    _Bool isNegative;
} bigInt;

bigInt BigInt (char *digitsString);  // constructor
void print (const bigInt *a);
bool correctBigInt (const bigInt *a); // check function parameter
bool more (const bigInt *a, const bigInt *b);   // a > b
bool left (const bigInt *a, const bigInt *b);   // a < b
bool equal (const bigInt *a, const bigInt *b);   // a = b
bigInt babs (const bigInt *a);                  // |a|
bigInt sum (const bigInt *a, const bigInt *b);  // a + b
bigInt sub (const bigInt *a, const bigInt *b);  // a - b
bigInt mul (const bigInt *a, const bigInt *b);  // a * b
bigInt bdiv (const bigInt *a, const bigInt *b); // a / b'
void DelBigInt (bigInt *a);  // destructor

#endif // BIGINT_H
