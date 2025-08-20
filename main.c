#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

static unsigned long long gcd(unsigned long long a, unsigned long long b);
int isPrime(int pr);
void listEncrytKey(int phi, int p, int q, int eKeys[]);
int enterEncrytKey(int encE, int phi);
unsigned long powMod(int base, int exp, int modN);
unsigned int findDecrKey(int encE, int phi);
void enterMsg(char *msg);
void enterPQ(int primeOK, int *p_ptr, int *q_ptr);
void printEKey(int eKeys[]);
void enterNumForDecr(int *dec, int *cLen);
void printEResult(char msg[], int enc[]);
void printDResult(int dec[], int cLen, int decPrev[]);

int main(void) {
    printf("=== Simple Text Encryption Program ===\n\n");
    printf("1) Enter two prime numbers p and q to generate keys.\n");
    printf("2) Choose Encrypt or Decrypt.\n");
    printf("   - Encrypt: choose e from the list (or any valid e), then enter text.\n");
    printf("   - Decrypt: enter the same p and q originally used, enter e and the ciphertext.\n\n");

    int p, q, modN, phi, encE, decD;
    int enc[50], cLen, dec[50], decPrev[50];
    int eKeys[5];
    char msg[50];
    int primeOK = 0;

    // ===== START=====

    // Enter p and q
    enterPQ(primeOK, &p, &q);

    // caculate phi and modN
    phi = (p - 1) * (q - 1);
    modN = p * q;
    printf("n = %d\n", modN);
    printf("phi(n) = (p-1)(q-1) = %d\n", phi);

    // Find possible encryption keys and print it
    // caculate possible encryption keys
    listEncrytKey(phi, p, q, eKeys);
    // Print possible encryption keys
    printEKey(eKeys);

    // Choose operation
    int menu;
    while (1) {
        // print menu
        printf("\nSelect an option:\n");
        printf("1. Encrypt\n2. Decrypt\n0. Exit\n\nYour choice: ");
        if (scanf("%d", &menu) != 1) {
            printf("Invalid input (must be an integer).\n");
            return 0;
        }

        switch (menu) {
        case 1: { // Encrypt
            // enter encryption key and message
            encE = enterEncrytKey(encE, phi);
            enterMsg(msg);

            // encrypt message
            for (int i = 0; i < (int)strlen(msg); ++i) {
                enc[i] = (int)msg[i];
                enc[i] = (int)powMod(enc[i], encE, modN);
            }

            // print encryption result
            printEResult(msg, enc);
            printf("\n\nTotal characters: %d\n", (int)strlen(msg));
            break;
        }
        case 2: { // Decrypt
            // enter encryption key
            encE = enterEncrytKey(encE, phi);

            // find decryption key
            decD = (int)findDecrKey(encE, phi);
            printf("\nDecryption key (d) = %d\n", decD);

            // enter ciphertext
            enterNumForDecr(dec, &cLen);

            // decrypt ciphertext
            for (int i = 0; i < cLen; ++i) {
                decPrev[i] = dec[i];
                dec[i] = (int)powMod(dec[i], decD, modN);
            }

            // print decryption result
            printDResult(dec, cLen, decPrev);
            break;
        }
        case 0: exit(0); // Exit the program
        default: printf("\nInvalid choice. Please enter 1, 2, or 0.\n");
        }
    }
}

