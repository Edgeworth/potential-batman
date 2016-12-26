#include "..Defines.h"

/* START SOLUTION */

ll gcd(ll a, ll b) { return b != 0 ? gcd(b, a % b) : a; }

// use g++'s __int128 for more precision
struct fc {
  ll n, d;
  fc(ll v = 0) : fc(v, 1) {}
  fc(ll _n, ll _d) : n(_n), d(_d) {
    while(d == 0) {}
    cons();
  }

  void cons() {
    if(n == 0) d = 1;
    else {
      ll e = gcd(abs(n), abs(d));
      n /= e;
      d /= e;
      if(d < 0) {
        n *= -1;
        d *= -1;
      }
    }
  }

  fc operator+(const fc& o) const {
    ll e = gcd(d, o.d);
    ll nn = n * (o.d / e) + o.n * (d / e);
    ll nd = (d / e) * o.d;
    return fc(nn, nd);
  }

  fc operator*(const fc& o) const {
    ll e1 = gcd(abs(n), o.d);
    ll e2 = gcd(d, abs(o.n));
    ll nn = (n / e1) * (o.n / e2);
    ll nd = (d / e2) * (o.d / e1);
    return fc(nn, nd);
  }

  fc operator-(const fc& o) const {
    return *this + fc(-1) * o;
  }

  fc operator/(const fc& o) const {   
    return *this * fc(o.d, o.n);
  }

  fc operator+=(const fc& o) {
    return *this = *this + o;
  } // etc. for ops

  bool operator==(const fc& o) const {
    return n == o.n && d == o.d;
  }

  bool operator<(const fc& o) const {
    return (*this - o).n < 0;
  }

  friend ostream& operator<<(ostream& os, const fc& f);
  friend istream& operator>>(istream& is, fc& f);
};

istream& operator>>(istream& is, fc& f) {
  char c;
  ll n, d = 1;

  is >> n >> ws;
  if(is.peek() == '/') is >> c >> d;

  f = fc(n, d);
  return is;
}

ostream& operator<<(ostream& os, const fc& f) {
  os << f.n;
  if(f.d > 1) os << "/" << f.d;
  return os;
}

/* END SOLUTION */

int main() {
    // tested with gaussian elim
    return 0;
}
