#include <stdio.h>
#include <stdlib.h>

#include "bigint.h"

const int BASE = 1e4;
const int BASE_LENGTH = 4;
const char *digitFormat = "%.4d";   //output format

bool isNumber (char *string) {  // check string before converting to bigInt
    if (strlen(string) == 0) {
        return false;
    }
    if (isdigit(string[0]) == false) {  // check potential minus
        if (string[0] != '-' || strlen(string) == 1) {  // '-' is not a number
            return false;
        }
    }

    int i;
    for (i = 1; i < strlen(string) - 1; ++i) {  // -1 because of '\n'
        if (isdigit(string[i]) == false) {
            return false;
        }
    }
    return true;
}

void bigIntPrint(const bigInt* bigOne) {    //print
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
    if (!isNumber(digitsString)) {
        if (strlen(digitsString) != 0) {   //empty bigInt numbers are created using "" as digitsString
            printf("It's not a number!\n");
        }
        datBigBoi.amount = 0;
        datBigBoi.isNegative = false;
        datBigBoi.digits = (int *) malloc(sizeof(int));
        datBigBoi.print = &bigIntPrint;
        return datBigBoi;
    }

    if (digitsString[0] == '-') {      // checking sign
        if (digitsString[1] != '0') {   // -0 --> 0
            datBigBoi.isNegative = true;
        }
        else {
            datBigBoi.isNegative = false;
        }
        strncpy(digitsString, digitsString + 1, strlen(digitsString));
    }
    else {
        datBigBoi.isNegative = false;
    }
                                                 // turn current symbols into .digits[i]
    int digitsLen = strlen(digitsString) - 1;   // digitsString has '\n' at the end so digitsLen = strlen() - 1
    int i, pos = 0, decPoint = 1;
    datBigBoi.digits = (int *) malloc(sizeof(int) * (digitsLen / BASE_LENGTH + 5));   // allocate memory to array of digits
    if (datBigBoi.digits == NULL) {
        printf("Memory allocation failed");
        exit(13859);
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
    datBigBoi.print = &bigIntPrint;
    return datBigBoi;
}

bool correctBigInt (const bigInt *a) {
    if (a -> amount == NULL) {
        return false;
    }
    if (a -> digits == NULL) {
        return false;
    }
    if (a -> isNegative == NULL) {
        return false;
    }
    if (a -> amount <= 0) {
        return false;
    }
    if (a -> amount > 1 && a -> digits[a -> amount - 1] == 0) {
        return false;
    }
    return true;
}

bool more (const bigInt *a, const bigInt *b) {      // a > b
    if (!correctBigInt(a) || !correctBigInt(b)) {
        printf("Incorrect bigInt number used");
        exit(87);
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
    if (!correctBigInt(a) || !correctBigInt(b)) {
        printf("Incorrect bigInt number used");
        exit(87);
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
    if (!correctBigInt(a) || !correctBigInt(b)) {
        printf("Incorrect bigInt number used");
        exit(87);
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
        exit(87);
    }

    bigInt newBigBoi = *a;
    newBigBoi.isNegative = false;
    return newBigBoi;
}

bigInt sum (const bigInt *a, const bigInt *b) {     // a + b
    if (!correctBigInt(a) || !correctBigInt(b)) {
        printf("Incorrect bigInt number used");
        exit(87);
    }

    bigInt newBigBoi = BigInt("");

    if (a -> isNegative && b -> isNegative) {   //-|a| + - |b| <=> -|a + b|
        newBigBoi.isNegative = true;
    }
    else if (a -> isNegative) {     // -|a| + b <=> b - |a|
        bigInt absA = babs(a);
        newBigBoi = sub(b, &absA);
        return newBigBoi;
    }
    else if (b -> isNegative) {     // a + (-|b|) <=> a - |b|
        bigInt absB = babs(b);
        newBigBoi = sub(a, &absB);
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

    newBigBoi.digits = (int *) realloc(newBigBoi.digits, sizeof(int) * (newBigBoi.amount + 2));
    if (newBigBoi.digits == NULL) {
        printf("Memory allocation failed");
        exit(13859);
    }

    for (i = 0; i < newBigBoi.amount + 2; ++i) {
        newBigBoi.digits[i] = 0;
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
    if (!correctBigInt(a) || !correctBigInt(b)) {
        printf("Incorrect bigInt number used");
        exit(87);
    }

    bigInt newBigBoi = BigInt("");

    if (a -> isNegative && b -> isNegative) {   // -|a| - (-|b|) <=> |b| - |a|
        bigInt absA = babs(a);
        bigInt absB = babs(b);
        newBigBoi = sub(&absB, &absA);
        return newBigBoi;
    }
    else if (a -> isNegative) {     // -|a| - |b| <=> -(|a| + |b|)
        bigInt absA = babs(a);
        newBigBoi = sum(&absA, b);
        newBigBoi.isNegative = true;
        return newBigBoi;
    }
    else if (b -> isNegative) { // a - (-|b|) <=> a + |b|
        bigInt absB = babs(b);
        newBigBoi = sum(a, &absB);
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
        exit(13859);
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
    if (!correctBigInt(a) || !correctBigInt(b)) {
        printf("Incorrect bigInt number used");
        exit(87);
    }

    bigInt newBigBoi = BigInt("");

    if (a -> isNegative ^ b -> isNegative) {   // -a * b <=> a * -b <=>  -(|a| * |b|)
        if (a -> isNegative) {
            bigInt absA = babs(a);
            newBigBoi = mul(&absA, b);
        }
        else {
            bigInt absB = babs(b);
            newBigBoi = mul(a, &absB);
        }
        if (!(newBigBoi.amount == 1 && newBigBoi.digits[0] == 0)) {  // != 0
            newBigBoi.isNegative = true;
        }
        return newBigBoi;
    }
    else if (a -> isNegative && b -> isNegative) {  // -a * -b <=> a * b
        bigInt absA = babs(a);
        bigInt absB = babs(b);
        newBigBoi = mul(&absA, &absB);
        return newBigBoi;
    }

    newBigBoi.digits = (int *) realloc(newBigBoi.digits, sizeof(int) * (a -> amount + b -> amount + 2));
    if (newBigBoi.digits == NULL) {
        printf("Memory allocation failed");
        exit(13859);
    }

    int i, j;
    for (i = 0; i <= a -> amount + b -> amount; ++i) {   //initialize result digits with 0
        newBigBoi.digits[i] = 0;
    }

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
    if (!correctBigInt(a) || !correctBigInt(b)) {
        printf("Incorrect bigInt number used");
        exit(87);
    }

    bigInt newBigBoi = BigInt("");

    if (b -> amount == 1 && b -> digits[0] == 0) {  // Division by zero
        return newBigBoi;
    }
    if (a -> isNegative ^ b -> isNegative) {   // -a / b <=> a / -b <=>  -(|a| / |b|)
        if (a -> isNegative) {
            bigInt absA = babs(a);
            newBigBoi = bdiv(&absA, b);
        }
        else {
            bigInt absB = babs(b);
            newBigBoi = bdiv(a, &absB);
        }
        if (!(newBigBoi.amount == 1 && newBigBoi.digits[0] == 0)) {  // != 0
            newBigBoi.isNegative = true;
        }
        return newBigBoi;
    }
    else if (a -> isNegative && b -> isNegative) {  // -a / -b <=> a / b
        bigInt absA = babs(a);
        bigInt absB = babs(b);
        newBigBoi = bdiv(&absA, &absB);
        return newBigBoi;
    }

    if (less(a, b)) {
        newBigBoi.amount = 1;
        newBigBoi.digits[0] = 0;
        return newBigBoi;
    }

    bigInt currValue = BigInt("");
    currValue.amount = 1;
    currValue.digits = (int *) realloc(currValue.digits, sizeof(int) * (a -> amount + 1));
    newBigBoi.digits = (int *) realloc(newBigBoi.digits, sizeof(int) * (a -> amount + 1));
    if (newBigBoi.digits == NULL || currValue.digits == NULL) {
        printf("Memory allocation failed");
        exit(13859);
    }

    int i;
    for (i = 0; i <= a -> amount; ++i) {
        currValue.digits[i] = 0;
        newBigBoi.digits[i] = 0;
    }

    for (i = a -> amount - 1; i >= 0; --i) {
        int currIt;
        for (currIt = currValue.amount; currIt >= 1; --currIt) {    // currValue *= BASE
            currValue.digits[currIt] = currValue.digits[currIt - 1];
            if (currValue.digits[currValue.amount] != 0) {
                ++currValue.amount;
            }
        }
        currValue.digits[0] = a -> digits[i];

        int leftBound = 0, rightBound = BASE; // binary search of x such that b * x <= currValue
        bigInt x = BigInt("");
        x.amount = 1;
        bigInt mid = BigInt("");
        mid.amount = 1;
        while (leftBound <= rightBound) {
            mid.digits[0] = (leftBound + rightBound) / 2;
            bigInt bMulX = mul(b, &mid);
            if (!more(&bMulX, &currValue)) {
                x.digits[0] = mid.digits[0];
                leftBound = mid.digits[0] + 1;
            }
            else {
                rightBound = mid.digits[0] - 1;
            }
            DelBigInt(&bMulX);
        }
        newBigBoi.digits[i] = x.digits[0];
        x = mul(b, &x);
        currValue = sub(&currValue, &x);    // currValue -= b * x
        DelBigInt(&mid);
        DelBigInt(&x);
    }

    int amount = a -> amount;
    while (amount >= 0 && newBigBoi.digits[amount] == 0) {
        amount--;
    }
    newBigBoi.amount = amount + 1;
    return newBigBoi;
}

void DelBigInt(bigInt *a) {
    a -> amount = NULL;
    free(a -> digits);
    a -> isNegative = NULL;
    a -> print = NULL;
}
