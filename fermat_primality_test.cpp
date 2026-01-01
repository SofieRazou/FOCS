#include <iostream>
using namespace std;

// Modular exponentiation without overflow
unsigned long long modexp(unsigned long long a, unsigned long long n, unsigned long long p) {
    __uint128_t res = 1;
    __uint128_t base = a % p;

    while (n > 0) {
        if (n & 1) {
            res = (res * base) % p;
        }
        base = (base * base) % p;
        n >>= 1;
    }
    return (unsigned long long)res;
}

// Fermat primality test with fixed bases
bool fermat_prime_test(unsigned long long n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;

    // Fixed small prime bases for determinism
    unsigned long long bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    int k = sizeof(bases) / sizeof(bases[0]);

    for (int i = 0; i < k; ++i) {
        unsigned long long a = bases[i];
        if (a >= n) continue; // skip bases >= n
        if (modexp(a, n - 1, n) != 1)
            return false;
    }
    return true;
}

int main() {
    unsigned long long n;
    cout << "Enter a number: ";
    cin >> n;

    if (fermat_prime_test(n))
        cout << n << " is probably prime." << endl;
    else
        cout << n << " is composite." << endl;

    return 0;
}
// int main() {
//     int p = 2281;
//     unsigned long long bases[] = {2, 3, 5, 7, 11};
//     bool prime = true;

//     for (auto a : bases) {
//         if (modexp(a, (1ULL << p) - 2, p) != 1) {
//             prime = false;
//             break;
//         }
//     }

//     if (prime)
//         cout << "2^" << p << " - 1 is probably prime." << endl;
//     else
//         cout << "Composite." << endl;
// }