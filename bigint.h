#ifndef BIGINT_H
#define BIGINT_H

typedef struct bigInt {
    int amount;     //quantity of digits
    int digits[10000];
    _Bool isNegative;
    void (*print)(const struct bigInt*);
} bigInt;


bigInt BigInt (char *digitsString);  // constructor
bool more (const bigInt *a, const bigInt *b);   // a > b
bool left (const bigInt *a, const bigInt *b);   // a < b
bool equal (const bigInt *a, const bigInt *b);   // a = b
bigInt babs (const bigInt *a);                  // |a|
bigInt sum (const bigInt *a, const bigInt *b);  // a + b
bigInt sub (const bigInt *a, const bigInt *b);  // a - b
bigInt mul (const bigInt *a, const bigInt *b);  // a * b
bigInt bdiv (const bigInt *a, const bigInt *b); // a / b'



#endif // BIGINT_H
