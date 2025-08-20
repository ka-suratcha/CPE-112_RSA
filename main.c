#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LEN 50
/* ---------- Math helpers ---------- */
static unsigned long long gcd(unsigned long long a, unsigned long long b);
static int isPrime(int pr);
static unsigned long long powMod(unsigned long long base, unsigned long long exp, unsigned long long mod);
static unsigned long long findDecrKey(unsigned long long encryptKey, unsigned long long phi);

/* ---------- UI helpers ---------- */
static void enterPQ(int *p_ptr, int *q_ptr);
static void listEncryptKeys(unsigned long long phi, int p, int q, int eKey[], int cap);
static void printEKey(int eKeys[], int cap);
static unsigned long long enterEncrytKey(unsigned long long phi);
static void enterMsg(char *msg);
static int enterNumForDecr(unsigned long long dercMsg[], int cap);
static void printDResult(const unsigned long long dercMsg[], const unsigned long long dercedNum[], int cLen);
static void printEResult(const char msg[], const unsigned long long encryptMsg[], int len);

int main(void) {
    printf("=== Simple Text Encryption Program ===\n\n");
    printf("1) Enter two prime numbers p and q to generate keys.\n");
    printf("2) Choose Encrypt or Decrypt.\n");
    printf("   - Encrypt: choose e from the list (or any valid e), then enter text.\n");
    printf("   - Decrypt: enter the same p and q originally used, enter e and the ciphertext.\n\n");

    int p, q;
    char msg[50];

    // ===== START=====

    // Enter p and q
    enterPQ(&p, &q);

    // caculate phi and n
    unsigned long long n   = (unsigned long long)p * (unsigned long long)q;
    unsigned long long phi = (unsigned long long)(p - 1) * (unsigned long long)(q - 1);

    printf("==========================================================================\n");
    printf("n = %llu\n", n);
    printf("phi(n) = (p-1)(q-1) = %llu\n\n", phi);
        if (n < 128ULL) {
        printf("Warning: n < 128. Some ASCII characters may not round-trip cleanly.\n");
    }

    int eKeys[5] = {0};
    listEncryptKeys(phi, p, q, eKeys, 5);


    unsigned long long encryptKey = 0, decrKey = 0;
    unsigned long long encryptMsg[MAX_LEN] = {0};
    unsigned long long dercMsg[MAX_LEN] = {0};
    unsigned long long dercedNum[MAX_LEN] = {0};

    // Choose operation
    int menu;
    while (1) {
        // print menu
        printEKey(eKeys, 5);  // your existing printer
        printf("\n\n");
        printf("Select an option:\n");
        printf("1. Encrypt\n2. Decrypt\n3. Show all values\n0. Exit\nYour choice: ");
        if (scanf("%d", &menu) != 1) {
            printf("Invalid input (must be an integer).\n");
            return 0;
        }
        printf("==========================================================================\n");


        switch (menu) {
        case 1: { // Encrypt
            // enter encryption key and message
            encryptKey = enterEncrytKey(phi);
            enterMsg(msg);
            int len = (int)strlen(msg);

            // encrypt message
            for (int i = 0; i < len; ++i) {
                encryptMsg[i] = (unsigned char)msg[i];
                encryptMsg[i] = powMod(encryptMsg[i], encryptKey, n);
            }

            // print encryption result
            printEResult(msg, encryptMsg, len);
            printf("\nTotal characters: %d\n", len);
            printf("==========================================================================\n");
            break;
        }
        case 2: { // Decrypt
            // enter encryption key
            encryptKey = enterEncrytKey(phi);

            // find decryption key
            decrKey = findDecrKey(encryptKey, phi);
            printf("\nDecryption key (d) = %llu\n", decrKey);

            // enter ciphertext
            int cLen = enterNumForDecr(dercMsg, MAX_LEN);

            // decrypt ciphertext
            for (int i = 0; i < cLen; ++i) {
                dercedNum[i] = dercMsg[i];
                dercMsg[i] = powMod(dercMsg[i], decrKey, n);
            }

            // print decryption result
            printf("==========================================================================\n");
            printDResult(dercMsg, dercedNum, cLen);
            printf("==========================================================================\n");
            break;
        }
        case 3: {
            // Show all values
            printf("n = %llu\n", n);
            printf("phi(n) = (p-1)(q-1) = %llu\n", phi);
            printf("==========================================================================\n");
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

static int isPrime(int pr) {
    if (pr < 2) return 0; // 0 and 1 are not prime numbers
    if (pr == 2 || pr == 3) return 1; // 2 is the only even
    if (pr % 2 == 0) return 0; // even numbers greater than 2 are not prime

    int r = (int)sqrt((double)pr); // calculate the square root of pr
    for (int i = 3; i <= r; i += 2) // check for factors from 3 to sqrt(pr)
        if (pr % i == 0) return 0;
    return 1;
}

/* (base^exp) % mod via iterative binary exponentiation */
static unsigned long long powMod(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    if(mod == 1) return 0; // everything mod 1 = 0
    if (base == 0) return 0;
    if (exp == 0) return 1;

    unsigned long long res = 1ULL; 


    if (exp % 2 == 0) {
        res = powMod(base, exp / 2, mod);
        res = (res * res) % mod;
    }
    else {
        res = base % mod;
        res = (res * powMod(base, exp - 1, mod) % mod) % mod;
    }
    return (unsigned long)((res + mod) % mod);
}

static unsigned long long findDecrKey(unsigned long long encryptKey, unsigned long long phi) {
    unsigned long long k = 1;
    while (1) {
        unsigned long long r = (1 + k * phi) % encryptKey;
        if (r == 0)
            return (unsigned long long)(1 + k * phi) / encryptKey;
        k++;
    }
}

/* ---------- UI helpers ---------- */
static void enterPQ(int *p_ptr, int *q_ptr) {
    int pTmp, qTmp;

    while(true){
        printf("Enter the first prime number (p >= 2): ");
        if (scanf("%d", &pTmp) == 1 && isPrime(pTmp)) break;
        printf("Invalid input (must be a prime number >= 2).\n");
        while (getchar() != '\n');
        continue;
    }

    while(true){
        printf("Enter the second prime number (q, different from p): ");
        if (scanf("%d", &qTmp) == 1 && isPrime(qTmp) && qTmp != pTmp) break;
        printf("Invalid input (must be a different prime >= 2).\n");
        while (getchar() != '\n');
        continue;
    }

    *p_ptr = pTmp;
    *q_ptr = qTmp;
}
static void listEncryptKeys(unsigned long long phi, int p, int q, int out[], int cap) {
    for (int i = 0; i < cap; ++i) out[i] = 0;         // sentinel init
    int k = 0;
    for (unsigned long long e = 2; e < phi && k < cap; ++e) {
        if (e == (unsigned long long)p || e == (unsigned long long)q) continue;
        if (gcd(e, phi) == 1ULL) out[k++] = (int)e;
    }
}
static void printEKey(int eKeys[], int cap) {
    printf("Possible encryption keys:\n");
    int shown = 0;
    for (int i = 0; i < cap && eKeys[i] != 0; i++){
        printf("%d\t", eKeys[i]);
        ++shown;
    }
    if(!shown) printf("None\n");
}
static unsigned long long enterEncrytKey(unsigned long long phi) {
    unsigned long long encryptKey;
    while (1) {
        printf("Enter the encryption key (e) where 2 <= e < phi and gcd(e, phi) = 1: ");
        if (scanf("%llu", &encryptKey) != 1) {
            printf("Invalid input (must be an integer).\n");
            while (getchar() != '\n');
            continue;
        }

        if (encryptKey >= 2ULL && encryptKey < phi && gcd(encryptKey, phi) == 1ULL) return encryptKey;
        printf("Invalid encryption key. Try again.\n");
        int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
    }
}
static void enterMsg(char *msg) {
    printf("Enter your message (max 50 chars, no spaces: use '-' instead): ");
    scanf("%49s", msg);
    printf("==========================================================================\n");
}
static int enterNumForDecr(unsigned long long dercMsg[], int cap) {
    int t;
    while(1) {
        printf("How many numbers in the ciphertext? (<= %d): ", cap);
        if (scanf("%d", &t) == 1 && t >= 0 && t <= cap) break;
        printf("Invalid count.\n");
        while (getchar() != '\n');
    }

    for (int i = 0; i < t; ++i) {
        printf("Enter number %d: ", i + 1);
        while (scanf("%llu", &dercMsg[i]) != 1) {
            printf("Invalid number. Try again: ");
            while (getchar() != '\n');
        }
    }
    return t;
}
static void printEResult(const char msg[], const unsigned long long encryptMsg[], int len) {
    printf("= Before encryption (characters):\n");
    for (int i = 0; i < len; ++i) printf("%c\t", msg[i]); 
    printf("\n");

    printf("= Before encryption (ASCII codes):\n");
    for (int i = 0; i < len; ++i) printf("%d\t", (unsigned char)msg[i]); 
    printf("\n");

    printf("= After encryption (numbers):\n");
    for (int i = 0; i < len; ++i) printf("%llu\t", encryptMsg[i]); 
    printf("\n");
}
static void printDResult(const unsigned long long dercMsg[], const unsigned long long dercedNum[], int cLen) {
    printf("= Ciphertext (numbers before decryption):\n");
    for (int i = 0; i < cLen; ++i)
        printf("%llu\t", dercedNum[i]);
    printf("\n");
    
    printf("= Plaintext (numbers after decryption):\n");
    for (int i = 0; i < cLen; ++i)
        printf("%llu\t", dercMsg[i]);
    printf("\n\n");

    printf("Message (as text):\n");
    for (int i = 0; i < cLen; ++i)
        printf("%c", (char)dercMsg[i]);
    printf("\n");
}

/*use 128bits to avoid overflow
static unsigned long long powMod(unsigned long long base,
                                 unsigned long long exp,
                                 unsigned long long mod) {
    if (mod == 1) return 0ULL;      // x mod 1 == 0
    unsigned long long res = 1ULL; // exp == 0 -> 1 % mod

    base %= mod; // shrink base into 0, mod-1

    while (exp) {
        if (exp & 1ULL) {
            __uint128_t x = ( (__uint128_t)res * base );
            res = (unsigned long long)(x % mod);
        }
        __uint128_t y = ( (__uint128_t)base * base );
        base = (unsigned long long)(y % mod);
        exp >>= 1ULL;
    }
    return res; // (base^original_exp) % mod
}*/