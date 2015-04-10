#include "Defines.h"

/* START SOLUTION */

// non-recursive topological sort
// g      : adjacency list of DAC input graph
// ordered: vector (which may initially be any size) 
//          in which the topologically sorted indices
//          are stored
void top_sort(const vvi& g, vi& ordered) {
    ordered.resize(0);
    vi indegs(g.size());

    for(int c = 0; c < g.size(); ++c)
        for(int n : g[c])
            ++indegs[n];

    queue<int> q;
    for(int c = 0; c < g.size(); ++c)
        if(indegs[c] == 0)
            q.push(c);

    while(!q.empty()) {
        int c = q.front();
        q.pop();

        // alternatively, process node here
        ordered.push_back(c);

        for(int n : g[c])
            if(--indegs[n] == 0)
                q.push(n);
    }
}

/* END SOLUTION */

// solves http://uva.onlinejudge.org/index.php?option=onlinejudge&Itemid=99999999&page=show_problem&category=156&problem=1246
int solve_ordering_tasks() {
    while(true) {
        int n, m;
        cin >> n >> m;

        if(n == 0 && m == 0)
            break;

        vvi g(n);
        vi ordering;
        for(int k = 0; k < m; ++k) {
            int i, j;
            cin >> i >> j;

            g[i-1].push_back(j-1);
        }

        top_sort(g, ordering);

        for(int k = 0; k < n; ++k) {
            if(k != 0)
                cout << " ";
            cout << ordering[k] + 1;
        }
        cout << endl;
    }
    return 0;
}

int main() {
    return solve_ordering_tasks();
}
