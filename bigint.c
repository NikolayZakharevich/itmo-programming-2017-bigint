#include <stdio.h>
#include <stdlib.h>

#include "bigint.h"

const int BASE = 1e4;
const int BASE_LENGTH = 4;
const char *digitFormat = "%.4d";   //output format

bool isNumber (char *string, int strLength) {  // check string before converting to bigInt
    if (strLength == 0) {
        return false;
    }
    if (isdigit(string[0]) == false) {  // check potential minus
        if (string[0] != '-' || strLength == 1) {  // '-' is not a number
            return false;
        }
    }

    int i;
    for (i = 1; i < strLength; ++i) {
        if (isdigit(string[i]) == false) {
            return false;
        }
    }
    return true;
}

void print(const bigInt* bigOne) {
    if (bigOne -> amount == 0) {        // empty variables have .amount = 0
        printf("Unable to print it!\n");
        return;
    }
    int i;
    if (bigOne -> isNegative == true) {
        printf("-");
    }
    printf("%d", bigOne -> digits[bigOne -> amount - 1]);
    for (i = bigOne -> amount - 2; i >= 0; --i) {
        printf(digitFormat, bigOne -> digits[i]);
    }
    printf("\n");
}

bigInt BigInt (char *digitsString) {    // constructor
    bigInt datBigBoi;   // new bigInt number
    int digitsLen = strlen(digitsString) - 1;   // digitsString has '\n' at the end so digitsLen = strlen() - 1
    if (!isNumber(digitsString, digitsLen)) {
        if (digitsLen != -1) {   //empty bigInt numbers are created using "" as digitsString
            printf("It's not a number!\n");
        }
        datBigBoi.amount = 0;
        datBigBoi.isNegative = false;
        datBigBoi.digits = (int *) malloc(sizeof(int));
        return datBigBoi;
    }

    if (digitsString[0] == '-') {      // checking sign
        if (digitsString[1] != '0') {   // -0 --> 0
            datBigBoi.isNegative = true;
        }
        else {
            datBigBoi.isNegative = false;
        }
        strncpy(digitsString, digitsString + 1, digitsLen);
        --digitsLen;
    }
    else {
        datBigBoi.isNegative = false;
    }
                                                 // turn current symbols into .digits[i]
    int i, pos = 0, decPoint = 1;
    datBigBoi.digits = (int *) malloc(sizeof(int) * (digitsLen / BASE_LENGTH + 5));   // allocate memory to array of digits
    if (datBigBoi.digits == NULL) {
        printf("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    datBigBoi.digits[pos] = 0;

    int newDigit = (digitsLen - 1) % BASE_LENGTH;
    for (i = digitsLen - 1; i >= 0; --i) {     //
        if (i % BASE_LENGTH == newDigit && i != digitsLen - 1) {  // next digit
            ++pos;
            datBigBoi.digits[pos] = 0;
            decPoint = 1;
        }
        datBigBoi.digits[pos] = datBigBoi.digits[pos] + (digitsString[i] - '0') * decPoint;
        decPoint*= 10;
    }

    int newAmount = pos + 1;    // erase leading zeroes
    while (newAmount > 1 && datBigBoi.digits[newAmount - 1] == 0) {
        --newAmount;
    }
    datBigBoi.amount = newAmount;
    return datBigBoi;
}

bool correctBigInt (const bigInt *a) {
    if (a -> amount <= 0) {
        return false;
    }
    if (a -> amount > 1 && a -> digits[a -> amount - 1] == 0) {
        return false;
    }
    return true;
}

bool more (const bigInt *a, const bigInt *b) {      // a > b
    if (!correctBigInt(a) && correctBigInt(b)) {
        printf("Incorrect bigInt number used");
        exit(EXIT_FAILURE);
    }

    _Bool bothNegative = false;

    if (!(a -> isNegative) && b -> isNegative) {
        return true;
    }
    if (a -> isNegative && !(b -> isNegative)) {
        return false;
    }
    if (a -> isNegative && b -> isNegative) {
        bothNegative = true;
    }

    if (a -> amount != b -> amount) {
        return (a -> amount > b -> amount) ^ bothNegative;
    }

    int i;
    for (i = a -> amount - 1; i >= 0; --i) {
        if (a -> digits[i] != b -> digits[i]) {
            return (a -> digits[i] > b -> digits[i]) ^ bothNegative;
        }
    }
    return false;
}

bool less (const bigInt *a, const bigInt *b) {      // a < b
    if (!correctBigInt(a) && correctBigInt(b)) {
        printf("Incorrect bigInt number used");
        exit(EXIT_FAILURE);
    }

    _Bool bothNegative = false;

    if (!(a -> isNegative) && b -> isNegative) {
        return false;
    }
    if (a -> isNegative && !(b -> isNegative)) {
        return true;
    }
    if (a -> isNegative && b -> isNegative) {
        bothNegative = true;
    }

    if (a -> amount != b -> amount) {
        return (a -> amount < b -> amount) ^ bothNegative;
    }

    int i;
    for (i = a -> amount - 1; i >= 0; --i) {
        if (a -> digits[i] != b -> digits[i]) {
            return (a -> digits[i] < b -> digits[i]) ^ bothNegative;
        }
    }
    return false;
}

bool equal (const bigInt *a, const bigInt *b) {     // a == b
    if (!correctBigInt(a) && correctBigInt(b)) {
        printf("Incorrect bigInt number used");
        exit(EXIT_FAILURE);
    }

    if (a -> isNegative ^ b -> isNegative) {
        return false;
    }

    if (a -> amount != b -> amount) {
        return false;
    }

    int i;
    for (i = a -> amount - 1; i >= 0; --i) {
        if (a -> digits[i] != b -> digits[i]) {
            return false;
        }
    }
    return true;
}

bigInt babs (const bigInt *a) { // |a|
    if (!correctBigInt(a)) {
        printf("Incorrect bigInt number used");
        exit(EXIT_FAILURE);
    }

    bigInt newBigBoi;
    newBigBoi.digits = (int *) malloc(sizeof(int) * a -> amount);
    if (newBigBoi.digits == NULL) {
        printf("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    int i;
    for (i = 0; i < a -> amount; ++i) {    // create copy of a
        newBigBoi.digits[i] = a -> digits[i];
    }
    newBigBoi.amount = a -> amount;
    newBigBoi.isNegative = false;
    return newBigBoi;
}

bigInt sum (const bigInt *a, const bigInt *b) {     // a + b
    if (!correctBigInt(a) && correctBigInt(b)) {
        printf("Incorrect bigInt number used");
        exit(EXIT_FAILURE);
    }

    bigInt newBigBoi = BigInt("");

    if (a -> isNegative && b -> isNegative) {   //-|a| + - |b| <=> -|a + b|
        newBigBoi.isNegative = true;
    }
    else if (a -> isNegative) {     // -|a| + b <=> b - |a|
        bigInt absA = babs(a);
        newBigBoi = sub(b, &absA);
        DelBigInt(&absA);
        return newBigBoi;
    }
    else if (b -> isNegative) {     // a + (-|b|) <=> a - |b|
        bigInt absB = babs(b);
        newBigBoi = sub(a, &absB);
        DelBigInt(&absB);
        return newBigBoi;
    }
    else {  // just |a| + |b|
        newBigBoi.isNegative = false;
    }

    int i, r = 0;   // here and further r - remainder of division by BASE of values which are more than BASE
    if (a -> amount > b -> amount) {
        newBigBoi.amount = a -> amount;
    }
    else {
        newBigBoi.amount = b -> amount;
    }

    newBigBoi.digits = (int *) calloc(newBigBoi.amount + 2, sizeof(int));
    if (newBigBoi.digits == NULL) {
        printf("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < newBigBoi.amount || r != 0; ++i) {
        newBigBoi.digits[i] = r;
        if (i < a -> amount) {
            newBigBoi.digits[i] += a -> digits[i];
        }
        if (i < b -> amount) {
            newBigBoi.digits[i] += b -> digits[i];
        }

        if (newBigBoi.digits[i] >= BASE) {
            newBigBoi.digits[i]-= BASE;
            r = 1;
        }
        else {
            r = 0;
        }
    }
    if (newBigBoi.digits[newBigBoi.amount] != 0) {
        newBigBoi.amount++;
    }
    return newBigBoi;
}

bigInt sub (const bigInt *a, const bigInt *b) {    // a - b
    if (!correctBigInt(a) && correctBigInt(b)) {
        printf("Incorrect bigInt number used");
        exit(EXIT_FAILURE);
    }

    bigInt newBigBoi = BigInt("");

    if (a -> isNegative && b -> isNegative) {   // -|a| - (-|b|) <=> |b| - |a|
        bigInt absA = babs(a);
        bigInt absB = babs(b);
        newBigBoi = sub(&absB, &absA);
        DelBigInt(&absA);
        DelBigInt(&absB);
        return newBigBoi;
    }
    else if (a -> isNegative) {     // -|a| - |b| <=> -(|a| + |b|)
        bigInt absA = babs(a);
        newBigBoi = sum(&absA, b);
        DelBigInt(&absA);
        newBigBoi.isNegative = true;
        return newBigBoi;
    }
    else if (b -> isNegative) { // a - (-|b|) <=> a + |b|
        bigInt absB = babs(b);
        newBigBoi = sum(a, &absB);
        DelBigInt(&absB);
        return newBigBoi;
    }
    else {  // just |a| - |b|
        if (more(a, b) || equal(a, b)) {
            newBigBoi.isNegative = false;
        }
        else {
            newBigBoi = sub(b, a);
            newBigBoi.isNegative = true;
            return newBigBoi;
        }
    }

    int i;
    newBigBoi.amount = a -> amount;
    newBigBoi.digits = (int *) realloc(newBigBoi.digits, sizeof(int) * newBigBoi.amount + 2);
    if (newBigBoi.digits == NULL) {
        printf("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < newBigBoi.amount; ++i) {    // create copy of a
        newBigBoi.digits[i] = a -> digits[i];
    }
    newBigBoi.digits[newBigBoi.amount] = 0;
    newBigBoi.digits[newBigBoi.amount + 1] = 0;

    for (i = 0; i < newBigBoi.amount; ++i) {
        if (i < b -> amount) {
            newBigBoi.digits[i] -= b -> digits[i];
        }
        if (newBigBoi.digits[i] < 0) {
            newBigBoi.digits[i]+= BASE;
            --newBigBoi.digits[i + 1];
        }

    }

    int amount = newBigBoi.amount;
    while (amount && newBigBoi.digits[amount] == 0) {
        amount--;
    }
    newBigBoi.amount = amount + 1;

    return newBigBoi;
}

bigInt mul (const bigInt *a, const bigInt *b) {     // a * b
    if (!correctBigInt(a) && correctBigInt(b)) {
        printf("Incorrect bigInt number used");
        exit(EXIT_FAILURE);
    }

    bigInt newBigBoi = BigInt("");

    if (a -> isNegative ^ b -> isNegative) {   // change sign
        if (!(a -> amount == 1 && a -> digits[0] == 0) && !(b -> amount == 1 && b -> digits[0] == 0)) {  // != 0
            newBigBoi.isNegative = true;
        }
    }

    newBigBoi.digits = (int *) calloc (a -> amount + b -> amount + 2, sizeof(int));
    if (newBigBoi.digits == NULL) {
        printf("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    int i, j;
    for (i = 0; i < a -> amount; ++i) {
        int r = 0;
        for (j = 0; j < b -> amount || r != 0; ++j) {
            if (j >= b -> amount) {
                 newBigBoi.digits[i + j] += r;
            }
            else {
                newBigBoi.digits[i + j] += a -> digits[i] * b -> digits[j] + r;
            }
            r = newBigBoi.digits[i + j] / BASE;
            newBigBoi.digits[i + j] -= r * BASE;
        }
    }
    int amount = a -> amount + b -> amount;
    while (amount > 0 && newBigBoi.digits[amount] == 0) {
        amount--;
    }
    newBigBoi.amount = amount + 1;
    return newBigBoi;
}

bigInt bdiv (const bigInt *a, const bigInt *b) {    // a / b
    if (!correctBigInt(a) && correctBigInt(b)) {
        printf("Incorrect bigInt number used");
        exit(EXIT_FAILURE);
    }

    bigInt newBigBoi = BigInt("");

    if (b -> amount == 1 && b -> digits[0] == 0) {  // Division by zero
        printf("Division by zero!");
        exit(EXIT_FAILURE);
    }
    if (a -> isNegative ^ b -> isNegative) {   // change sign
        if (!(a -> amount == 1 && a -> digits[0] == 0)) {  // != 0
            newBigBoi.isNegative = true;
        }
    }

    bigInt absA = babs(a);
    bigInt absB = babs(b);
    if (less(&absA, &absB)) {
        newBigBoi.amount = 1;
        newBigBoi.digits[0] = 0;
        return newBigBoi;
    }
    DelBigInt(&absA);
    DelBigInt(&absB);

    newBigBoi.digits = (int *) calloc(a -> amount + 1, sizeof(int));
    if (newBigBoi.digits == NULL) {
        printf("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    int *currValue;  // part of number a, which satisfies the condition BASE * b >= currValue
    currValue = (int *) calloc(a -> amount + 1, sizeof(int));
    if (currValue == NULL) {
        printf("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    int i;
    for (i = a -> amount - 1; i >= 0; --i) {
        int j;
        for (j = a -> amount; j >= 1; --j) { // currValue *= BASE
            currValue[j] = currValue[j - 1];
        }
        currValue[0] = a -> digits[i];

        int leftBound = 0, rightBound = BASE; // binary search of max(x) such that b * x <= currValue
        int x = 0, retX = 0;    // retX - last good x
        int *bMulX1, *bMulX2;
        int **currMul, **retMul;    //retMul - last good b*x
        bMulX1 = (int *) calloc (b -> amount + 1, sizeof(int));
        bMulX2 = (int *) calloc (b -> amount + 1, sizeof(int));
        if (bMulX1 == NULL || bMulX2 == NULL) {
            printf("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        currMul = &bMulX1;
        retMul = &bMulX2;

        while (leftBound <= rightBound) {
            x = (leftBound + rightBound) >> 1;
            int r = 0;               // multiplying
            for (j = 0; j < b -> amount; ++j) {
                (*currMul)[j] = b -> digits[j] * x + r;
                r = (*currMul)[j] / BASE;
                (*currMul)[j] %= BASE;
            }
           (*currMul)[j] = r;

            _Bool compare = true; // b * x <= currValue
            for (j = b -> amount; j >= 0; --j) {    // comparing
                if ((*currMul)[j] < currValue[j]) {
                    break;
                }
                else if ((*currMul)[j] > currValue[j]) {
                    compare = false;
                    break;
                }
            }
            if (compare) {
                retX = x;
                int *temp = *currMul; // swap currMul and retMul
                *currMul = *retMul;
                *retMul = temp;
                leftBound = x + 1;
            }
            else {
                rightBound = x - 1;
            }
        }
        newBigBoi.digits[i] = retX;
        for (j = 0; j <= b -> amount; ++j) {    // currValue -= bMulX;
            currValue[j] -= (*retMul)[j];
            if (currValue[j] < 0) {
                currValue[j] += BASE;
                --currValue[j + 1];
            }

        }
        free(bMulX1);
        free(bMulX2);
    }
    free(currValue);

    int amount = a -> amount;
    while (amount >= 0 && newBigBoi.digits[amount] == 0) {
        amount--;
    }
    newBigBoi.amount = amount + 1;
    return newBigBoi;
}

void DelBigInt(bigInt *a) {
    a -> digits = (int *) realloc(NULL, sizeof(int));
    free(a -> digits);
}
