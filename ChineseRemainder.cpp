#include "Defines.h"

// do not use for neg values
tuple<ll, ll> xgcd_pos(ll a, ll b) {
  if(b == 0) return make_tuple(1, 0);
  
  ll r = a % b;
  ll q = a / b;
  
  ll s, t;
  tie(s, t) = xgcd_pos(b, r);
  return make_tuple(t, s - q * t);
}
  
// returns (s, t) s.t. gcd(a, b) = sa + tb
// handles negs and 0s

// Bezout's lemma:
// if (s, t) is as above, then the set of pairs
// satisfying the same equation is given by:
// (s + k * b / gcd(a, b), t - k * a / gcd(a, b))
// for k ranging over the integers
tuple<ll, ll> xgcd(ll a, ll b) {
  tuple<ll, ll> res = xgcd_pos(abs(a), abs(b));
  get<0>(res) *= (a < 0) ? -1 : 1;
  get<1>(res) *= (b < 0) ? -1 : 1;
  
  return res;
}

/* START SOLUTION */

// we have a system of congruences:
// x = a1 mod n1, x = a2 mod n2, ...
// with ns pairwise coprime, then we solve for x, and
// x + k*n1*n2*... is also a soln for any integer k

// if the ns are not all pairwise coprime, a soln
// exists iff ai = aj mod gcd(ni, nj) for all i, j,
// and x + k * lcm(n1, n2, ...) is a soln for any
// integer k
// we do not handle the non-coprime case
ll chinese_remainder(const vll& ns, const vll& as) {
  ll x = 0;
  ll N = 1;
  for(ll n : ns) N *= n;

  for(int i = 0; i < ns.size(); ++i) {
    ll s;
    tie(s, _) = xgcd(N / ns[i], ns[i]);
    s *= N / ns[i];
    // s = s mod N (works even with negative s)
    s = (s % N + N) % N;
     
    x += as[i] * s;
    x = x % N;
  }

  return x;
}

int simple_test() {
  // x = 2 mod 3, x = 3 mod 4, x = 1 mod 5
  //   => x = 11 mod 60
  cout << chinese_remainder({3, 4, 5}, {2, 3, 1}) << "\n";
  return 0;
}

int solve_biorythms() {
  const int N = 23 * 28 * 33;
  int caze = 1;
  while(true) {
    ll p, e, i, d, x;
    cin >> p >> e >> i >> d;

    if(p == -1) break;

    x = chinese_remainder({23, 28, 33}, {p % 23, e % 28, i % 33});
    ll ans = x - d;
    if(ans <= 0) ans += N;
    cout << "Case " << caze++ << ": the next triple peak occurs in "
       << ans << " days.\n";
  }
  return 0;
}

int main() {
  return solve_biorythms();
}
