#include "../Defines.h"

/* START SOLUTION */

// Returns a map of i to i's partner for a matching where
// the sum of the costs cost[i][ret[i]] is maximal.
vi max_matching(vvi &cost) {
  int n = cost.size();
  vi lx(n, 0),  ly(n, 0);  // labels on x, y
  vi my(n, -1), mx(n, -1); // yi is matched with my[yi]
  int match_size = 0;

  for (int x = 0; x < n; x++) { // initial labeling
    for (int y = 1; y < n; y++) {
      lx[x] = max(lx[x], cost[x][y]);
    }
  }

  while (match_size < n) {
    // create an alternating tree in the tight graph
    vb tx(n, false), ty(n, false); // xs, ys in the tree
    vi prev(n, -1);  // each x's grandparent
    queue<int> q; // xs in the tree with some tight edges
    vi slack(n); // each y's least slack edge to the tree
    vi slack_partner(n);

    // choose some free x with which to root our tree
    for (int x = 0; x < n; x++) {
      if (mx[x] == -1) {
        tx[x] = true;
        prev[x] = -2;

        // initial slackness
        for (int y = 0; y < n; y++) {
          slack[y] = lx[x] + ly[y] - cost[x][y];
          slack_partner[y] = x;
        }

        q.push(x);
        break;
      }
    }

    // BFS to find an augmenting path in the alternating tree
    int free_y = -1; // the other end of our augmenting path
    int free_y_parent = -1;
    while (free_y == -1) {
      // Grow the tree using a BFS.
      while (!q.empty() && free_y == -1) {
        int x = q.front();
        q.pop();

        for (int y = 0; y < n; y++) {
          if (ty[y]) continue;
          if (lx[x] + ly[y] != cost[x][y]) continue;

          if (my[y] == -1) { 
            free_y = y;
            free_y_parent = x;
            break; // Augmenting path found!
          } else {
            // Add y and its match, m, to the tree.
            int m = my[y];
            tx[m] = ty[y] = true;
            prev[m] = x;
            q.push(m);

            // Update least slack cache as the tree's changed
            for (int yy = 0; yy < n; yy++) {
              if (lx[m] + ly[yy] - cost[m][yy] < slack[yy]) {
                slack[yy] = lx[m] + ly[yy] - cost[m][yy];
                slack_partner[yy] = m;
              }
            }
          }
        }
      } // BFS

      if (free_y != -1) break;

      // We have run out of tight edges but not yet found an
      // augmenting path. Time to make another tight edge by
      // improving the labeling.
      int d = INT_MAX; // Least slack found, delta.
      for (int y = 0; y < n; y++) {
        if (!ty[y]) {
          d = min(d, slack[y]);
        }
      }

      // Update labels and least slack cache.
      for (int x = 0; x < n; x++) {
        if (tx[x]) lx[x] -= d;
      }
      for (int y = 0; y < n; y++) {
        if (ty[y]) ly[y] += d;
        else slack[y] -= d;
      }

      // Make sure our BFS will find newly reachable ys.
      for (int y = 0; y < n; y++) {
        if (slack[y] == 0 && !ty[y] && tx[slack_partner[y]]){
          q.push(slack_partner[y]);
        }
      }
    } // while (free_y == -1)

    // We have an augmenting path, apply it.
    int x = free_y_parent;
    int y = free_y;
    while (x != -2) {
      int next_y = mx[x];
      mx[x] = y;
      my[y] = x;
      y = next_y;
      x = prev[x];
    }
    match_size++;
  }
  return mx;
}

/* END SOLUTION */

int simple_test() {
  while (!cin.eof()) {
    int N;
    cin >> N;
    vvi cost(N, vi(N));
    for (int x = 0; x < N; x++) {
      for (int y = 0; y < N; y++) {
        cin >> cost[x][y];
      }
    }
    cin >> ws;

    vi mx = max_matching(cost);
    // Calculate matching cost.
    int total = 0;
    for (int x = 0; x < mx.size(); x++) {
      total += cost[x][mx[x]];
    }
    cout << total << endl;
  }
  return 0;
}

static double EPS = 1E-6;

// http://uva.onlinejudge.org/external/100/p10072.pdf
void solve_cricket_team() {
  int caze = 1;
  while (true) {
    int N;
    cin >> N;
    if (N == 0) break;

    vi bt(N);
    vi bl(N);
    vi fl(N);
    for (int i = 0; i < N; i++) {
      cin >> bt[i] >> bl[i] >> fl[i];
    }

    int BT, BL, AR;
    cin >> BT >> BL >> AR;
    
    vvi adj(N, vi(N, 0));
    for (int i = 0; i < BT + BL + AR; ++i) {
      for (int j = 0; j < N; ++j) {
        if      (i < BT)      adj[i][j] = int(0.5 + EPS + 0.8 * (double) bt[j] +                        0.2 * (double) fl[j]);
        else if (i < BT + BL) adj[i][j] = int(0.5 + EPS + 0.1 * (double) bt[j] + 0.7 * (double) bl[j] + 0.2 * (double) fl[j]);
        else                  adj[i][j] = int(0.5 + EPS + 0.4 * (double) bt[j] + 0.4 * (double) bl[j] + 0.2 * (double) fl[j]);
      }
    }

    vi match = max_matching(adj);

    if (caze > 1) cout << endl;
    cout << "Team #" << caze++ << endl;
    int tot = 0;
    set<int> bts, bls, ars;
    for (int i = 0; i < BT + BL + AR; ++i) {
      tot += adj[i][match[i]];
      if      (i < BT)      bts.insert(match[i] + 1);
      else if (i < BT + BL) bls.insert(match[i] + 1);
      else                  ars.insert(match[i] + 1);
    }
    cout << "Maximum Effective Score = " << tot;
    cout << endl << "Batsmen :";
    for (int i : bts) cout << " " << i;
    cout << endl << "Bowlers :";
    for (int i : bls) cout << " " << i;
    cout << endl << "All-rounders :";
    for (int i : ars) cout << " " << i;
    cout << endl;
  }
}



int main() {
  solve_cricket_team();
  //return simple_test();
}
