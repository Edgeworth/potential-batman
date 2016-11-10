// From Gozz
/* START SOLUTION */
// Two-phase simplex algorithm for solving linear programs of the form
//
//     maximize     c^T x
//     subject to   Ax <= b
//                  x >= 0
//
// INPUT: A -- an m x n matrix
//        b -- an m-dimensional vector
//        c -- an n-dimensional vector
//        x -- a vector where the optimal solution will be stored
//
// OUTPUT: value of the optimal solution (infinity if unbounded
//         above, nan if infeasible)
//
// To use this code, create an SimplexSolver object with A, b, and c as
// arguments.  Then, call Solve(x).
struct SimplexSolver {
  int m, n;
  vi B, N;
  vve D;
  SimplexSolver(const vve &A, const ve &b, const ve &c) : m(b.size()), n(c.size()), N(n + 1), B(m), D(m + 2, ve(n + 2)) {
    for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j) D[i][j] = A[i][j];
    for (int i = 0; i < m; ++i) { B[i] = n + i; D[i][n] = -1; D[i][n + 1] = b[i]; }
    for (int j = 0; j < n; ++j) { N[j] = j; D[m][j] = -c[j]; }
    N[n] = -1; D[m + 1][n] = 1;
  }
  void pivot(int r, int s) {
    for (int i = 0; i < m + 2; ++i) if (i != r)
      for (int j = 0; j < n + 2; ++j) if (j != s)
        D[i][j] -= D[r][j] * D[i][s] / D[r][s];
    for (int j = 0; j < n + 2; ++j) if (j != s) D[r][j] /= D[r][s];
    for (int i = 0; i < m + 2; ++i) if (i != r) D[i][s] /= -D[r][s];
    D[r][s] = el_t(1) / D[r][s];
    swap(B[r], N[s]);
  }
  bool simplex(int phase) {
    int x = phase == 1 ? m + 1 : m;
    while (true) {
      int s = -1;
      for (int j = 0; j <= n; ++j) {
        if (phase == 2 && N[j] == -1) continue;
        if (s == -1 || D[x][j] < D[x][s] || D[x][j] == D[x][s] && N[j] < N[s]) s = j;
      }
      if (-EPS < D[x][s]) return true;
      int r = -1;
      for (int i = 0; i < m; ++i) {
        if (D[i][s] < EPS) continue;
        if (r == -1 || D[i][n + 1] / D[i][s] < D[r][n + 1] / D[r][s] || (D[i][n + 1] / D[i][s]) == (D[r][n + 1] / D[r][s]) && B[i] < B[r]) r = i;
      }
      if (r == -1) return false;
      pivot(r, s);
    }
  }
  el_t solve(ve &x) {
    int r = 0;
    for (int i = 1; i < m; ++i) if (D[i][n + 1] < D[r][n + 1]) r = i;
    if (D[r][n + 1] < -EPS) {
      pivot(r, n);
      if (!simplex(1) || D[m + 1][n + 1] < -EPS) return -numeric_limits<el_t>::infinity();
      for (int i = 0; i < m; ++i) if (B[i] == -1) {
        int s = -1;
        for (int j = 0; j <= n; ++j)
          if (s == -1 || D[i][j] < D[i][s] || D[i][j] == D[i][s] && N[j] < N[s]) s = j;
        pivot(i, s);
      }
    }
    if (!simplex(2)) return numeric_limits<el_t>::infinity();
    x = ve(n);
    for (int i = 0; i < m; ++i) if (B[i] < n) x[B[i]] = D[i][n + 1];
    return D[m][n + 1];
  }
};
/* END SOLUTION */
