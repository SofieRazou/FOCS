// #include <iostream>
// #include <vector>
// #include <utility>
// #include <chrono>
// #include <fstream>
// #include <boost/multiprecision/cpp_int.hpp>

// using namespace boost::multiprecision;
// using namespace std;
// #include <boost/multiprecision/cpp_int.hpp>
// #include <gmp.h>
// #include <iostream>
// #include <chrono>

// using namespace boost::multiprecision;
// using namespace std;


// // Euclidean GCD
// cpp_int gcd(cpp_int a, cpp_int b) {
//     while (b != 0) {
//         cpp_int temp = b;
//         b = a % b;
//         a = temp;
//     }
//     return a;
// }

// // Optimized Binary GCD (Stein's algorithm) - iterative version
// cpp_int bgcd(cpp_int a, cpp_int b) {
//     if (a == 0) return b;
//     if (b == 0) return a;

//     // Find common factors of 2
//     int shift = 0;
//     while (((a | b) & 1) == 0) { // both even
//         a >>= 1;
//         b >>= 1;
//         ++shift;
//     }

//     // Make a odd
//     while ((a & 1) == 0) a >>= 1;

//     while (b != 0) {
//         // Make b odd
//         while ((b & 1) == 0) b >>= 1;

//         if (a > b) std::swap(a, b);
//         b -= a;
//     }

//     return a << shift;
// }


// int main() {
//     // 10 pairs of really large numbers (~200-300 digits each)
//     vector<pair<cpp_int, cpp_int>> nums = {
//         {cpp_int("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"),
//          cpp_int("9876543210987654321098765432109876543210987654321098765432109876543210987654321098765432109876543210")},
//         {cpp_int("11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111") << 100,
//          cpp_int("22222222222222222222222222222222222222222222222222222222222222222222222222222222222222222") << 95},
//         {cpp_int("3141592653589793238462643383279502884197169399375105820974944592") << 80,
//          cpp_int("2718281828459045235360287471352662497757247093699959574966967627") << 75},
//         {cpp_int("13579135791357913579135791357913579135791357913579135791357913579") << 70,
//          cpp_int("24680246802468024680246802468024680246802468024680246802468024680") << 65},
//         {cpp_int("999999999999999999999999999999999999999999999999999999999999999999999999999999999") << 60,
//          cpp_int("888888888888888888888888888888888888888888888888888888888888888888888888888888888") << 55},
//         {cpp_int("123456789012345678901234567890123456789012345678901234567890") << 50,
//          cpp_int("987654321098765432109876543210987654321098765432109876543210") << 50},
//         {cpp_int("314159265358979323846264338327950288419716939937510") << 80,
//          cpp_int("271828182845904523536028747135266249775724709369995") << 80},
//         {cpp_int("13579246801357924680135792468013579246801357924680") << 70,
//          cpp_int("24680135792468013579246801357924680246801357924680") << 70},
//         {cpp_int("98765432101234567890987654321012345678909876543210") << 60,
//          cpp_int("12345678909876543210123456789098765432101234567890") << 60},
//         {cpp_int("99999999999999999999999999999999999999999999999999") << 80,
//          cpp_int("88888888888888888888888888888888888888888888888888") << 75}
//     };

//     ofstream fout("gcd_times.csv");
//     fout << "euclid_us,binary_us\n";

//     cpp_int sink = 0;

//     for (size_t i = 0; i < nums.size(); i++) {
//         auto &p = nums[i];

//         auto start = chrono::high_resolution_clock::now();
//         cpp_int r1 = gcd(p.first, p.second);
//         auto end = chrono::high_resolution_clock::now();
//         auto euclid_time = chrono::duration_cast<chrono::microseconds>(end - start).count();
//         sink += r1;

//         start = chrono::high_resolution_clock::now();
//         cpp_int r2 = bgcd(p.first, p.second); // use optimized binary GCD
//         end = chrono::high_resolution_clock::now();
//         auto binary_time = chrono::duration_cast<chrono::microseconds>(end - start).count();
//         sink += r2;

//         cout << "Test case " << i+1 << ": Euclid = " << euclid_time
//              << " µs, Binary = " << binary_time << " µs" << endl;

//         fout << euclid_time << "," << binary_time << "\n";
//     }

//     fout.close();
//     cout << "Benchmark completed. Results saved to gcd_times.csv" << endl;
//     return 0;
// }
// #include <iostream>
// #include <vector>
// #include <utility>
// #include <chrono>
// #include <gmpxx.h>
// #include <fstream>
// using namespace std;

