#include <iostream>
#include <vector>
#include <utility>
#include <chrono>
#include <gmpxx.h>
#include <fstream>
#include <algorithm>

using namespace std;

// Euclidean GCD
mpz_class gcd(mpz_class a, mpz_class b) {
    while (b != 0) {
        a %= b;
        a.swap(b);
    }
    return a;
}

// Optimized Binary GCD
mpz_class bgcd_superfast(mpz_class a, mpz_class b) {
    if (a == 0) return b;
    if (b == 0) return a;

    int shift = 0;

    while (((a | b) & 1) == 0) {
        size_t tz_a = mpz_scan1(a.get_mpz_t(), 0);
        size_t tz_b = mpz_scan1(b.get_mpz_t(), 0);
        size_t min_tz = min(tz_a, tz_b);
        a >>= min_tz;
        b >>= min_tz;
        shift += min_tz;
    }

    a >>= mpz_scan1(a.get_mpz_t(), 0);

    while (b != 0) {
        b >>= mpz_scan1(b.get_mpz_t(), 0);
        if (a > b) swap(a, b);
        b -= a;
    }

    return a << shift;
}

int main() {
    vector<pair<mpz_class, mpz_class>> nums;

    // Random generator
    gmp_randclass rng(gmp_randinit_mt);
    rng.seed(123456); // reproducible

    // Generate test cases with increasing bit size
    for (size_t bits = 256; bits <= 8192; bits *= 2) {
        for (int i = 0; i < 5; ++i) {  // multiple samples per size
            mpz_class a = rng.get_z_bits(bits);
            mpz_class b = rng.get_z_bits(bits);
            nums.emplace_back(a, b);
        }
    }

    ofstream fout("gcd_times.csv");
    fout << "bits,euclid_us,binary_us\n";

    mpz_class sink = 0;

    for (size_t i = 0; i < nums.size(); i++) {
        auto &p = nums[i];

        size_t bits_a = mpz_sizeinbase(p.first.get_mpz_t(), 2);
        size_t bits_b = mpz_sizeinbase(p.second.get_mpz_t(), 2);
        size_t bits = max(bits_a, bits_b);

        auto start = chrono::high_resolution_clock::now();
        mpz_class r1 = gcd(p.first, p.second);
        auto end = chrono::high_resolution_clock::now();
        auto euclid_time = chrono::duration_cast<chrono::microseconds>(end - start).count();
        sink += r1;

        start = chrono::high_resolution_clock::now();
        mpz_class r2 = bgcd_superfast(p.first, p.second);
        end = chrono::high_resolution_clock::now();
        auto binary_time = chrono::duration_cast<chrono::microseconds>(end - start).count();
        sink += r2;

        cout << "Bits = " << bits
             << " | Euclid = " << euclid_time
             << " µs, Binary = " << binary_time << " µs\n";

        fout << bits << "," << euclid_time << "," << binary_time << "\n";
    }

    fout.close();
    cout << "Benchmark completed. Results saved to gcd_times.csv\n";
    return 0;
}
