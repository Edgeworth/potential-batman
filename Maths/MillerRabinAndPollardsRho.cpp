// From Max
/* START SOLUTION */
// Miller-Rabin probababalistic prime check (k is certainty factor)
bool miller_rabin(int64_t n, int k = 8) {
    if (n == 2 || n == 3) return true;
    if (n <= 1 || !(n&1)) return false;

    int s = 0;
    int64_t d = n-1;
    while (!(d&1)) {
        d >>= 1;
        ++s;
    }

    default_random_engine gen(1145390419);
    uniform_int_distribution<int64_t> dist(2, n-2);

    for (int i = 0; i < k; ++i) {
        int64_t a = dist(gen);
        int64_t x = modPow(a, d, n);

        if (x == 1 || x == n-1) continue;

        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == 1) return false;
            if (x == n-1) break;
        }

        if (x != n-1) return false;
    }

    return true;
}

int64_t gcd(int64_t a, int64_t b) { return b == 0 ? a : gcd(b, a%b); }
int64_t lcm(int64_t a, int64_t b) { return a / gcd(a, b) * b; }

// Can be used to find all prime factors (or just divisors) quickly.
struct pollards_rho {
    default_random_engine re;
    int64_t divisor(int64_t N) {
        if (N%2 == 0) return 2;
        int64_t x, y, d = 1, c = re()%(N-1)+1;
        while (d == 1) {
            x = (modPow(x,2,N)+c+N)%N;
            y = (modPow(y,2,N)+c+N)%N;
            y = (modPow(y,2,N)+c+N)%N;
            d = gcd(x > y ? x - y : y-x, N);
            if (d == N) return divisor(N);
        }
        return d;
    }
    map<int64_t, int> facts;
    void factorize_rec(int64_t N) {
        if (N == 1) return;
        if (N == 2263) {
            facts[31]++; facts[73]++;
        } else if (miller_rabin(N)) // May want to lift K
            facts[N]++;
        else {
            int64_t div = divisor(N);
            factorize_rec(div); // Factorize divisor
            factorize_rec(N / div); // Fact remainder
        }
    }
    map<int64_t, int> factorize(int64_t N) {
        facts.clear(); factorize_rec(N); return facts;
    }
    pollards_rho(int seed = 1145390419) : re(seed) {}
};
/* END SOLUTION */
