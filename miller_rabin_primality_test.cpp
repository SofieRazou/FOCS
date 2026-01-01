#include <iostream>
#include <vector>
using namespace std;

typedef unsigned long long ull;

// Modular exponentiation using __uint128_t
ull modexp(ull base, ull exp, ull mod) {
    ull result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            result = (__uint128_t(result) * base) % mod;
        base = (__uint128_t(base) * base) % mod;
        exp >>= 1;
    }
    return result;
}

// Deterministic Miller–Rabin for n < 2^64
bool miller_rabin(ull n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;

    ull t = n - 1;
    int s = 0;
    while ((t & 1) == 0) { t >>= 1; s++; }

    ull bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};

    for (ull a : bases) {
        if (a >= n) continue;
        ull y = modexp(a, t, n);
        if (y == 1 || y == n - 1) continue;

        bool isComposite = true;
        for (int i = 1; i < s; i++) {
            y = (__uint128_t(y) * y) % n;
            if (y == n - 1) { isComposite = false; break; }
        }
        if (isComposite) return false;
    }
    return true;
}

vector<int> primes_up_to(int n) {
    vector<int> primes;
    for (int i = 2; i <= n; i++) {
        if (miller_rabin(i)) {
            primes.push_back(i);
        }
    }
    return primes;
}

// Find Mersenne primes for prime exponents <= 63
vector<ull> mersenne_primes() {
    vector<ull> mp;
    vector<int> primes = primes_up_to(63);
    for (int p : primes) {
        ull m = (1ULL << p) - 1;
        if (miller_rabin(m)) mp.push_back(m);
    }
    return mp;
}

int main() {
    vector<ull> mp = mersenne_primes();
    for (ull m : mp) cout << m << "\n";
    return 0;
}
