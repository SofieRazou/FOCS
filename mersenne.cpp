##include <iostream>
#include <vector>
using namespace std;

typedef unsigned long long ull;

// Deterministic Miller-Rabin for n < 2^64
bool miller_rabin(ull n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;

    ull d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }

    ull bases[] = {2,3,5,7,11,13,17,19,23,29,31,37};

    for (ull a : bases) {
        if (a >= n) continue;
        ull x = 1, p = a, exp = d;
        while (exp) {
            if (exp & 1) x = (__uint128_t)x * p % n;
            p = (__uint128_t)p * p % n;
            exp >>= 1;
        }

        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (int r = 1; r < s; r++) {
            x = (__uint128_t)x * x % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

vector<ull> mersenne_primes() {
    vector<ull> mp;
    // Only prime exponents
    int prime_exponents[] = {2,3,5,7,13,17,19,31,61}; // i < 64
    for (int x : prime_exponents) {
        ull mx = (1ULL << x) - 1;  // 2^x - 1
        if (miller_rabin(mx)) mp.push_back(mx);
    }
    return mp;
}

int main() {
    vector<ull> m = mersenne_primes();
    for (ull n : m) cout << n << " ";
    cout << endl;
    return 0;
}
