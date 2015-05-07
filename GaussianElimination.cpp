#include "Defines.h"

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

/* START SOLUTION */

typedef vvd mat;

int gauss_elim(mat& m, int col_lim) {
  int col = 0, row = 0;
  dbl det = 0; // dbl det = 1;

  // compute reduced row eschalon form
  while(col < col_lim && row < m.size()) {
    // find best row to pivot
    int piv_row = row;
    for(int r = row + 1; r < m.size(); ++r) {
      if(lt(fabs(m[piv_row][col]),
          fabs(m[r][col]))) piv_row = r;
    }

    if(!equ(m[piv_row][col], 0)) {
      if(piv_row != row) {
        swap(m[piv_row], m[row]);
        det = det * -1;
      }

      // make leading entry 1 (for rationals only, to
      // keep num/denom in range)
      //det = det * m[row][col];
      //for(int c = m[0].size() - 1; c >= col; --c)
      //  m[row][c] = m[row][c] / m[row][col];
        
      // zero out all other entries in this column
      for(int r = 0; r < m.size(); ++r) {
        if(r == row) continue;

        dbl fact = m[r][col] / m[row][col];
        for(int c = col; c < m[0].size(); ++c)
          m[r][c] = m[r][c] - fact * m[row][c];
      }

      ++row;
    }
    else det = 0;
    
    ++col;
  }

  // make leading entries 1s (for floats only, delayed to
  // improve numerical stability)
  col = 0, row = 0;
  while(col < col_lim && row < m.size()) {
    while(col < col_lim && equ(m[row][col], 0)) ++col;

    if(col == col_lim) break;

    det = det * m[row][col];
    for(int c = m[0].size() - 1; c >= col; --c)
      m[row][c] = m[row][c] / m[row][col];

    ++row;
  }

  return row; //return det;
}

// assumes m is a non-empty square matrix
// outputs inverse of m in im if an inverse exists
// (i.e. if det(m) != 0) and returns true in this case
bool inv(const mat& m, mat& im) {
  // make im blank
  im.resize(m.size());
  fill(im.begin(), im.end(), vd(2 * m.size()));

  for(int r = 0; r < m.size(); ++r) {
    // copy in m
    for(int c = 0; c < m.size(); ++c)
      im[r][c] = m[r][c];
    // add identity matrix on right
    im[r][m.size() + r] = 1;
  }

  int rank = gauss_elim(im, m.size());

  // keep right side of im only
  for(int r = 0; r < m.size(); ++r) {
    copy(im[r].begin() + m.size(), 
         im[r].end(), im[r].begin());
    im[r].resize(m.size());
  }

  return rank == m.size();
}

/* END SOLUTION */

// example from:
// http://rosettacode.org/wiki/Gaussian_elimination
// expect: -0.01 1.60279 -1.6132 1.24549 -0.49099 0.0657607
int simple_test() {
  mat m = {
    {1.00, 0.00, 0.00,  0.00,  0.00, 0.00, -0.01},
    {1.00, 0.63, 0.39,  0.25,  0.16, 0.10, 0.61},
    {1.00, 1.26, 1.58,  1.98,  2.49, 3.13, 0.91},
    {1.00, 1.88, 3.55,  6.70, 12.62, 23.80, 0.99},
    {1.00, 2.51, 6.32, 15.88, 39.90, 100.28, 0.60},
    {1.00, 3.14, 9.87, 31.01, 97.41, 306.02, 0.02}};

  gauss_elim(m, 6);
  
  for(auto& r : m) {
    for(auto& e : r)
      cout << e << " ";
    cout << endl;
  }
  cout << endl;

  return 0;
}

// fucking solves:
// http://uva.onlinejudge.org/index.php?option=onlinejudge&Itemid=99999999&page=show_problem&category=&problem=1050
// with fc typedefd as dbl and 'rational' changes uncommented in elim code 
int solve_lineq() {
  bool first = true;
  while(true) {
    int P;
    cin >> P;

    if(P == 0) break;

    int U, E;
    cin >> U >> E;

    mat m(E, vd(U+1));

    for(int e = 0; e < E; ++e)
      for(int c = 0; c < U + 1; ++c) cin >> m[e][c];

    int rank = gauss_elim(m, U);
    bool incon = false;
    for(int e = E - 1; e >= rank; --e)
      incon = incon || !(m[e][U] == 0);

    if(!first) cout << "\n";
    cout << "Solution for Matrix System # " << P << "\n";
    
    if(incon) cout << "No Solution.\n";
    else if(rank < U) cout << "Infinitely many solutions "
      << "containing " << (U - rank) << " arbitrary constants.\n";
    else for(int u = 0; u < U; ++u)
      cout << "x[" << u+1 << "] = " << m[u][U] << "\n";

    first = false;
  }
  return 0;
}

// solves:
// http://uva.onlinejudge.org/index.php?option=onlinejudge&Itemid=99999999&page=show_problem&category=&problem=625
// with gauss_elim set to produce det instead of rank
int solve_int_det() {
  while(true) {
    int N;
    cin >> N;

    if(N == 0) {
      cout << "*\n";
      break;
    }

    mat m(N, vd(N));
    for(int r = 0; r < N; ++r)
      for(int c = 0; c < N; ++c) 
        cin >> m[r][c];

    cout << gauss_elim(m, N) << "\n";
  }
}

// checked with wolfram alpha
int simple_inv_test() {
  vector<mat> ms(4), ims(4);

  ms[0] = {
    {1, 2},
    {2, 1},
  };

  ms[1] = {
    {3, 0, 2},
    {2, 0, -2},
    {0, 1, 1},
  };

  ms[2] = {
    {4, 0, 0, 0},
    {0, 0, 2, 0},
    {0, 1, 2, 0},
    {1, 0, 0, 1},
  };

  ms[3] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16},
  };

  for(int i = 0; i < ms.size(); ++i) {
    if(inv(ms[i], ims[i])) {
      cout << "matrix " << i << " has inverse:\n";
      for(vd& r : ims[i]) {
        for(dbl e : r)
          cout << e << " ";
        cout << "\n";
      }
    }
    else cout << "matrix " << i << " has no inverse.\n";
  }
  return 0;
}

int main() {
  return simple_test();
}
