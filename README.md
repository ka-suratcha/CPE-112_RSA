# CPE-112_RSA

## 📖 Overview
This project was created as part of **CPE-112: Computer Programming Laboratory**.  

The goal of this assignment is use thing you learn from the lecture into a working program.  

I decided to make a **simple RSA encryption/decryption program**, because:
- RSA requires a lot of math, which I learned in **CPE-200: Computer Engineering Mathematics**.
- It is a good way to learn and practice the **C programming language**.
- It also helps you remember things you learned about cryptography and number theory.

### 🔐 About RSA:
RSA is an asymmetric cryptographic algorithm that uses two keys: a private key to decrypt data and a public key to encrypt it. Its security comes from how hard it is to factor big prime numbers.

---

## ✨ Features
- a Use two prime numbers, p and q, that the user gives you to make RSA keys.
- Find `n` and Euler's Totient `φ(n)`.
- Suggest valid encryption keys `e` for the user.
- **Encrypt messages** (up to 50 characters, based on ASCII).
- **Decrypt ciphertext** back into plain text using the calculated decryption key `d`.
- For educational purposes, show all the intermediate values, such as "n," "φ(n)," keys, ciphertext, and so on.
- Check that the input for primes, encryption keys, and ciphertext values is correct.

---

## 🧩 Flowchart
(added later)

## ▶️ Example Workflow
1. **Enter two primes** (e.g., `p = 17`, `q = 11`).
- Program calculates `n = p * q` and `φ(n) = (p-1)(q-1)`.
2. **Choose operation**  
- **Encrypt**: Enter encryption key `e` and a message.  
- **Decrypt**: Enter the ciphertext and the program will compute the decryption key `d` automatically.
4. **View results**  
- See before/after ASCII values, encrypted numbers, and final decrypted text.

---

## 🚀 Future Improvements
- Add support for **bigger key sizes** (currently limited by `unsigned long long`).
- For a more realistic RSA, use padding schemes like PKCS#1.
- Let people type in longer things and **space characters** instead of just hyphens (`-`).
- Add **UI/UX** by making the program easier to use (better error handling and menu navigation).
- Increased security by use **128-bit integer arithmetic** 
---