// // Euclidean GCD
// mpz_class gcd(mpz_class a, mpz_class b) {
//     while (b != 0) {
//         // reuse r to avoid repeated allocations
//         mpz_class r;
//         mpz_tdiv_r(r.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());  // direct modulo
//         a.swap(b);  // swap internal data, avoids copying
//         b = r;
//     }
//     return a;
// }

// // Optimized Binary GCD 
// mpz_class bgcd_superfast(mpz_class a, mpz_class b) {
//     if (a == 0) return b;
//     if (b == 0) return a;

//     int shift = 0;

//     // Remove common factors of 2 using mpz_scan1
//     while (((a | b) & 1) == 0) {
//         size_t tz_a = mpz_scan1(a.get_mpz_t(), 0);
//         size_t tz_b = mpz_scan1(b.get_mpz_t(), 0);
//         size_t min_tz = min(tz_a, tz_b);
//         a >>= min_tz;
//         b >>= min_tz;
//         shift += min_tz;
//     }

//     // Make a odd
//     a >>= mpz_scan1(a.get_mpz_t(), 0);

//     while (b != 0) {
//         b >>= mpz_scan1(b.get_mpz_t(), 0);
//         if (a > b) swap(a, b);
//         b -= a;
//     }

//     return a << shift;
// }

// int main() {
//     // 10 pairs of really really large numbers (not as much as 10^42 ofc :))
//     vector<pair<mpz_class, mpz_class>> nums = {
//         {mpz_class("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"),
//          mpz_class("9876543210987654321098765432109876543210987654321098765432109876543210987654321098765432109876543210")},
//         {mpz_class("11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111") << 100,
//          mpz_class("22222222222222222222222222222222222222222222222222222222222222222222222222222222222222222") << 95},
//         {mpz_class("3141592653589793238462643383279502884197169399375105820974944592") << 80,
//          mpz_class("2718281828459045235360287471352662497757247093699959574966967627") << 75},
//         {mpz_class("13579135791357913579135791357913579135791357913579135791357913579") << 70,
//          mpz_class("24680246802468024680246802468024680246802468024680246802468024680") << 65},
//         {mpz_class("999999999999999999999999999999999999999999999999999999999999999999999999999999999") << 60,
//          mpz_class("888888888888888888888888888888888888888888888888888888888888888888888888888888888") << 55},
//         {mpz_class("123456789012345678901234567890123456789012345678901234567890") << 50,
//          mpz_class("987654321098765432109876543210987654321098765432109876543210") << 50},
//         {mpz_class("314159265358979323846264338327950288419716939937510") << 80,
//          mpz_class("271828182845904523536028747135266249775724709369995") << 80},
//         {mpz_class("13579246801357924680135792468013579246801357924680") << 70,
//          mpz_class("24680135792468013579246801357924680246801357924680") << 70},
//         {mpz_class("98765432101234567890987654321012345678909876543210") << 60,
//          mpz_class("12345678909876543210123456789098765432101234567890") << 60},
//         {mpz_class("99999999999999999999999999999999999999999999999999") << 80,
//          mpz_class("88888888888888888888888888888888888888888888888888") << 75}
//     };

//  sort(nums.begin(), nums.end(), [](const pair<mpz_class, mpz_class>& p1, const pair<mpz_class, mpz_class>& p2) {
//     size_t max1 = std::max(mpz_sizeinbase(p1.first.get_mpz_t(), 2), mpz_sizeinbase(p1.second.get_mpz_t(), 2));
//     size_t max2 = std::max(mpz_sizeinbase(p2.first.get_mpz_t(), 2), mpz_sizeinbase(p2.second.get_mpz_t(), 2));
//     return max1 > max2; // largest number first
// });




//     ofstream fout("gcd_times.csv");
//     fout << "euclid_us,binary_us\n";

//     mpz_class sink = 0; 

//     for (size_t i = 0; i < nums.size(); i++) {
//         auto &p = nums[i];

//         auto start = chrono::high_resolution_clock::now();
//         mpz_class r1 = gcd(p.first, p.second);
//         auto end = chrono::high_resolution_clock::now();
//         auto euclid_time = chrono::duration_cast<chrono::microseconds>(end - start).count();
//         sink += r1;

//         start = chrono::high_resolution_clock::now();
//         mpz_class r2 = bgcd_superfast(p.first, p.second);
//         end = chrono::high_resolution_clock::now();
//         auto binary_time = chrono::duration_cast<chrono::microseconds>(end - start).count();
//         sink += r2;

//         cout << "Test case " << i+1 << ": Euclid = " << euclid_time
//              << " µs, Binary = " << binary_time << " µs" << endl;

//         fout << euclid_time << "," << binary_time << "\n";
//     }
//     fout.close();
//     cout << "Benchmark completed. Results saved to gcd_times.csv" << endl;
//     return 0;
// }
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
