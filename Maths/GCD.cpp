#include "../Defines.h"


/* START SOLUTION */

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

/* END SOLUTION */

int test() {
  ll ps[4][2] = {{13, 7}, {-13, 7}, {-13, -7}, {13, -7}};
  
  for(int i = 0; i < 4; i++) {
    tuple<ll, ll> res = xgcd(ps[i][0], ps[i][1]);
  
    cout << get<0>(res) << " * " << ps[i][0]  << " + " << 
      get<1>(res) << " * " << ps[i][1] << " = " <<
      get<0>(res) * ps[i][0] + get<1>(res) * ps[i][1] << endl;
  }
  return 0;
}

// solves http://uva.onlinejudge.org/index.php?option=onlinejudge&Itemid=99999999&page=show_problem&category=&problem=1031
int solve_marbles() {
  while(true) {
    ll n; 
    cin >> n;
    
    if(n == 0)
      break;

    ll c1, n1, c2, n2;
    cin >> c1 >> n1 >> c2 >> n2;

    ll s, t;
    tie(s, t) = xgcd(n1, n2);
    const ll d = s * n1 + t * n2;

    if(n % d != 0) {
      cout << "failed" << endl;
      continue;
    }

    const ll k = n / d;
    const ll bi = (ll)ceil(-1.0 * d * k * s / n2);
    const ll ti = (ll)floor(1.0 * d * k * t / n1);

    if(bi > ti) {
      cout << "failed" << endl;
      continue;
    }

    const ll dC = c1 * n2 / d - c2 * n1 / d;
    const ll i = dC < 0 ? ti : bi;
    const ll m1 = k * s + i * n2 / d, m2 = k * t - i * n1 / d;

    cout << m1 << " " << m2 << endl;
  }
  return 0;
}

int main() {
  return solve_marbles();
}