/* === Core functions === */
/* ---------- Math helpers ---------- */
static unsigned long long gcd(unsigned long long a, unsigned long long b) {
    while (b) {
        unsigned long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int isPrime(int pr) {
    if (pr < 2) return 0; // 0 and 1 are not prime numbers
    if (pr == 2 || pr == 3) return 1; // 2 is the only even
    if (pr % 2 == 0) return 0; // even numbers greater than 2 are not prime
    int r = (int)sqrt((double)pr);
    for (int i = 3; i <= r; i += 2)
        if (pr % i == 0) return 0;
    return 1;
}

/* (base^exp) % mod via iterative binary exponentiation */
unsigned long powMod(int base, int exp, int modN) {
    unsigned long res;
    if (base == 0)
        return 0;
    if (exp == 0)
        return 1;

    if (exp % 2 == 0) {
        res = powMod(base, exp / 2, modN);
        res = (res * res) % modN;
    }
    else {
        res = base % modN;
        res = (res * powMod(base, exp - 1, modN) % modN) % modN;
    }
    return (unsigned long)((res + modN) % modN);
}

/* ---------- UI helpers ---------- */
void enterPQ(int ok, int *p_ptr, int *q_ptr) {
    int pTmp, qTmp;

    while (true) {
        printf("Enter the first prime number (p): ");
        if (scanf("%d", &pTmp) != 1) {
            printf("Invalid input (must be an integer).\n");
            while (getchar() != '\n');
            continue;
        }
        ok = isPrime(pTmp);
        if (ok == 0 || pTmp < 2) {
            printf("Invalid input (must be a prime number >= 2).\n");
        }
        else break;
    }

    while (true) {
        printf("Enter the second prime number (q): ");
        if (scanf("%d", &qTmp) != 1) {
            printf("Invalid input (must be an integer).\n");
            while (getchar() != '\n');
            continue;
        }

        ok = isPrime(qTmp);
        if (ok == 0 || qTmp == pTmp || qTmp < 2) {
            printf("Invalid input (must be a different prime >= 2).\n");
        }
        else break;
    }

    *p_ptr = pTmp;
    *q_ptr = qTmp;
}

void listEncrytKey(int phi, int p, int q, int eKeys[]) {
    int k = 0;
    for (int e = 2; e < phi; e++) {
        if (gcd(e, phi) == 1 && e != p && e != q)
            eKeys[k++] = e;
        if (k == 5)
            break;
    }
}

int enterEncrytKey(int encE, int phi) {
    while (1) {
        printf("Enter the encryption key (e): ");
        if (scanf("%d", &encE) != 1) {
            printf("Invalid input (must be an integer).\n");
            while (getchar() != '\n');
            continue;
        }

        if (gcd(encE, phi) == 1)
            return encE; // e must be coprime with φ(n)
        printf("Invalid encryption key. Please try again.\n");
        while ((encE = getchar()) != '\n' && encE != EOF);
    }
}

void enterMsg(char *msg) {
    printf("Enter your message (max 50 chars, no spaces — use '-' instead): ");
    scanf("%49s", msg);
}

void enterNumForDecr(int *dec, int *cLen) {
    int t = 0;
    printf("How many numbers in the ciphertext? ");
    scanf("%d", &t);

    for (int i = 1; i <= t; ++i) {
        printf("Enter number %d: ", i);
        scanf("%d", &dec[i - 1]);
    }
    *cLen = t;
}

void printEKey(int eKeys[]) {
    printf("\nPossible encryption keys:\n");
    for (int i = 0; i < 5; i++)
        printf("%d\t", eKeys[i]);
    printf("\n");
}

void printEResult(char msg[], int enc[]) {
    printf("\nBefore encryption (characters):\n");
    for (int i = 0; i < (int)strlen(msg); ++i)
        printf("%c\t", msg[i]);
    printf("\n");

    printf("Before encryption (ASCII codes):\n");
    for (int i = 0; i < (int)strlen(msg); ++i)
        printf("%d\t", msg[i]);

    printf("\n\nAfter encryption (numbers):\n");
    for (int i = 0; i < (int)strlen(msg); ++i)
        printf("%d\t", enc[i]);
}

void printDResult(int dec[], int cLen, int decPrev[]) {
    printf("\n\nCiphertext (numbers before decryption):\n");
    for (int i = 0; i < cLen; ++i)
        printf("%d\t", decPrev[i]);

    printf("\nPlaintext (numbers after decryption):\n");
    for (int i = 0; i < cLen; ++i)
        printf("%d\t", dec[i]);
    printf("\n");

    printf("Plaintext (as text):\n");
    for (int i = 0; i < cLen; ++i)
        printf("%c", dec[i]);
    printf("\n");
}

unsigned int findDecrKey(int encE, int phi) {
    int k = 1;
    while (1) {
        int r = (1 + k * phi) % encE;
        if (r == 0)
            return (unsigned int)(1 + k * phi) / encE;
        k++;
    }
}
