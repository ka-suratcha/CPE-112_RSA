#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int gcd(int a, int b);
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

int main(void)
{
    printf("=== Simple Text Encryption Program ===\n\n");
    printf("How to use this program:\n\n");
    printf("1) Enter two prime numbers to generate the public key.\n");
    printf("   If you only want to encrypt (not decrypt), you may choose any primes.\n");
    printf("   However, remember them because they are required for decryption.\n\n");
    printf("2) Choose an option by entering 1, 2, or 0:\n");
    printf("   2.1) If you select Encryption:\n");
    printf("        - Enter an encryption key (e) from the 'Possible encryption keys' list.\n");
    printf("        - Enter your text (spaces are not allowed, use '-' instead).\n\n");
    printf("   2.2) If you select Decryption:\n");
    printf("        - Enter the encryption key (e).\n");
    printf("        - Enter how many numbers are in the ciphertext.\n");
    printf("        - Enter each encrypted number when prompted.\n");
    printf("        - The decrypted text will be displayed afterwards.\n\n");

    int p, q, modN, phi, encE, decD;
    int enc[50], cLen, dec[50], decPrev[50];
    int eKeys[5];
    char msg[50];
    int primeOK = 0;

    // Enter p and q
    enterPQ(primeOK, &p, &q);

    phi = (p - 1) * (q - 1);
    modN = p * q;
    printf("n = %d\n", modN);
    printf("phi(n) = (p-1)(q-1) = %d\n", phi);

    // Find possible encryption keys
    listEncrytKey(phi, p, q, eKeys);

    // Print possible encryption keys
    printEKey(eKeys);

    // Choose operation
    int menu;
    while (1)
    {
        printf("\nSelect an option:\n");
        printf("1. Encrypt\n2. Decrypt\n0. Exit\n\nYour choice: ");
        if (scanf("%d", &menu) != 1)
        {
            printf("Invalid input (must be an integer).\n");
            return 0;
        }

        switch (menu)
        {
        case 1:
        { // Encrypt
            encE = enterEncrytKey(encE, phi);
            enterMsg(msg);

            for (int i = 0; i < (int)strlen(msg); ++i)
            {
                enc[i] = (int)msg[i];
                enc[i] = (int)powMod(enc[i], encE, modN);
            }

            printEResult(msg, enc);
            printf("\n\nTotal characters: %d\n", (int)strlen(msg));
            break;
        }
        case 2:
        { // Decrypt
            encE = enterEncrytKey(encE, phi);
            decD = (int)findDecrKey(encE, phi);
            printf("\nDecryption key (d) = %d\n", decD);

            enterNumForDecr(dec, &cLen);

            for (int i = 0; i < cLen; ++i)
            {
                decPrev[i] = dec[i];
                dec[i] = (int)powMod(dec[i], decD, modN);
            }

            printDResult(dec, cLen, decPrev);
            break;
        }
        case 0:
            exit(0);
        default:
            printf("\nInvalid choice. Please enter 1, 2, or 0.\n");
        }
    }
}

/* === Core functions === */

int gcd(int a, int b)
{
    int g = 1;
    for (int i = 1; i <= a && i <= b; i++)
        if (a % i == 0 && b % i == 0)
            g = i;
    return g;
}

int isPrime(int pr)
{
    for (int i = 2; i <= (int)sqrt(pr); i++)
        if (pr % i == 0)
            return 0;
    return 1;
}

void listEncrytKey(int phi, int p, int q, int eKeys[])
{
    int k = 0;
    for (int e = 2; e < phi; e++)
    {
        if (gcd(e, phi) == 1 && e != p && e != q)
            eKeys[k++] = e;
        if (k == 5)
            break;
    }
}

void enterPQ(int ok, int *p_ptr, int *q_ptr)
{
    int pTmp, qTmp;

    while (true)
    {
        printf("Enter the first prime number (p): ");
        if (scanf("%d", &pTmp) != 1)
        {
            printf("Invalid input (must be an integer).\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        ok = isPrime(pTmp);
        if (ok == 0 || pTmp < 2)
        {
            printf("Invalid input (must be a prime number >= 2).\n");
        }
        else
            break;
    }

    while (true)
    {
        printf("Enter the second prime number (q): ");
        if (scanf("%d", &qTmp) != 1)
        {
            printf("Invalid input (must be an integer).\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        ok = isPrime(qTmp);
        if (ok == 0 || qTmp == pTmp || qTmp < 2)
        {
            printf("Invalid input (must be a different prime >= 2).\n");
        }
        else
            break;
    }

    *p_ptr = pTmp;
    *q_ptr = qTmp;
}

int enterEncrytKey(int encE, int phi)
{
    while (1)
    {
        printf("Enter the encryption key (e): ");
        if (scanf("%d", &encE) != 1)
        {
            printf("Invalid input (must be an integer).\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        if (gcd(encE, phi) == 1)
            return encE; // e must be coprime with φ(n)
        printf("Invalid encryption key. Please try again.\n");
        while ((encE = getchar()) != '\n' && encE != EOF)
            ;
    }
}

void printEKey(int eKeys[])
{
    printf("\nPossible encryption keys:\n");
    for (int i = 0; i < 5; i++)
        printf("%d\t", eKeys[i]);
    printf("\n");
}

void enterMsg(char *msg)
{
    printf("Enter your message (max 50 chars, no spaces — use '-' instead): ");
    scanf("%49s", msg);
}

unsigned long powMod(int base, int exp, int modN)
{
    unsigned long res;
    if (base == 0)
        return 0;
    if (exp == 0)
        return 1;

    if (exp % 2 == 0)
    {
        res = powMod(base, exp / 2, modN);
        res = (res * res) % modN;
    }
    else
    {
        res = base % modN;
        res = (res * powMod(base, exp - 1, modN) % modN) % modN;
    }
    return (unsigned long)((res + modN) % modN);
}

void enterNumForDecr(int *dec, int *cLen)
{
    int t = 0;
    printf("How many numbers in the ciphertext? ");
    scanf("%d", &t);

    for (int i = 1; i <= t; ++i)
    {
        printf("Enter number %d: ", i);
        scanf("%d", &dec[i - 1]);
    }
    *cLen = t;
}

void printEResult(char msg[], int enc[])
{
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

void printDResult(int dec[], int cLen, int decPrev[])
{
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

unsigned int findDecrKey(int encE, int phi)
{
    int k = 1;
    while (1)
    {
        int r = (1 + k * phi) % encE;
        if (r == 0)
            return (unsigned int)(1 + k * phi) / encE;
        k++;
    }
}
