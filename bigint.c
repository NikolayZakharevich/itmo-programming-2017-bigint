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
    if (isdigit(string[0]) == false && string[0] != '-') {
        return false;
    }

     _Bool isNumber = true;
    int i;
    for (i = 1; i < strlen(string) - 1; ++i) {  // -1 because of '\n'
        if (isdigit(string[i]) == false) {
            isNumber = false;
            break;
        }
    }
    return isNumber;
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
        datBigBoi.print = &bigIntPrint;
        return datBigBoi;
    }

    if (digitsString[0] == '-') {      // checking sign
        if (digitsString[1] != '0') {   // -0 --> 0
            datBigBoi.isNegative = true;
        }
        strncpy(digitsString, digitsString + 1, strlen(digitsString));
    }
    else {
        datBigBoi.isNegative = false;
    }

    int i, pos = 0;
    for (i = strlen(digitsString) - 2; i >= 0; i-= 4) {     // digitsString has '\n' at the end so i = strlen() - 2
        int start = i - BASE_LENGTH + 1;
        if (start < 0) {
            start = 0;
        }
        char *digitsTemp = (char *) malloc(i - start + 1);      // turn current symbols into .digits[i]
        if (digitsTemp == NULL) {
            printf("Memory allocation failed");
            exit(13859);
        }
        strncpy(digitsTemp, digitsString + start, i - start + 1);
        datBigBoi.digits[pos] = atoi(digitsTemp);
        free(digitsTemp);
        ++pos;
    }
    datBigBoi.amount = pos;
    datBigBoi.print = &bigIntPrint;
    return datBigBoi;
}

bool more (const bigInt *a, const bigInt *b) {      // a > b
    if (!(a -> isNegative) && b -> isNegative) {
        return true;
    }
    if (a -> isNegative && !(b -> isNegative)) {
        return false;
    }
    if (a -> isNegative && b -> isNegative) {
        bigInt *temp = a;
        a = b;
        b = temp;
    }

    if (a -> amount != b -> amount) {
        return a -> amount > b -> amount;
    }

    int i;
    for (i = a -> amount - 1; i >= 0; --i) {
        if (a -> digits[i] != b -> digits[i]) {
            return a -> digits[i] > b -> digits[i];
        }
    }
    return false;
}

bool less (const bigInt *a, const bigInt *b) {      // a < b
    if (!(a -> isNegative) && b -> isNegative) {
        return false;
    }
    if (a -> isNegative && !(b -> isNegative)) {
        return true;
    }
    if (a -> isNegative && b -> isNegative) {
        bigInt *temp = a;
        a = b;
        b = temp;
    }

    if (a -> amount != b -> amount) {
        return a -> amount < b -> amount;
    }

    int i;
    for (i = a -> amount - 1; i >= 0; --i) {
        if (a -> digits[i] != b -> digits[i]) {
            return a -> digits[i] < b -> digits[i];
        }
    }
    return false;
}

bool equal (const bigInt *a, const bigInt *b) {     // a == b
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
    bigInt newBigBoi = *a;
    newBigBoi.isNegative = false;
    return newBigBoi;
}

bigInt sum (const bigInt *a, const bigInt *b) {     // a + b
    bigInt newBigBoi = BigInt("");
    if (a -> amount == 0 || b -> amount == 0) {
        return newBigBoi;
    }

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

    int i, r = 0;
    if (a -> amount > b -> amount) {
        newBigBoi.amount = a -> amount;
    }
    else {
        newBigBoi.amount = b -> amount;
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
    if (a -> amount == 0 || b -> amount == 0) {
        bigInt newBigBoi = BigInt("");
        return newBigBoi;
    }
    bigInt newBigBoi = *a;
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
            newBigBoi = *b;
            b = a;
            newBigBoi.isNegative = true;
        }
    }
    int i;
    for (i = 0; i < newBigBoi.amount; ++i) {
        if (i < b -> amount) {
            newBigBoi.digits[i] -= b -> digits[i];
        }
        if (newBigBoi.digits[i] < 0) {
            newBigBoi.digits[i]+= BASE;
            --newBigBoi.digits[i + 1];
        }
    }
    int pos = newBigBoi.amount;
    while (pos && newBigBoi.digits[pos] == 0) {
        pos--;
    }
    newBigBoi.amount = pos + 1;
    return newBigBoi;
}

bigInt mul (const bigInt *a, const bigInt *b) {     // a * b
    bigInt newBigBoi = BigInt("");
    if (a -> amount == 0 || b -> amount == 0) {
        return newBigBoi;
    }
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

    int i, j;
    for (i = 0; i < a -> amount; ++i) {
        int r = 0;
        for (j = 0; j < b -> amount | r != 0; ++j) {
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
    int pos = a -> amount + b -> amount;
    while (pos > 0 && newBigBoi.digits[pos] == 0) {
        pos--;
    }
    newBigBoi.amount = pos + 1;
    return newBigBoi;
}

bigInt bdiv (const bigInt *a, const bigInt *b) {    // a / b
    bigInt newBigBoi = BigInt("");

    if (b -> amount == 1 && b -> digits[0] == 0) {  // Division by zero
        return newBigBoi;
    }

    if (a -> amount == 0 || b -> amount == 0) {
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

    int i;
    for (i = a -> amount - 1; i >= 0; --i) {
        int currIt;
        for (currIt = currValue.amount; currIt >= 1; --currIt) {
            currValue.digits[currIt] = currValue.digits[currIt - 1];
            if (currValue.digits[currValue.amount] != 0) {
                ++currValue.amount;
            }
        }
        currValue.digits[0] = a -> digits[i];

        bigInt x = BigInt("");   // binary search of x such that b * x <= currValue
        x.amount = 1;
        x.digits[0] = 0;
        int leftBound = 0, rightBound = BASE;
        bigInt mid = BigInt("");
        mid.amount = 1;
        while (leftBound <= rightBound) {
            mid.digits[0] = (leftBound + rightBound) / 2;
            bigInt curr = mul(b, &mid);
            if (!more(&curr, &currValue)) {
                x = mid;
                leftBound = mid.digits[0] + 1;
            }
            else {
                rightBound = mid.digits[0] - 1;
            }
        }
        newBigBoi.digits[i] = x.digits[0];
        x = mul(b, &x);
        currValue = sub(&currValue, &x);    // currValue -= b * x
    }

    int pos = a -> amount;
    while (pos >= 0 && newBigBoi.digits[pos] == 0) {
        pos--;
    }
    newBigBoi.amount = pos + 1;
    return newBigBoi;
}
